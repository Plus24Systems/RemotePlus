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

#ifndef AXIOMSYSWIDGET_H
#define AXIOMSYSWIDGET_H

#include <wx/wx.h>
#include <termios.h>

#include "RemotePlusDefines.h"

//----------------------------------------------------------------------------//

class AxiomSystemWidget;

class AxiomSystemThread : public wxThread
{
    public:
        AxiomSystemThread (AxiomSystemWidget *widget) : wxThread (wxTHREAD_JOINABLE)
        {
            axiom_widget = widget;
        }

        virtual ExitCode Entry () wxOVERRIDE;
        void uart_tx (int data);
    
    private:
        AxiomSystemWidget *axiom_widget;
        int uart0;
       
};

//----------------------------------------------------------------------------//

wxDECLARE_EVENT (AXIOMSYS_EVENT, wxCommandEvent);

// The following are used to feedback any UART errors to the connection widget
//  from the other widgets - this is a temp. measure and will be replaced.
wxDECLARE_EVENT (AXIOM_CONNECTION_EVENT, wxCommandEvent);

//----------------------------------------------------------------------------//

class AxiomSystemWidget: public wxPanel
{
    public:
        AxiomSystemWidget (wxWindow* parent, wxWindowID id, int stat);
        ~AxiomSystemWidget ();

        void OnAxiomSysEvent (wxCommandEvent& event);
        void uart_tx (int msgType, int data); 

    private:
        void OnPaint (wxPaintEvent&);

        AxiomSystemThread axiom_thread;
        int status;
        wxString systext;
        int uart0;    
};

#endif // AxiomSystemWidget

//----------------------------------------------------------------------------//
