#=======================================================================
# Author: Alexander Keddy
#
# Copyright 2014 Alexander Keddy
#
# This file is part of GenGIS.
#
# GenGIS is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# GenGIS is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with GenGIS.  If not, see <http://www.gnu.org/licenses/>.
#=======================================================================


# IRIDA OUATH2 API TEST
from rauth import OAuth2Service
import json, ast, sys, GenGIS, wx, re, os, base64
from GBIFGeneric import GBIFGeneric
from IRIDAQueryLayout import IRIDAQueryLayout
from IRIDAQueryLayout import IRIDALoginLayout
from IRIDAQueryLayout import IRIDACredentialsLayout
from IRIDAQueryLayout import IRIDAClientLayout
from wx.lib.pubsub import pub
from urllib3.contrib import pyopenssl
pyopenssl.inject_into_urllib3()

class Credential:
	def __init__(self, alias="", ID="",secret="",active=False):
		self.alias = alias
		self.ID = ID
		self.secret = secret
		self.active = active
	
	def GetAlias(self):
		return self.alias
	
	def GetID(self):
		return self.ID
		
	def GetSecret(self):
		return self.secret
		
	def GetActive(self):
		return self.active
		
	def SetAlias(self,alias):
		self.alias = alias
		
	def SetID(self, ID):
		self.ID = ID
		
	def SetSecret(self, secret):
		self.secret = secret
		
	#allows the activity of a credential to be changed, by default makes it false
	def SetActive(self, active = False):
		self.active = active
		
	def ToString(self):
		return "%s\n%s\n%s\n" %(self.alias, self.ID, self.secret) 
		

class Client(IRIDAClientLayout):
	def __init__(self,Parent = None):
		IRIDAClientLayout.__init__(self,Parent)
		self.parent = Parent
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico",wx.BITMAP_TYPE_ICO))
		self.index=-1
	
	def SetAlias(self,alias):
		self.m_AliasCtrl.SetValue(alias)
		
	def SetID(self,ID):
		self.m_IDCtrl.SetValue(ID)
		
	def SetSecret(self,secret):
		self.m_SecretCtrl.SetValue(secret)
	
	def SetIndex(self,index):
		self.index = index
		
	def GetAlias(self):
		return self.m_AliasCtrl.GetValue()
		
	def GetID(self):
		return self.m_IDCtrl.GetValue()
		
	def GetSecret(self):
		return self.m_SecretCtrl.GetValue()
		
	def GetIndex(self):
		return self.index
		
	def OnOK(self,event):
		self.parent.CloseClient( self.GetIndex(),self.GetAlias(), self.GetID(), self.GetSecret())
		self.Close()
	
class Credentials(IRIDACredentialsLayout):
	
	def __init__(self, IRIDA, withUI=True):
		IRIDACredentialsLayout.__init__(self,None)
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico",wx.BITMAP_TYPE_ICO))
		self.credentList = []
		self.ActiveCredential = ""
		self.OnLoadCredentials()
		self.parent = IRIDA
		if withUI:
			self.PopulateAliasList()
	
	#	When client is closed this function adds the new set of credentials to the existing set.
	def CloseClient(self, index, alias, id, secret):
		#if alias not == "" and id not == "" and secret not == "":
		if alias and id and secret:
			if index > -1:
				self.AddClient([alias,id,secret,False],index)
			else:
				self.AddClient([alias,id,secret,False])
			self.PopulateAliasList(index)
			
		return None
	
	def OnAdd(self,event):
		client = Client(self)
		client.Show()
		client.SetAlias("")
		client.SetID( "" )
		client.SetSecret( "" )

		return None
	
	def OnEdit(self,event):
		client = Client(self)
	#	alias = self.m_AliasList.GetStringSelection()
		index = self.m_AliasList.GetSelection()
		
		#if the index exists in self.credentList
		if index < len(self.credentList):
			cred = self.credentList[index]
		else:
			return None
		client.Show()
		client.SetAlias( cred.GetAlias() )
		client.SetID( cred.GetID() )
		client.SetSecret( cred.GetSecret() )
		client.SetIndex( index )

		return None
		
	def OnDelete(self,event):
	#	alias = self.m_AliasList.GetStringSelection()
		index = self.m_AliasList.GetSelection()
		if index < len(self.credentList):
			self.credentList.pop(index)
		self.PopulateAliasList()
		return None
	
	def OnCredentialClose(self,event):
		print self.ActiveCredential
		self.OnSaveCredentials()
		if self.ActiveCredential:
			#self.OnSaveCredentials()
			self.parent.OnCredentialClose(self.ActiveCredential)
		self.Close()
	
	def OnLoadCredentials(self):
		#THIS IS PROBABLY OS DEPENDENT
		 # Get file path
		dir = os.path.realpath(__file__).split('\\')
		dir.pop()
		path = '\\'.join(dir)
		path = os.path.join(path,'credentials.secrets')
		secFile = ""
	#	credMap = []
		if os.path.isfile(path):
			secFile = open(path,"r")
			decrypt = self.DecryptCredentials(secFile.read())
			#break up all credentials
			raw = decrypt.split("#")
			for i in range(0,len(raw)):
				cred = raw[i].split('\n')
				#get rid of trailing endline
				cred.pop()
				if len(cred) == 3:
					#Adds raw cred to credentList and converts it to type Credential
					cred = self.AddClient(cred)
					#Set active credential to be used by the plugin
					if self.credentList.index(cred) == 0:
						self.ActiveCredential = cred
				else:
					print "Credential is:"
					print cred
			secFile.close()
		else:
			secFile = open(path,"w")
			decrypt = ""
			secFile.close()
			return None
		
	def OnSaveCredentials(self):
		dir = os.path.realpath(__file__).split('\\')
		dir.pop()
		path = '\\'.join(dir)
		path = os.path.join(path,'credentials.secrets')
		if os.path.isfile(path):
			try:
				secFile = open(path,"w")
				cred = self.ToText(self.credentList)
				encrypt = self.EncryptCredentials(cred)
				secFile.write(encrypt)
				secFile.close()
				return 1
			except IOError:
				print "Credential Save Error"
				return None
		return None
	
	def ToText(self,text):
		res = ""
		for i in text:
			res += '#'+i.ToString()

		return res
		
	def EncryptCredentials(self,file):
		encrypted = base64.b64encode(file)
		return encrypted
		
	def DecryptCredentials(self,file):
		decrypted = base64.b64decode(file)
		return decrypted

	def PopulateAliasList(self, index=-1):
		self.m_AliasList.Clear()
		for cred in self.credentList:

			self.m_AliasList.InsertItems( [cred.GetAlias()], self.credentList.index(cred) )
	
	def AddClient(self,cred,index=-1):
		credential = Credential(cred[0],cred[1],cred[2])
		if index > -1:
			self.credentList[index] = credential
		else:
			self.credentList.append( credential )
		return credential
		
	def OnActive(self,event):
		print self.credentList,"\n"
		#Get selected from list
		index = self.m_AliasList.GetSelection()
		if index < len(self.credentList):
			cred = self.credentList[index]
		else:
			return None
		#Swap in credentList with current active
		self.ActiveCredential = cred
		current = self.credentList[0]
		self.credentList[0] = cred
		self.credentList[index] = current
		#Swap in m_AliasList with current active
		self.m_AliasList.SetString( 0, cred.GetAlias())
		self.m_AliasList.SetString( index, current.GetAlias())
		print self.credentList,"\n"
		
		return None
		
class Login(IRIDALoginLayout):
	def __init__(self, IRIDA):
		IRIDALoginLayout.__init__(self,None)
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico",wx.BITMAP_TYPE_ICO))
		self.interface = IRIDA
		self.success = False
	
	#Enters User Credentials
	def OnOK(self,event):
		wx.BeginBusyCursor()
		self.interface.username = self.m_UsernameCtrl.GetValue()
		self.interface.password = self.m_PasswordCtrl.GetValue()
		self.success=self.interface.AuthorizeToken()
		if self.success == -1:
			#deal with the fact that credentials have not been saved previously
			wx.EndBusyCursor()
			self.Close()
		elif self.success:
			self.interface.m_Search.Enable(True)
			self.interface.m_Calc.Enable(True)
		#	self.interface.InitializeResults()
			self.interface.InitializeAnalyses()
			wx.EndBusyCursor()
		#	self.interface.Focus()
		#	self.interface.Raise()
			self.Close()
		else:
			wx.EndBusyCursor()
		
	def OnClose(self, event):
		event.Skip()
	#	if not self.success:
	#		self.interface.Close()

	
class IRIDA(IRIDAQueryLayout):
	#baseURL =  "https://irida.corefacility.ca/irida-public-test/irida-api/"
	#baseURL =  "https://irida.corefacility.ca/staging/api/"
	#baseURL =  "http://localhost:48888/irida/api/"
	baseURL =  "http://129.173.66.114:48888/irida/api/"
	
	#clientId = "gengis"
	clientId = ""
	# THIS NEEDS TO BE HIDDEN
	clientSecret = ""
	#clientSecret = "bledjijitFeOpEerichFeatsatHowGheibwenecWijWifciek/"
	username = ""
	password = ""
	service = ""
	token = ""
	selectedTaxon = set()
	# this will be a 1:1 mapping of Project Name to Unique ID
	projectMap = {}
	obs = []
	phylo = ""
	
	def __init__(self,parent=None):
		IRIDAQueryLayout.__init__(self,parent)
		self.GBIFGeneric = GBIFGeneric()
		self.login = Login(self)
		#self.credentials = Credentials()
		self.SetIcon(wx.Icon(GenGIS.mainWindow.GetExeDir() + "images/CrazyEye.ico",wx.BITMAP_TYPE_ICO))
		self.m_Add.SetBitmapLabel(wx.Image(GenGIS.mainWindow.GetExeDir() + "images/green_arrow_down.png",wx.BITMAP_TYPE_PNG).ConvertToBitmap())
		self.m_Add.SetBitmapHover(wx.Image(GenGIS.mainWindow.GetExeDir() + "images/green_arrow_down_hover.png",wx.BITMAP_TYPE_PNG).ConvertToBitmap())
		self.m_Remove.SetBitmapLabel(wx.Image(GenGIS.mainWindow.GetExeDir() + "images/red_arrow_up.png",wx.BITMAP_TYPE_PNG).ConvertToBitmap())
		self.m_Remove.SetBitmapHover(wx.Image(GenGIS.mainWindow.GetExeDir() + "images/red_arrow_up_hover.png",wx.BITMAP_TYPE_PNG).ConvertToBitmap())
		
		self.m_IDList.Clear()
		self.m_Search.Enable(False)
		self.m_Calc.Enable(False)
		#fix to expand summary box enough to print two lines of text properly
		
		#Map Data
		if GenGIS.layerTree.GetNumMapLayers() > 0 :
			self.m_AddData.Enable()
			borders = GenGIS.layerTree.GetMapLayer(0).GetController().GetMapBorders()
			#check if geographic coordinates are used or some other measure; only geographic are compatible
			geographic = GenGIS.StudyController.IsUsingGeographic(GenGIS.study.GetController())
			projected = GenGIS.StudyController.IsUsingProjection(GenGIS.study.GetController())
			if(not (geographic or projected)):
				wx.MessageBox("Geographic coordinates are not being used in the current map file. Only geographic coordinates are compatible with MG-RAST. Geographic range will need to be manually set, and any returned data will not display correctly.","Warning")
				self.m_AddData.Disable()
		credential = Credentials(self,False)
		credential.OnCredentialClose("fake")
		
		# By this point the active credential should be loaded.
		if self.clientId and self.clientSecret:
			self.login.ShowModal()
	
	def ReLogin(self, event):
		self.login.ShowModal()
		# Bring the main IRIDA instance back to the foreground after running login.
		self.Raise()
	
	def SetClientID(self,ID):
		self.clientId = ID
		
	def SetClientSecret(self,secret):
		self.clientSecret = secret
	
	def HandleLinks(self, result, type):
		response = {}
		if type == 'label':
			response['label'] = '';
			
		links = result[type]
		for val in links:
			if type=='links':
				response[val['rel']] = val['href']
			elif type == 'label':
				response['label'] = response['label'] + val
			else:
				if 'name' in val:
					response[val['name']] = val
				elif 'sampleName' in val:
					response[val['sampleName']] = val
				
		return response
		
	def ngs_request(self, url, oauth_service, access_token,debug=False):
		session = oauth_service.get_session(access_token)
		# the raw json being fetched
		response = session.get(url)
	#	print response.text
		if debug:
			print response.text
		result = json.loads(response.text)
		return result
	
	def newick_request( self, url, oauth_service, access_token, debug = False):
		session = oauth_service.get_session(access_token)
		# the raw newick being fetched
		response = session.request("GET",url, True,headers={'Accept':'text/plain'})
		if debug:
			print response.text
		return response.text
	
	def decoder(self,returnDict):
	# NEEDS A WAY TO DETECT BAD PASSWORD
	#	print returnDict
	#	if "Error report" in returnDict:
	#		print "error occured"
	#		return {}
		iridaDict = ast.literal_eval(returnDict)

		if "error" in iridaDict.keys():
			wx.MessageBox(iridaDict['error_description'], iridaDict['error'])
			return None
		return iridaDict

	def GetOauthService(self):
		oauth_service = OAuth2Service(
		client_id = self.clientId,
		client_secret = self.clientSecret,
		name="irida",
		access_token_url = self.baseURL + "oauth/token",
		base_url=self.baseURL)
		return oauth_service

	def GetParams(self):
		params = {'data' : {'grant_type' : 'password',
			'client_id' : self.clientId,
			'client_secret' : self.clientSecret,
			'username' : self.username,
			'password' : self.password}}
		return params
		
	def GetToken(self):
		oauth_service = self.GetOauthService()
		params = self.GetParams()
	
		try:
		#	print "sandwich"
			access_token = oauth_service.get_access_token(decoder = self.decoder,**params)
		#	print "banana"
			
		except:
			return None, None
		session = oauth_service.get_session(access_token)
		return oauth_service, access_token

	# Type is either RESOURCES or LINKS
	def GetResponse(self, url, oauth_service, access_token, type, debug=False):
		result = self.ngs_request(url, oauth_service, access_token, debug)
		if debug:
			print "#########\n\n",result,"\n\n#########"
		response = {}
		try:
			# parse out the response from the server
			response = self.HandleLinks(result['resource'],type)
		except ValueError:
			print "Server replied but reponse is in an unsupported format."
			sys.exit(1)
		return response
	
	# Formates a lat/lon into one string with 2 decimal places
	def FormatLocation(self, value):
		lat = value['latitude']
		lon = value['longitude']
		if lat != None and lon != None:
			loc = '%.2f_%.2f'%(float(lat),float(lon))
			return lon,lat,loc
		else:
			loc = '0_0'
			return 0,0,loc
			
	#Given All samples create a set of unique locations and the sequences that belong to them
	#Returns a dictionary of this information
	def CreateLocations(self, samples):
		idNum = 1
		idString = 'Loc_'
		locations = {}
		for sample in samples:
			for key, value in sample.items():
				lon,lat,loc = self.FormatLocation(value)
				id = idString + '%0*d' %(3,idNum)
				idNum += 1
				if loc in locations:
					locations[loc].append(key)
				else:
					locations[loc] = [id, lon, lat, key]
		return locations
		
	def CreateOutText(self, locations, samples):
		locationText = ""
		sequenceText = ""
		locationHeader = "Site ID, Longitude, Latitude\n"
		sequencesHeader = ["Sequence ID, Site ID "]
		first = True
		#first create locations
		for key, value in locations.items():
			locationText = '%s%s, %s, %s\n' %( locationText, value[0], value[1], value[2] ) 
		locationText = locationHeader + locationText
		
		#then create sequences
		for sample in samples:
			for key, values in sample.items():
				seq = []			
				#Append Sequence ID
				seq.append(key)
				#Append Site ID
			#	seq.append('Seq_%0*d' %(3, idNum) )
				lon,lat,loc = self.FormatLocation(values)
				seq.append(locations[loc][0])
				for i,j in values.items():
					if i == 'links':
						continue
					if first:
						sequencesHeader.append(str(i))
					seq.append(str(j).replace(',','|'))
				first = False
				text = ','.join(seq)
				sequenceText = sequenceText + text + '\n'
		
		sequenceHeader = ','.join(sequencesHeader)
		sequenceText = sequenceHeader + '\n' + sequenceText
		return locationText, sequenceText
		
	def OnHelp(self, event):
		wx.LaunchDefaultBrowser( 'http://kiwi.cs.dal.ca/GenGIS/index.php/Description_of_GenGIS_plugins#MG-RAST_Query' )
	#	Close the Plugin
	def OnClose(self, event):
		if self.login:
			self.login.Close()
		event.Skip()
	
	#	Close the Plugin
	def OnOK( self, event ):
		self.Close()

	#	Add Data from Results Table to ID List
	def OnAdd(self,event):
		i=0
		IDCount = self.m_IDList.GetCount()
		for index in self.m_Result.GetSelections():
			selected = self.m_Result.GetString(index)
			split = selected.split(": ")
			if (split[0],split[1]) not in self.selectedTaxon:
				self.m_IDList.InsertItems(["%s" % selected],IDCount+i)
				#add study ID and study location
				self.selectedTaxon.add((split[0],split[1]))
				i+=1
			
	#	Remove Data from ID List
	def OnRemove(self,event):
		candidates = sorted(self.m_IDList.GetSelections(),reverse=True)
		for index in candidates:
			selected = self.m_IDList.GetString(index)
			split = selected.split(": ")
			self.selectedTaxon.remove((split[0],split[1]))
			self.m_IDList.Delete(index)
	
	# Retrieves token from IRIDA API. 
	def AuthorizeToken(self):
		if self.username == "" or self.password == "":
			wx.MessageBox("Please enter a username and/or password.")
			return 0
		if self.clientId == "" or self.clientSecret == "":
			wx.MessageBox("No Client Credentials have been loaded.")
			return -1
		try:
			self.service, self.token = self.GetToken()
			if self.service == None or self.token == None:
				wx.MessageBox("Bad username and/or password.")
				return 0
		except ValueError:
			wx.MessageBox("Unable to retrieve token from API at this time. Please try again later.")
			return 0
		return 1

	def InitializeResults(self):
		wx.BeginBusyCursor()
		result = []
		links = self.GetResponse(self.baseURL+'projects/', self.service, self.token,'resources')
		if len(links.keys())==0:
			wx.MessageBox("No Projects currently stored in IRIDA")
		else:
			for key,value in links.items():
				str = "%s: %s" %(key,value['projectDescription'])
				# add mapping for this project
				self.projectMap[key] = value['identifier']
				result.append(str)
			self.m_Result.InsertItems(result,self.m_Result.GetCount())
		wx.EndBusyCursor()
	
	def InitializeAnalyses(self):
		wx.BeginBusyCursor()
		result = []
		links = self.GetResponse(self.baseURL+'analysisSubmissions/', self.service, self.token,'resources')
		if len(links.keys())==0:
			wx.MessageBox("No analyses currently stored in IRIDA")
		else:
			for key,value in links.items():
				str = "%s: %s" %(key,value['label'])
				# add mapping for this project
				self.projectMap[key] = value['identifier']
				result.append(str)
			self.m_Result.InsertItems(result,self.m_Result.GetCount())
		wx.EndBusyCursor()
	
	def OnSearch(self,event):
		wx.BeginBusyCursor()
		#	Clear the results list
		projName = self.m_ProjectName.GetValue()
		if projName not in self.projectMap.keys():
			wx.EndBusyCursor()
			wx.MessageBox("Please enter a valid Project name.")
			return

		project = projName
		if(len(project)==0):
			wx.MessageBox("You did not enter a taxon name.")
		else:
			self.selectedTaxon.clear()
			self.selectedTaxon.add((project,"dummy value"))
			self.OnCalculate(wx.EVT_BUTTON)

		wx.EndBusyCursor()
	
	def OnCalculate( self, event):
		self.RetrieveSamples()
	#	self.RetrievePhylogeny()
		
#	def RetrieveSamples(self,event):
	def RetrieveSamples(self):
		if(self.selectedTaxon):
			#index of which taxon is being used
			index = 0
			for proj in self.selectedTaxon:
				self.m_Progress.WriteText("Retrieving %s...\n" %(proj[0]))
				project = self.projectMap[proj[0]]
			#	projectURL = '%sprojects/%s' %(self.baseURL,project)
				projectURL = '%sprojects/%s' %(self.baseURL,3)
				self.phylo = self.RetrieveKnownPhylogeny(project)
			#	print newick
				projLink = self.GetResponse(projectURL, self.service, self.token,'links')
				sampleURL = projLink['project/samples']
				samples = self.GetResponse(sampleURL, self.service, self.token, 'resources')
	
				# obs is just a list of these samples
				self.obs.append(samples)
				self.m_Progress.WriteText("%s samples retrieved.\n" %(len(samples.keys())))
				self.m_Progress.WriteText("Done.\n")
			
#	def RetrievePhylogeny(self,event):
	def RetrievePhylogeny(self):
		if(self.selectedTaxon):
			#index of which taxon is being used
			index = 0
			for proj in self.selectedTaxon:
				self.m_Progress.WriteText("Retrieving %s...\n" %(proj[0]))
				project = self.projectMap[proj[0]]
				projectURL = '%sanalysisSubmissions/%s' %(self.baseURL,project)
				print projectURL
				projLink = self.GetResponse(projectURL, self.service, self.token,'links')
				analysisURL = projLink['analysis']
				print analysisURL
				# get samples
				'''
				sequenceURL = analysisURL + "/sequenceFiles/pairs"
				print "pants"
				sequenceLink = self.GetResponse(projectURL, self.service, self.token, 'response',True)
				print "sandwich"
				'''
				
				# get phylogeny
				links = self.GetResponse(analysisURL, self.service, self.token, 'links')
				newickURL = links['outputFile/tree']
				newick = self.newick_request(newickURL, self.service,self.token,True)
	
				# obs is just a list of these samples
		#		self.obs.append(samples)
		
				self.m_Progress.WriteText("%s samples retrieved.\n" %(len(samples.keys())))
				self.m_Progress.WriteText("Done.\n")
		
	def RetrieveKnownPhylogeny(self, identifier):
		projectURL = '%sanalysisSubmissions/%s' %(self.baseURL,identifier)
		projLink = self.GetResponse(projectURL, self.service, self.token,'links')
		analysisURL = projLink['analysis']
		
		# get phylogeny
		links = self.GetResponse(analysisURL, self.service, self.token, 'links')
		newickURL = links['outputFile/tree']
		newick = self.newick_request(newickURL, self.service,self.token)
		return newick

	
	def OnAddData(self, event):
		if GenGIS.layerTree.GetNumMapLayers() == 0:
			wx.MessageBox("This action requires a Map layer. Please load a Map.")
			return
		if len(self.obs) > 0:
			loc = self.CreateLocations(self.obs)
			locText, seqText = self.CreateOutText(loc,self.obs)
			locArray = locText.split('\n')
			seqArray = seqText.split('\n')
			locArray.pop()
			seqArray.pop()
			layerName = "IRIDALayer_%d" % GenGIS.layerTree.GetNumLocationLayers()
			GenGIS.mainWindow.OpenLocationsCSVFile(locArray, layerName)
			GenGIS.mainWindow.OpenSequenceCSVFile(seqArray, layerName)
			print self.phylo
			self.phylo = self.phylo.replace("reference:0.00000001,","")
			print self.phylo
			fh = "phylogeny.nwk"
			file = open("phylogeny.nwk",'w')
			file.write(self.phylo)
			file.close()
			GenGIS.mainWindow.OpenTreeFile(fh)
		else:
			wx.MessageBox("Please make a successful IRIDA query first.")
			
	def OnExportData(self,event):
		if (len(self.obs) > 0):
			fileTypes = 'Loc and Seq Files (*.csv)|*.csv'
			dlg = wx.FileDialog(self, "Save plot", "", "", fileTypes, wx.SAVE)
			if dlg.ShowModal()==wx.ID_OK:
				filename =	dlg.GetFilename()
				dir = dlg.GetDirectory()
				file_split = filename.split(".",1)
				OUTLfile = os.path.join(dir,file_split[0]+"_locs.csv")		
				OUTSfile = os.path.join(dir,file_split[0]+"_seqs.csv")
				loc = self.CreateLocations(self.obs)
				locText, seqText = self.CreateOutText(loc,self.obs)
				self.GBIFGeneric.WRITEEXPORT(OUTLfile , locText )
				self.GBIFGeneric.WRITEEXPORT(OUTSfile, seqText )
				
	def SetToken(self,tok):
		self.token = tok
	
	def OnCredentials(self,event):
		credentials = Credentials(self)
		credentials.Show()

	def OnCredentialClose(self,active):
	#	self.credential = active
		self.clientId = active.GetID()
		self.clientSecret = active.GetSecret()
		