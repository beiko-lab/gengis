//=======================================================================
// Author: Donovan Parks
//
// Copyright 2009 Donovan Parks
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
#ifndef _GENGIS_PROGRESS_DLG_
#define _GENGIS_PROGRESS_DLG_

#include "../core/Precompiled.hpp"

namespace GenGIS
{

	/**
	* @brief Progress dialog.
	*/
	class ProgressDlg 
	{
	public:
		/** Constructor. */
		explicit ProgressDlg( const wxString& title, const wxString& message, uint maximum = 100, wxWindow* parent = 0 ) : m_progress( 0 )
		{
			m_progressDlg = new wxProgressDialog( title , message, maximum, parent, wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_SMOOTH | wxPD_ELAPSED_TIME | wxPD_AUTO_HIDE );
			m_progressDlg->SetFocus();
		}


		/** Destructor. */
		virtual ~ProgressDlg()
		{
			m_progressDlg->Destroy();
		}

		/** Update progress dialog. */
		bool Update( uint value, const wxString& message = wxEmptyString )
		{
			m_progress = value;
			return m_progressDlg->Update( value, message );
		}

		/** Increment progress dialog count by 1. */
		bool Increment()
		{
			m_progress++;
			return m_progressDlg->Update( m_progress, wxEmptyString );
		}

	private:
		/** Progress dialog. */
		wxProgressDialog* m_progressDlg;

		/** Track progress. */
		uint m_progress;
	};

}

#endif
