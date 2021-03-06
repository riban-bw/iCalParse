/***************************************************************
 * Name:      iCalApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Brian Walton (brian@riban.co.uk)
 * Created:   2015-11-14
 * Copyright: Brian Walton (riban.co.uk)
 * License:
 **************************************************************/

#include "iCalApp.h"

//(*AppHeaders
#include "iCalMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(iCalApp);

bool iCalApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	iCalFrame* Frame = new iCalFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
