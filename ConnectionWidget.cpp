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

#include "ConnectionWidget.h"

//----------------------------------------------------------------------------//

wxDEFINE_EVENT(CONNECTION_EVENT, wxCommandEvent);

//----------------------------------------------------------------------------//

ConnectionWidget::ConnectionWidget (wxWindow* parent, wxWindowID id, int status) : wxPanel (parent, id, wxDefaultPosition, wxSize(150, 60)), connection_thread (this)
{
 
    // Create & start the AXIOM System Thread
    if (connection_thread.Create () != wxTHREAD_NO_ERROR || connection_thread.Run () != wxTHREAD_NO_ERROR) {
        return;
    }
    
    Connect (wxEVT_PAINT, wxPaintEventHandler (ConnectionWidget::OnPaint));    
} 

//----------------------------------------------------------------------------//

ConnectionWidget::~ConnectionWidget ()
{
//    connection_thread.Delete ();
}

//----------------------------------------------------------------------------//

void ConnectionWidget::OnConnectionEvent (wxCommandEvent& event)
{
    status = event.GetInt ();

    Refresh ();  
}

//----------------------------------------------------------------------------//

void ConnectionWidget::OnPaint (wxPaintEvent& WXUNUSED (evt))
{
    // This is the main paint object
    wxPaintDC dc (this);

    int fontsize = 0;
    wxString connectionlabel = wxT ("Remote");

    // Set-up status text
    switch (status) {
        case UART_NOT_CONNECTED: {
            connectiontext = "Not Connected";
            fontsize = 14;
        } break;        

        case UART_CONNECTED: {
            connectiontext = "Connected";
            fontsize = 15;            
        } break;  
        
        case UART_ERROR: {
            connectiontext = "Error Connecting";
            fontsize = 13;              
        } break;                  
    }    
    
    // Define message font
    wxFont connectiontextfont (fontsize, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT ("sans-serif")); 

    // Setup for calculating the center of the widget for the function text
    dc.SetFont(connectiontextfont);
    wxSize menuwidth = this -> GetClientSize ();
    wxCoord w, h;

    // Calculate the center of the widget for the function text
    dc.GetTextExtent(connectionlabel, &w, &h);
    int connectionlabelmiddle = wxMax (0, (menuwidth.x - w) / 2);
    
    // Calculate the center of the widget for the function text
    dc.GetTextExtent (connectiontext, &w, &h);
    int connectiontextmiddle = wxMax (0, (menuwidth.x - w) / 2);

    // Set-up colours
    switch (status) {
        case UART_NOT_CONNECTED: {
            dc.SetBrush (wxBrush (wxColour (128, 128, 128)));
            dc.SetTextForeground (wxColour (200, 200, 200));
        } break;
          
        case UART_CONNECTED: {
            dc.SetBrush (wxBrush (wxColour (55, 255, 55)));
            dc.SetTextForeground (wxColour (55, 55, 55));  
        } break; 
        
        case UART_ERROR: {
            dc.SetBrush (wxBrush (wxColour (255, 55, 55)));
            dc.SetTextForeground (wxColour (255, 255, 255));  
        } break;         
    }

    // Draw the widget border
    dc.DrawRectangle (0, 0, 150, 60);

    // Set the font atributes
    dc.SetFont (connectiontextfont);
    
    // Render the text 
    dc.DrawText (connectionlabel, connectionlabelmiddle, 6);          
    dc.DrawText (connectiontext, connectiontextmiddle, 36);  
}

//----------------------------------------------------------------------------//

wxThread::ExitCode ConnectionThread::Entry ()
{
    wxThreadEvent event (wxEVT_THREAD, CONTHREAD_EVENT);   
    wxFileName UART ("/dev/ttyUSB0");  

    // Inside this loop we just check to see if the ttyUSB0 device is there
    while (1) {
        if (UART.Exists()) {
            event.SetInt (CONNECTED);
            wxQueueEvent (connection_widget, event.Clone ());
        } else {
            event.SetInt (NOT_CONNECTED);
            wxQueueEvent (connection_widget, event.Clone ());   
        }

        wxMilliSleep (1000);
    }    

    return (ExitCode)0;
}

//----------------------------------------------------------------------------//
