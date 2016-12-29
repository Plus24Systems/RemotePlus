//----------------------------------------------------------------------------//
//  Application: Remote Plus for AXIOM Cameras                                //
//                                                                            //
//  Author:   Phil Kerr                                                       //
//  Company:  Plus24 Systems Ltd.                                             //
//  GitHub:   https://github.com/Plus24Systems/RemotePlus                     //
//  Copyright (C) 2016 Phil Kerr - Plus24 Systems Ltd.                        //
//                                                                            //
//   This program is free software: you can redistribute it and/or modify     //
//   it under the terms of the GNU General Public License as published by     //
//   the Free Software Foundation, either version 3 of the License, or        //
//   (at your option) any later version.                                      //
//                                                                            //
//   This program is distributed in the hope that it will be useful,          //
//   but WITHOUT ANY WARRANTY; without even the implied warranty of           //
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
//   GNU General Public License for more details.                             //
//                                                                            //
//   You should have received a copy of the GNU General Public License        //
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//                                                                            //
//                                                                            //
//----------------------------------------------------------------------------//

#ifndef TEMPWIDGET_H
#define TEMPWIDGET_H

#include <wx/wx.h>
#include <termios.h>

#include "RemotePlusDefines.h"

// The following are used to feedback any UART errors to the connection widget
//  from the other widgets - this is a temp. measure and will be replaced.
wxDECLARE_EVENT (TEMP_CONNECTION_EVENT, wxCommandEvent);

//----------------------------------------------------------------------------//

class TemperatureWidget;

class TemperatureThread : public wxThread
{
    public:
        TemperatureThread(TemperatureWidget *widget) : wxThread(wxTHREAD_JOINABLE)
        {
            temp_widget = widget;
        }

        virtual ExitCode Entry () wxOVERRIDE;
        void uart_tx (int data);
    
    private:
        TemperatureWidget *temp_widget;
        int tempFormat;
        int uart0;
        FILE *fp;
        char path [1000];        
};

//----------------------------------------------------------------------------//

wxDECLARE_EVENT (TEMP_EVENT, wxCommandEvent);

//----------------------------------------------------------------------------//

class TemperatureWidget: public wxPanel
{
    public:
        TemperatureWidget (wxWindow* parent, wxWindowID id);
        ~TemperatureWidget ();

        void OnTemperatureEvent (wxCommandEvent& event);

    private:
        void OnPaint (wxPaintEvent&);

        TemperatureThread temp_thread;
        wxString temptext;
};

#endif // TemperatureWidget

//----------------------------------------------------------------------------//
