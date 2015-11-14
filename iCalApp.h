/***************************************************************
 * Name:      iCalApp.h
 * Purpose:   Defines Application Class
 * Author:    Brian Walton (brian@riban.co.uk)
 * Created:   2015-11-14
 * Copyright: Brian Walton (riban.co.uk)
 * License:
 **************************************************************/

#ifndef ICALAPP_H
#define ICALAPP_H

#include <wx/app.h>

class iCalApp : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif // ICALAPP_H
