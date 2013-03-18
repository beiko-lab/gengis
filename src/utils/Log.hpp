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

#ifndef _GENGIS_LOG_
#define _GENGIS_LOG_

#include "../core/Precompiled.hpp"

namespace GenGIS
{

	/**
	* @brief Singleton class for writing general information, warnings, and errors 
	*        to both the application console and to file.
	*/
	class Log
	{
	public:
		/** Destructor */
		~Log()
		{
			foreach(const std::string& str, m_logData)
			{
				m_log << str << std::endl;
			}

			m_log.close();
		}

		/** Get the one and only instance of this class.. */
		static Log& Inst()
		{
			static Log log;
			return log;
		}

		/** Add text to log file. */
		void Add(const char* text)
		{
			m_logData.push_back(text);
		}

		/** Add text to log file. */
		void Add(const std::string& text)
		{
			m_logData.push_back(text);
		}

		/** Add text to log file. */
		void Write(const char* text)
		{
			m_log << text << std::endl;

			if(m_console != NULL)
			{
				m_console->AppendText(wxString(text, wxConvUTF8) + wxT("\n"));

				wxTextPos pos = m_console->GetLastPosition();
				m_console->ShowPosition(pos);
			}
		}

		/** Add text to log file. */
		void Write(const std::string& text)
		{
			Write(text.c_str());
		}

		/** Add text to log file. */
		void Write(const wxString& string)
		{
			Write(std::string(string.mb_str()));
		}

		/** Add text to log file. */
		void SilentWrite(const char* text)
		{
			m_log << text << std::endl;
		}

		/** Add text to log file. */
		void SilentWrite(const std::string& text)
		{
			SilentWrite(text.c_str());
		}

		/** Add warning to log file. */
		void Warning(const char* text)
		{
			m_log << text << std::endl;

			if(m_console != NULL)
			{
				m_console->BeginTextColour(wxColour(128, 0, 0));
				m_console->BeginBold();
				m_console->AppendText(wxString(text, wxConvUTF8) + wxT("\n"));
				m_console->EndBold();
				m_console->EndTextColour();

				wxTextPos pos = m_console->GetLastPosition();
				m_console->ShowPosition(pos);
			}
		}

		/** Add warning to log file. */
		void Warning(const std::string& text)
		{
			Warning(text.c_str());
		}

		/** Add warning to log file. */
		void Warning(const wxString& string)
		{
			Warning(std::string(string.mb_str()));
		}

		/** Add warning to log file. */
		void Warning(const wchar_t* text)
		{
			m_log << text << std::endl;

			if(m_console != NULL)
			{
				m_console->BeginTextColour(wxColour(128, 0, 0));
				m_console->BeginBold();
				m_console->AppendText(wxString(text) + wxT("\n"));
				m_console->EndBold();
				m_console->EndTextColour();

				wxTextPos pos = m_console->GetLastPosition();
				m_console->ShowPosition(pos);
			}
		}

		/** Add warning to log file. */
		void Warning(const std::wstring& text)
		{
			Warning(text.c_str());
		}

		/** Add error to log file. */
		void Error(const char* text)
		{
			m_log << text << std::endl;

			if(m_console != NULL)
			{
				m_console->BeginTextColour(wxColour(255, 0, 0));
				m_console->BeginBold();
				m_console->AppendText(wxString(text, wxConvUTF8) + wxT("\n"));
				m_console->EndBold();
				m_console->EndTextColour();

				wxTextPos pos = m_console->GetLastPosition();
				m_console->ShowPosition(pos);
			}
		}

		/** Add error to log file. */
		void Error(const std::string& text)
		{
			Error(text.c_str());
		}

		/** Add error to log file. */
		void Error(const wchar_t* text)
		{
			m_log << text << std::endl;

			if(m_console != NULL)
			{
				m_console->BeginTextColour(wxColour(255, 0, 0));
				m_console->BeginBold();
				m_console->AppendText(wxString(text) + wxT("\n"));
				m_console->EndBold();
				m_console->EndTextColour();

				wxTextPos pos = m_console->GetLastPosition();
				m_console->ShowPosition(pos);
			}
		}

		/** Add error to log file. */
		void Error(const std::wstring& text)
		{
			Error(text.c_str());
		}

		/** Add error to log file. */
		void Error(const wxString& string)
		{
			Error(std::string(string.mb_str()));
		}

		/** Set output console. */
		void SetConsole(wxRichTextCtrl* console) { m_console = console; }

	protected:
		/** Private singleton constructor (must instantiate this class through Inst()). */
		Log();

		/** Private copy constructor (must instantiate this class through Inst()). */
		Log(const Log&);

		/** Private assignment constructor (must instantiate this class through Inst()). */
		Log& operator= (const Log&);


	private:
		/** Log stream */
		std::ofstream m_log;

		/** Vector of log data. */
		std::vector<std::string> m_logData;

		/** Console to output messages to. */
		wxRichTextCtrl* m_console;
	};

}

#endif
