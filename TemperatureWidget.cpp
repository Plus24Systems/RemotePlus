//----------------------------------------------------------------------------//
//  Application: Remote Plus for AXIOM Cameras                                //
//                                                                            //
//  Author:   Phil Kerr                                                       //
//  Company:  Plus24 Systems Ltd.                                             //
//  Website:  www.plus24systems.com/RemotePlus                                //
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

#include "TemperatureWidget.h"

//----------------------------------------------------------------------------//

wxDEFINE_EVENT (TEMP_EVENT, wxCommandEvent);
wxDEFINE_EVENT (TEMP_CONNECTION_EVENT, wxCommandEvent);

//----------------------------------------------------------------------------//

TemperatureWidget::TemperatureWidget (wxWindow* parent, wxWindowID id) : wxPanel (parent, id, wxDefaultPosition, wxSize (135, 60)), temp_thread (this)
{
    if (temp_thread.Create () != wxTHREAD_NO_ERROR ||
         temp_thread.Run () != wxTHREAD_NO_ERROR )
    {
        wxLogError (wxT("Can't create/run thread!"));
        return;
    }
    
    Connect (wxEVT_PAINT, wxPaintEventHandler (TemperatureWidget::OnPaint));    
}

//----------------------------------------------------------------------------//

TemperatureWidget::~TemperatureWidget()
{
    temp_thread.Delete ();
}

//----------------------------------------------------------------------------//

void TemperatureWidget::OnTemperatureEvent (wxCommandEvent& event)
{
    // Reset the display text
    temptext.Clear ();
    
    // Append the temp type, in this case centegrade, to the new temp display text
    temptext << event.GetString().Left(6) << "c";

    // Repaint the widget
    Refresh ();  
}

//----------------------------------------------------------------------------//

void TemperatureWidget::OnPaint (wxPaintEvent& WXUNUSED(evt))
{
    // This is the main paint object
    wxPaintDC dc (this);
    
    // Set widget label
    wxString templabel = wxT ("Temperature");    
    
    // Set font
    wxFont temptextfont (14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT ("sans-serif"));

    // Setup for calculating the center of the widget for the function text
    dc.SetFont (temptextfont);
    wxSize tempwidth = this -> GetClientSize ();
    wxCoord w, h;

    // Calculate the center of the widget for the label text
    dc.GetTextExtent (templabel, &w, &h);
    int templabelmiddle = wxMax (0, (tempwidth.x - w) / 2);
    
    // Calculate the center of the widget for the function text
    dc.GetTextExtent (temptext, &w, &h);
    int temptextmiddle = wxMax (0, (tempwidth.x - w) / 2);

    // Draw main widget
    dc.SetBrush (wxBrush (wxColour (155, 255, 155)));
    dc.DrawRectangle (0, 0, 135, 60);

    // Set temp display text 
    dc.SetFont (temptextfont);
    dc.SetTextForeground (wxColour(55, 55, 55));         
    
    // Render text  
    dc.DrawText (templabel, templabelmiddle, 6);         
    dc.DrawText (temptext, temptextmiddle, 36);  
}

//----------------------------------------------------------------------------//

wxThread::ExitCode TemperatureThread::Entry ()
{
    int signal;
    
    // Set initial value
    signal = UART_ERROR;   
    
    // Open the UART interface
    uart0 = open ("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);

    // This inital check opens the UART channel fully.
    if (uart0 == -1) 
        signal = UART_ERROR;
    else if (uart0 >= 0) 
        signal = UART_CONNECTED;    
 
    // Initial Connection Status Event
    wxCommandEvent custom_event (TEMP_CONNECTION_EVENT);
    custom_event.SetInt (signal); 
    wxTheApp -> GetTopWindow () -> GetEventHandler () -> AddPendingEvent (custom_event);   

    // Set up the UART
    struct termios options;

    tcgetattr (uart0, &options);
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_iflag = ICRNL;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush (uart0, TCIFLUSH);
    tcsetattr (uart0, TCSANOW, &options);

    while (1) {
        if (uart0 != -1) {
            // Send a request
            uart_tx (GET_AXIOM_TEMP_REQUEST);
        } else {
            // Connection Status Error Event
            wxCommandEvent custom_event (TEMP_CONNECTION_EVENT);
            custom_event.SetInt (UART_ERROR); 
            wxTheApp -> GetTopWindow () -> GetEventHandler () -> AddPendingEvent (custom_event);          
        }

        wxMilliSleep (800);
    }

    return (ExitCode)0;
}

//----------------------------------------------------------------------------//

void TemperatureThread::uart_tx (int data) 
{
    int datalen;

    datalen = 1;

    unsigned char message [datalen];

    message [0] = data;
        
    if (uart0 != -1) {
        int count = write (uart0, message, 1);      
      
        if (count < 0) {
            // Connection Status Event
            wxCommandEvent custom_event (TEMP_CONNECTION_EVENT);
            custom_event.SetInt (UART_ERROR); 
            wxTheApp -> GetTopWindow () -> GetEventHandler () -> AddPendingEvent (custom_event);          
        } 
    }
}

//----------------------------------------------------------------------------//



