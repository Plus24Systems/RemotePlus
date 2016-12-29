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

#ifndef CONNECTIONWIDGET_H
#define CONNECTIONWIDGET_H

#include <wx/wx.h>
#include <wx/event.h>
#include <wx/thread.h>
#include <wx/dynarray.h>
#include <wx/numdlg.h>
#include <wx/progdlg.h>
#include <wx/event.h>
#include <wx/filename.h>

#include "RemotePlusDefines.h"

//----------------------------------------------------------------------------//

class ConnectionWidget;

class ConnectionThread : public wxThread
{
    public:
        ConnectionThread (ConnectionWidget *widget) : wxThread (wxTHREAD_JOINABLE)
        {
            connection_widget = widget;
        }

        virtual ExitCode Entry () wxOVERRIDE;
        void uart_tx (int msgType, int data);
    
    private:
        ConnectionWidget *connection_widget;
        int uart0;    
};

//----------------------------------------------------------------------------//

wxDECLARE_EVENT (CONNECTION_EVENT, wxCommandEvent);
wxDECLARE_EVENT (UART_TX, wxCommandEvent);

//----------------------------------------------------------------------------//

class ConnectionWidget: public wxPanel
{
    public:
        ConnectionWidget (wxWindow* parent, wxWindowID id, int stat);
        ~ConnectionWidget ();
        
        void OnConnectionEvent (wxCommandEvent& event);
        void OnPaint (wxPaintEvent&);        
        
    private:
        int status;
        ConnectionThread connection_thread;
        wxString connectiontext;
};

enum {
    CONTHREAD_EVENT
};

#endif // ConnectionWidget

//----------------------------------------------------------------------------//
