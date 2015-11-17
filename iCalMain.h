/***************************************************************
 * Name:      iCalMain.h
 * Purpose:   Defines Application Frame
 * Author:    Brian Walton (brian@riban.co.uk)
 * Created:   2015-11-14
 * Copyright: Brian Walton (riban.co.uk)
 * License:
 **************************************************************/

#ifndef ICALMAIN_H
#define ICALMAIN_H

//(*Headers(iCalFrame)
#include <wx/menu.h>
#include <wx/listbox.h>
#include <wx/frame.h>
#include <wx/statusbr.h>
//*)

enum EVENT_PROPERTY
{
    EVENT_NONE,
    EVENT_SUMMARY,
    EVENT_LOCATION,
    EVENT_DESCRIPTION,
    EVENT_UID,
    EVENT_START,
    EVENT_END
};

enum EVENT_RECUR_FREQ
{
    RECUR_NONE,
    RECUR_DAILY,
    RECUR_WEEKLY,
    RECUR_MONTHLY,
    RECUR_YEARLY
};

class iCalFrame: public wxFrame
{
    public:

        iCalFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~iCalFrame();

    private:

        //(*Handlers(iCalFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnMenuOpenSelected(wxCommandEvent& event);
        //*)

        /** @brief  Parse recurring events
        *   @param  sRule The recurring event rule
        *   @todo   Return rule structure
        */
        void ParseRecur(wxString& sRule);


        //(*Identifiers(iCalFrame)
        static const long idLstLog;
        static const long idMenuOpen;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(iCalFrame)
        wxMenuItem* m_pMenuOpen;
        wxStatusBar* StatusBar1;
        wxListBox* m_pLstLog;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // ICALMAIN_H
