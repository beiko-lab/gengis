//=======================================================================
// Author: Alexander Keddy
//
// Copyright 2013 Alexander Keddy
//
// This file is part of GenGIS.
//
// GenGIS is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// GenGIS is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GenGIS.  If not, see <http://www.gnu.org/licenses/>.
//=======================================================================

#include "../core/Precompiled.hpp"

#include "../core/LocationLayer.hpp"
#include "../core/SequenceLayer.hpp"
#include "../core/TileModel.hpp"
#include "../core/LocationModel.hpp"

//#include "../core/LocationController.hpp"

using namespace GenGIS;

void TileModel::AddLocationLayer(LocationLayerPtr location)
{
	// Update number of locations in Tile
	numLocations++;
	// Add sequence layers to tile?
	std::vector<SequenceLayerPtr> sequences = location->GetAllSequenceLayers();
	UpdateSequences(sequences);
	// Add fields to tile
	UpdateData(location->GetLocationController()->GetData());
}

template<class Archive> 
void TileModel::serialize(Archive & ar, const unsigned int version)
{
	// Serialize base class (LocationModel)
	ar & boost::serialization::base_object<LocationModel>(*this);

	ar & numLocations;  // uint
	ar & m_bActive;     // bool
	ar & m_colour;		// Colour
	ar & m_combinationMethod; // DATA_COMBINE
}
template void TileModel::serialize<boost::archive::text_woarchive>(boost::archive::text_woarchive& ar, const unsigned int version); 
template void TileModel::serialize<boost::archive::text_wiarchive>(boost::archive::text_wiarchive& ar, const unsigned int version);

void TileModel::UpdateData(std::map<std::wstring,std::wstring> newData)
{
	std::map<std::wstring,std::wstring>::iterator dataIter;
	for(dataIter = newData.begin(); dataIter != newData.end(); ++dataIter)
	{
		std::wstring field = dataIter->first;
		std::wstring value = dataIter->second;
		// check if tile's data currently has a value
		// if not make a new one
		if( FindField(field) == 1 )
		{
			std::wstring curValue = GetData( field );
//			if(StringTools::IsDecimalNumber( value ))
//			{
				//binding on nonsensical string
				curValue = curValue + StringTools::ToStringW("|") + value;
				AddData(field, curValue);
//			}
		}
		else
		{
			AddData(field,value);
		}
	}

}

void TileModel::UpdateSequences(std::vector<SequenceLayerPtr> sequences)
{
 	for(uint i = 0; i < sequences.size(); i++)
 	{
		AddSequence(sequences[i]->GetSequenceController()->GetSequenceModel());
	}

}

void TileModel::CombineData()
{
	// for every field in data combine as user specified
	std::map<std::wstring,std::wstring> data = GetData();
	std::map<std::wstring,std::wstring>::iterator dataIter;
	for(dataIter = data.begin(); dataIter != data.end(); ++dataIter)
	{
		std::wstring field = dataIter->first;
		std::wstring value = dataIter->second;

		std::vector<std::wstring> seperatedValue;
		std::vector<double> values;
		double result;
		std::wstring resultConverted;
		boost::split(seperatedValue,value,boost::is_any_of("|"),boost::token_compress_on);

		// combination Methods that only handle numbers
		//		FIND A WAY TO EXCLUDE SEQUENCE ID!!!!!!!!!!
		if( ( StringTools::IsDecimalNumber( seperatedValue[0] ) )
			&& ( StringTools::ToLower(field).compare(_T("cell id")) != 0 )
			&& ( StringTools::ToLower(field).compare(_T("cellid")) != 0 )
			&& ( m_combinationMethod != GINI ) )
		{
			// convert string values to double values. Non numeric already parsed out
			for( uint i = 0; i < seperatedValue.size(); i++)
			{	
				values.push_back(StringTools::ToDouble(seperatedValue[i]));
			}	
			// MATH referenced from musiphil on Sep 30, 2011
			// http://stackoverflow.com/questions/7616511/calculate-mean-and-standard-deviation-from-a-vector-of-samples-in-c-using-boos
			if( m_combinationMethod == AVERAGE )
			{
				double mean =  std::accumulate(values.begin(), values.end(), 0.0) / values.size();
				result = mean;
			}
			else if(m_combinationMethod == STDEV )
			{		
				double mean =  std::accumulate(values.begin(), values.end(), 0.0) / values.size();
				double sq_sum = std::inner_product(values.begin(), values.end(), values.begin(), 0.0);
				double stdev = std::sqrt(sq_sum / values.size() - mean * mean);		
				result = stdev;
			}
			else if(m_combinationMethod == SUM )
			{			
				double sum = std::accumulate(values.begin(),values.end(),0);
				result = sum;
			}
			resultConverted = StringTools::ToStringW(result,2);
		}
		else
		{
			std::vector<std::wstring>::iterator it;
			it = std::unique( seperatedValue.begin(),seperatedValue.end() );

			seperatedValue.resize(std::distance(seperatedValue.begin(),it) );
			resultConverted = boost::algorithm::join( seperatedValue, "|" );

			//	resultConverted = value;
		}
		// GINI index can handle String fields
		if(m_combinationMethod == GINI )
		{		
			double giniSimpson = GiniSimpson( seperatedValue );
			result = giniSimpson;
			resultConverted = StringTools::ToStringW(result,2);
		}
		AddData( field, resultConverted);
	}
}

void TileModel::CombineSequenceData()
{
	std::map< std::wstring, std::vector<std::wstring> > master;

	for( uint i =0; i< GetNumSequence(); i++ )
	{
		std::map<std::wstring,std::wstring> data = GetSequence(i)->GetData();
		for( std::map<std::wstring,std::wstring>::iterator it = data.begin(); it != data.end(); ++it)
		{
			master[ (*it).first ].push_back(  (*it).second  );
		}
	}
	
	std::map<std::wstring,std::wstring> combinedData;
	// now combine all the sequences in the specified manner
	for( std::map<std::wstring,std::vector<std::wstring> >::iterator it = master.begin(); it != master.end(); ++it)
	{
		double result;
		std::wstring resultConverted;

		//convert any possible numeric
		std::vector<double> valueAsNum( (*it).second.size() );
		
		if( ( StringTools::IsDecimalNumber( (*it).second[0] ) )
		&& ( StringTools::ToLower((*it).first).compare(_T("sequence id")) != 0 )
		&& ( StringTools::ToLower((*it).first).compare(_T("sequenceid")) != 0 )
		&& ( StringTools::ToLower((*it).first).compare(_T("site id")) != 0 )
		&& ( StringTools::ToLower((*it).first).compare(_T("siteid")) != 0 )
		&& ( m_combinationMethod != GINI ) )
		{
			for( uint i = 0; i < (*it).second.size(); i++)
			{
				valueAsNum[i] = StringTools::ToDouble( (*it).second[i] );
			}
			
			if( m_combinationMethod == AVERAGE )
			{
				double mean =  std::accumulate(valueAsNum.begin(), valueAsNum.end(), 0.0) / valueAsNum.size();
				result = mean;
			}
			else if(m_combinationMethod == STDEV )
			{		
				double mean =  std::accumulate(valueAsNum.begin(), valueAsNum.end(), 0.0) / valueAsNum.size();
				double sq_sum = std::inner_product(valueAsNum.begin(), valueAsNum.end(), valueAsNum.begin(), 0.0);
				double stank = valueAsNum.size();
				double stdev = std::sqrt(abs(sq_sum / valueAsNum.size() - mean * mean));
				result = stdev;
			}
			else if(m_combinationMethod == SUM )
			{			
				double sum = std::accumulate(valueAsNum.begin(),valueAsNum.end(),0);
				result = sum;
			}
			resultConverted = StringTools::ToStringW( result, 4 );
		}
		else
		{
			resultConverted = boost::algorithm::join( (*it).second, " " );
		}

		if(m_combinationMethod == GINI )
		{	
			// GINI index can do it's own conversions as it will accept non-numeric
			double giniSimpson = GiniSimpson( (*it).second );
			result = giniSimpson;
			resultConverted = StringTools::ToStringW( result, 4 );
		}
		combinedData[(*it).first] = resultConverted;
	}

	SequenceModelPtr newSequence(new SequenceModel(GetSequence(0)->GetSequenceId(), GetSequence(0)->GetSiteId(),combinedData));
	RemoveAllSequence();
	AddSequence(newSequence);
}

double TileModel::GiniSimpson(std::vector<std::wstring> values)
{
	// dictionary containing unique values and their count
	std::map<std::wstring,int> sampleDict;
	int total=0;
	for( std::vector<std::wstring>::iterator it = values.begin(); it != values.end(); ++it)
	{
		if( sampleDict.find(*it) != sampleDict.end() )
			sampleDict[*it] += 1;
		else
			sampleDict[*it] = 1;
		total++;
	}
	// create "sample dict" as per OnCalculate method of AlphaDiversity.py

	double D = 0;
	for ( std::map<std::wstring,int>::iterator sampIt = sampleDict.begin(); sampIt != sampleDict.end(); ++sampIt )
	{
		int count = (*sampIt).second;
		double p = double(count)/total;

		D += p*p;		
	}
	double simpson = 1.0 - D;

	return simpson;

}