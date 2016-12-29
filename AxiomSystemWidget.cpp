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

#include "AxiomSystemWidget.h"

//----------------------------------------------------------------------------//

wxDEFINE_EVENT (AXIOMSYS_EVENT, wxCommandEvent);
wxDEFINE_EVENT (AXIOM_CONNECTION_EVENT, wxCommandEvent);

//----------------------------------------------------------------------------//

AxiomSystemWidget::AxiomSystemWidget (wxWindow* parent, wxWindowID id, int status) : wxPanel (parent, id, wxDefaultPosition, wxSize (150, 60)), axiom_thread (this)
{

    // Create & start the AXIOM System Thread
    if (axiom_thread.Create () != wxTHREAD_NO_ERROR || axiom_thread.Run () != wxTHREAD_NO_ERROR) {
        wxLogError (wxT ("Can't create/run thread!"));
        return;
    }
    
    Connect (wxEVT_PAINT, wxPaintEventHandler (AxiomSystemWidget::OnPaint));
}

//----------------------------------------------------------------------------//

AxiomSystemWidget::~AxiomSystemWidget ()
{
    axiom_thread.Delete ();
}

//----------------------------------------------------------------------------//

void AxiomSystemWidget::OnAxiomSysEvent (wxCommandEvent& evt)
{
    systext.Clear ();

    wxString event = evt.GetString ();

    if (event == "NOT_STARTED") 
        status = AXIOM_NOT_STARTED;
    else if (event == "STARTING") 
        status = AXIOM_STARTING;
    else if (event == "RUNNING") 
        status = AXIOM_RUNNING;

    Refresh ();  
}

//----------------------------------------------------------------------------//

void AxiomSystemWidget::OnPaint (wxPaintEvent& WXUNUSED (evt))
{
    // This is the main paint object
    wxPaintDC dc (this);

    // Defines widget label
    wxString syslabel = wxT ("AXIOM");

    // Set widget status text
    switch (status) {
        case AXIOM_NOT_STARTED: 
            systext = "Stopped";
        break;
        case AXIOM_STARTING: 
            systext = "Starting";
        break;
        case AXIOM_RUNNING: 
            systext = "Running";
        break;             
        case AXIOM_ERROR: 
            systext = "Error";
        break; 
    }

    // Set font
    wxFont systextfont (14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT ("sans-serif"));

    // Setup for calculating the center of the widget for the function text
    dc.SetFont (systextfont);
    wxSize menuwidth = this -> GetClientSize ();
    wxCoord w, h;

    // Calculate the center of the widget for the label text
    dc.GetTextExtent (syslabel, &w, &h);
    int syslabelmiddle = wxMax (0, (menuwidth.x - w) / 2);
    
    // Calculate the center of the widget for the function text
    dc.GetTextExtent(systext, &w, &h);
    int systextmiddle = wxMax (0, (menuwidth.x - w) / 2);

    // Set widget colours
    switch (status) {
        case AXIOM_NOT_STARTED: 
            dc.SetBrush(wxBrush(wxColour(128, 128, 128)));
            dc.SetTextForeground(wxColour(200, 200, 200));            
        break;
        case AXIOM_STARTING: 
            dc.SetBrush(wxBrush(wxColour(255, 165, 0)));
            dc.SetTextForeground(wxColour(20, 20, 20));             
        break;
        case AXIOM_RUNNING: 
            dc.SetBrush(wxBrush(wxColour(55, 255, 55)));
            dc.SetTextForeground(wxColour(55, 55, 55));             
        break;             
        case AXIOM_ERROR: 
            dc.SetBrush(wxBrush(wxColour(255, 55, 55)));
            dc.SetTextForeground(wxColour(255, 255, 255));             
        break; 
    }

    // Render widget
    dc.DrawRectangle (0, 0, 150, 60);
    dc.SetFont (systextfont);
    
    dc.DrawText (syslabel, syslabelmiddle, 6);         
    dc.DrawText (systext, systextmiddle, 36);  
}

//----------------------------------------------------------------------------//

wxThread::ExitCode AxiomSystemThread::Entry ()
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
    wxCommandEvent custom_event (AXIOM_CONNECTION_EVENT);
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

    wxMilliSleep (800);    
    uart_tx (GET_AXIOM_STATUS_REQUEST);

    return (ExitCode)0;
}

//----------------------------------------------------------------------------//

void AxiomSystemThread::uart_tx (int data) 
{
    int datalen;

    datalen = 1;

    unsigned char message [datalen + 4];

    message [0] = data;
    if (uart0 != -1) {
         int count = write (uart0, message, 1);      
        
        if (count < 0) {
            // Connection Status Event
            wxCommandEvent custom_event (AXIOM_CONNECTION_EVENT);
            custom_event.SetInt (UART_ERROR); 
            wxTheApp -> GetTopWindow () -> GetEventHandler () -> AddPendingEvent (custom_event);   
        }
    }
}

//----------------------------------------------------------------------------//
