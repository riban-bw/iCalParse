/***************************************************************
 * Name:      iCalMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Brian Walton (brian@riban.co.uk)
 * Created:   2015-11-14
 * Copyright: Brian Walton (riban.co.uk)
 * License:
 **************************************************************/

#include "iCalMain.h"
#include "recurrule.h"
#include <wx/msgdlg.h>
#include <wx/filedlg.h>
#include <wx/textfile.h>
#include <wx/log.h>

//(*InternalHeaders(iCalFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(iCalFrame)
const long iCalFrame::idLstLog = wxNewId();
const long iCalFrame::idMenuOpen = wxNewId();
const long iCalFrame::idMenuQuit = wxNewId();
const long iCalFrame::idMenuAbout = wxNewId();
const long iCalFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(iCalFrame,wxFrame)
    //(*EventTable(iCalFrame)
    //*)
END_EVENT_TABLE()

iCalFrame::iCalFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(iCalFrame)
    wxMenuItem* MenuItem2;
    wxMenu* Menu1;
    wxMenuItem* m_pMenuQuit;
    wxMenuBar* MenuBar1;
    wxMenu* Menu2;

    Create(parent, id, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("id"));
    m_pLstLog = new wxListBox(this, idLstLog, wxPoint(152,144), wxDefaultSize, 0, 0, 0, wxDefaultValidator, _T("idLstLog"));
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    m_pMenuOpen = new wxMenuItem(Menu1, idMenuOpen, _("&Open\tCtrl-o"), _("Open iCal file"), wxITEM_NORMAL);
    Menu1->Append(m_pMenuOpen);
    m_pMenuQuit = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(m_pMenuQuit);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);

    Connect(idMenuOpen,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&iCalFrame::OnMenuOpenSelected);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&iCalFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&iCalFrame::OnAbout);
    //*)
}

iCalFrame::~iCalFrame()
{
    //(*Destroy(iCalFrame)
    //*)
}

void iCalFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void iCalFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void iCalFrame::OnMenuOpenSelected(wxCommandEvent& event)
{
    wxFileDialog dlg(this, "Open file", "", "", "iCal files (*.ics)|*.ics", wxFD_OPEN|wxFD_FILE_MUST_EXIST);
    if(dlg.ShowModal() == wxID_CANCEL)
        return;
    wxTextFile icsFile(dlg.GetPath());
    if(!icsFile.Open())
    {
        wxMessageBox("Failed to open file");
        return;
    }
    size_t nLineCount = icsFile.GetLineCount();
    size_t nEventCount = 0;
    m_pLstLog->AppendAndEnsureVisible(wxString::Format("Opened file with %d lines", nLineCount));

    //!@todo Move parsing to separate function?
    //Find start of calendar block
    wxString sLine = icsFile.GetFirstLine();
    while(!icsFile.Eof() && sLine != "BEGIN:VCALENDAR")
          sLine = icsFile.GetNextLine();
    if(icsFile.Eof())
    {
        wxMessageBox("Failed to find calendar block in file");
        return;
    }
    m_pLstLog->AppendAndEnsureVisible("Found calendar section");

    //Parse each line of file until end of calendar block
    for(; !icsFile.Eof(); sLine = icsFile.GetNextLine())
    {
        if(sLine == "END:VCALENDAR")
        {
            //Found end of calendar block. Need to do something with it...
            m_pLstLog->AppendAndEnsureVisible("End of calendar section");
            return;
        }
        if(sLine.Left(8) == "VERSION:" && sLine != "VERSION:2.0")
		{
			//!@todo Support iCalendar versions other than 2.0
            m_pLstLog->AppendAndEnsureVisible(wxString::Format("Found calendar version: %s.\nOnly support version 2.0", sLine.Mid(8)));
			return;
		}
        if(sLine == "BEGIN:VEVENT")
        {
            //Found a calendar event. Parse each line of the event
            EVENT_PROPERTY nLastProperty = EVENT_NONE;
            wxString sSummary;
            wxString sLocation;
            wxString sDescription;
            wxString sUid;
            wxDateTime dtStart((time_t)0);
            wxDateTime dtEnd((time_t)0);
            bool bSuccess = true; //true if event parsing succeeds
            RecurRule* pRecur = NULL;
            while (!icsFile.Eof() && sLine != "END:VEVENT")
            {
                sLine = icsFile.GetNextLine();
                if(sLine.Left(5) == "RRULE" && !pRecur)
                {
                    //First recur rule (there should only be one)
                    pRecur = new RecurRule(sLine.AfterFirst(':'));
                }
                if(sLine.Left(5) == "BEGIN")
                {
                    while(!icsFile.Eof() && sLine.Left(3) != "END")
                    {
                        sLine = icsFile.GetNextLine();
                        //!@todo Handle subevent
                    }
                }
                if(sLine.Left(1) == " " || sLine.Left(1) == "\t")
                {
                    //Line continuation
                    switch(nLastProperty)
                    {
                        case EVENT_DESCRIPTION:
                            sDescription += sLine.Mid(2);
                            break;
                        case EVENT_LOCATION:
                            sLocation += sLine.Mid(2);
                            break;
                        case EVENT_SUMMARY:
                            sSummary += sLine.Mid(2);
                            break;
                        case EVENT_UID:
                            sUid += sLine.Mid(2);
                            break;
                    }
                }
                else if(sLine.Left(7) == "SUMMARY")
                {
                    sSummary = sLine.AfterFirst(':');
                    nLastProperty = EVENT_SUMMARY;
                    //!@todo handle long lines
                }
                else if(sLine.Left(8) == "LOCATION")
                {
                    sLocation = sLine.AfterFirst(':');
                    nLastProperty = EVENT_LOCATION;
                }
                else if(sLine.Left(11) == "DESCRIPTION")
                {
                    sDescription = sLine.AfterFirst(':');
                    nLastProperty = EVENT_DESCRIPTION;
                }
                else if(sLine.Left(3) == "UID")
                {
                    sUid = sLine.AfterFirst(':');
                    nLastProperty = EVENT_UID;
                }
                else if(sLine.Left(7) == "DTSTART")
                {
                    //!@todo Handle paramters: Value=Date|Date-Time
                    wxString sParam = sLine.AfterFirst(';').BeforeFirst(':');
                    if(sParam == "VALUE=DATE")
                        bSuccess &= dtStart.ParseFormat(sLine.AfterFirst(':'), "%Y%m%d");
//                    else if(sParam == "VALUE=DATE-TIME")
                    else
                        bSuccess &= dtStart.ParseFormat(sLine.AfterFirst(':'), "%Y%m%dT%H%M%S");
                        nLastProperty = EVENT_START;
                }
                else if(sLine.Left(5) == "DTEND")
                {
                    wxString sParam = sLine.AfterFirst(';').BeforeFirst(':');
                    if(sParam == "VALUE=DATE")
                        bSuccess &= dtEnd.ParseFormat(sLine.AfterFirst(':'), "%Y%m%d");
                    else
                        bSuccess &= dtEnd.ParseFormat(sLine.AfterFirst(':'), "%Y%m%dT%H%M%S");
                        nLastProperty = EVENT_END;
                }
                else
                {
                    //Unhandled parameter
                    nLastProperty = EVENT_NONE;
                }
            }
            if(bSuccess)
            {
                wxString sResult = wxString::Format("%d: %s - %s\n\t%s\n\tLocation: %s\n\tDescription:%s\n\tUID: %s", ++nEventCount, dtStart.FormatISOCombined(), dtEnd.FormatISOCombined(), sSummary, sLocation, sDescription, sUid);
                if(pRecur && pRecur->IsValid())
                {
                    sResult += wxString::Format("\n\tRecurs %d times", pRecur->GetCount());
                    sResult += wxString::Format("\n\tRecur freq: %s", pRecur->GetFreqAsString());
                    if(pRecur->GetDays() != RECUR_NONE)
                        sResult += wxString::Format("\n\tRecur day: %s", pRecur->GetDaysAsString());
                }
                m_pLstLog->AppendAndEnsureVisible(sResult);
            }
            else
                m_pLstLog->AppendAndEnsureVisible(wxString::Format("%d: ERROR PARSING EVENT", ++nEventCount));
            if(pRecur)
            {
                delete pRecur;
                pRecur = NULL;
            }
        }
    }


//    m_pLstLog->AppendAndEnsureVisible(wxString::Format("Found section: %s", sLine.Mid(6)));

}

