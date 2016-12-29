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

#include "MenuWidget.h"

//----------------------------------------------------------------------------//

wxDEFINE_EVENT (GET_GAIN_EVENT, wxCommandEvent);
wxDEFINE_EVENT (CANCEL_EVENT, wxCommandEvent);

const long MenuWidget::ID_LISTBOX1 = wxNewId ();
const long MenuWidget::ID_SELECTBUT = wxNewId ();
const long MenuWidget::ID_CANCELBUT = wxNewId ();

wxDEFINE_EVENT (MENU_CONNECTION_EVENT, wxCommandEvent);
wxDEFINE_EVENT (AXIOM_CONNECTION_EVENT, wxCommandEvent);

//----------------------------------------------------------------------------//

MenuWidget::MenuWidget (wxWindow* parent, wxWindowID id, int type, int row, int active) : wxPanel (parent, id, wxDefaultPosition, wxSize (150, 60))
{
    menutype = type;
    menurow = row;    
    menuactive = active;
    
    popupActive = FALSE;

    menuposition = 0;
        
    switch (menutype) {
        case MENU_ONE:           
            valtext = "10ms";         
        break;    
        case MENU_TWO:           
            valtext = "1080p60";         
        break; 
        case MENU_THREE:           
            valtext = "Disabled";          
        break; 
        case MENU_FOUR:           
            valtext = "x1";          
        break; 
        case MENU_FIVE:           
            valtext = " ";        
        break;     
        case MENU_SIX:           
            valtext = " ";            
        break; 
        case MENU_SEVEN:           
            valtext = " ";            
        break; 
        case MENU_EIGHT:           
            valtext = " ";            
        break; 
    } 
    
    this -> Bind (CANCEL_EVENT, &MenuWidget::OnCancelEvent, this);
    Connect (wxEVT_PAINT, wxPaintEventHandler (MenuWidget::OnPaint));
    
    
}

//----------------------------------------------------------------------------//

MenuWidget::~MenuWidget ()
{

}

//----------------------------------------------------------------------------//

void MenuWidget::OnPaint (wxPaintEvent& event)
{
    wxPaintDC dc (this);
        
    // Hardcoded menu type defines - will be replaced by user config in later releases.        
    switch (menutype) {
        case MENU_ONE:                
            functext = "Exposure";            
        break;         
        case MENU_TWO:                
            functext = "Video Mode";           
        break;  
        case MENU_THREE:                
            functext = "Overlays";             
        break;  
        case MENU_FOUR:                
            functext = "Gain";            
        break;   
        case MENU_FIVE:                
            functext = "HDR Mode";            
        break;              
        case MENU_SIX:                
            functext = "HDMI Live Stream";            
        break;  
        case MENU_SEVEN:                
            functext = "Colour Mode";            
        break;                         
        case MENU_EIGHT:                
           functext = "Crop Mode";            
        break; 
        case 99:                
            functext = "Undefined";
            valtext = " ";           
        break;                          
    }
            
    wxFont funcfont (11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT ("sans-serif"));
    wxFont valfont (16, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT ("sans-serif"));

    // Setup for calculating the center of the widget for the function text
    dc.SetFont (funcfont);
    wxSize menuwidth = this -> GetClientSize ();
    wxCoord w, h;

    // Calculate the center of the widget for the function text
    dc.GetTextExtent (functext, &w, &h);
    int functextmiddle = wxMax (0, (menuwidth.x - w) / 2);

    // Calculate the center of the widget for the value text
    dc.SetFont (valfont);
    dc.GetTextExtent (valtext, &w, &h);
    int valtextmiddle = wxMax (0, (menuwidth.x - w) / 2);

    // Set Menu background colour
    if (menuactive == MENU_NOT_ACTIVE) 
        dc.SetBrush (wxBrush (wxColour (255, 255, 255)));
    else if (menuactive == MENU_ACTIVE) 
        dc.SetBrush (wxBrush (wxColour (55, 155, 195)));

    // Render Menu basic frame
    if (menurow == MENU_TOP) {
        dc.SetPen (wxPen (wxColour(0, 0, 0)));
        dc.DrawRectangle (0, 0, 150, 60);
        dc.SetBrush (wxBrush (wxColour(0, 0, 0)));
        dc.DrawRectangle (2, 25, 146, 33);
    } else if (menurow == MENU_BOTTOM) {
        dc.SetPen (wxPen (wxColour (0, 0, 0)));
        dc.DrawRectangle (0, 0, 150, 60);
        dc.SetBrush (wxBrush (wxColour (0, 0, 0)));
        dc.DrawRectangle (2, 2, 146, 33);
    }

    // Set menu text foreground color
    dc.SetFont (funcfont);
    
    if (menuactive == MENU_NOT_ACTIVE)     
        dc.SetTextForeground (wxColour (0, 0, 0));
    else if (menuactive == MENU_ACTIVE)
        dc.SetTextForeground (wxColour (255, 255, 255));    
        
    // Render menu function text        
    if (menurow == MENU_TOP)
        dc.DrawText (functext, functextmiddle, 5);
    else if (menurow == MENU_BOTTOM)
        dc.DrawText (functext, functextmiddle, 40);
     
    // Set font for value text
    dc.SetFont (valfont);

    // Set menu text foreground color
    if (menuactive == MENU_NOT_ACTIVE)       
        dc.SetTextForeground (wxColour (255, 255, 255));
    else if (menuactive == MENU_ACTIVE)       
        dc.SetTextForeground (wxColour (255, 25, 25));
    
    // Render menu value text
    if (menurow == MENU_TOP)    
        dc.DrawText (valtext, valtextmiddle, 29);
    else if (menurow == MENU_BOTTOM)
        dc.DrawText (valtext, valtextmiddle, 7);
}

//----------------------------------------------------------------------------//

void MenuWidget::showMenuDialogBox (int mtype)
{
    menutype = mtype;

    // We check to see if the popup is not active, if so we mark it
    if (!popupActive) {
        popupActive = TRUE;

        origmenuposition = menuposition;
        
// Seperate wxDialog dimensions as keyboard use requires OK/Cancel buttons.
#if UseWiringPi        
        menudialogbox = new wxDialog (this, -1, _T(""), wxDefaultPosition, wxSize (210, 135));
#else
        menudialogbox = new wxDialog (this, -1, _T(""), wxDefaultPosition, wxSize (210, 195));        
#endif

        panel = new wxPanel (menudialogbox, -1);
        panel -> SetBackgroundColour (wxColour (80, 80, 80));
        panel -> SetForegroundColour (wxColour (155, 155, 155));    

        ListBox1 = new wxListBox (panel, ID_LISTBOX1, wxPoint (5, 5), wxSize (200, 125), 0, 0, 0, wxDefaultValidator, _T("ID_LISTBOX1"));

        // Populate the listbox - hadcoded values - will be replaced by user config in later releases. 
        switch (menutype) {
            case MENU_ONE: {                
                ListBox1 -> Append (_("5ms"));
                ListBox1 -> Append (_("10ms"));
                ListBox1 -> Append (_("15ms"));
                ListBox1 -> Append (_("20ms"));
                ListBox1 -> Append (_("25ms"));
                ListBox1 -> Append (_("30ms"));
                ListBox1 -> Append (_("35ms"));
                ListBox1 -> Append (_("40ms"));
            
                if (menuposition > ListBox1 -> GetCount ()) 
                    menuposition = ListBox1 -> GetCount ();            

                ListBox1 -> SetSelection (menuposition);
                ListBox1 -> EnsureVisible (menuposition);    
            } break; 
    
            case MENU_TWO: {     
                ListBox1 -> Append (_("1080p24"));
                ListBox1 -> Append (_("1080p25"));
                ListBox1 -> Append (_("1080p30"));
                ListBox1 -> Append (_("1080p50"));
                ListBox1 -> Append (_("1080p60"));
                ListBox1 -> Append (_("2160p24"));
                ListBox1 -> Append (_("2160p25"));
                ListBox1 -> Append (_("2160p30"));
                ListBox1 -> Append (_("2160p50"));
                ListBox1 -> Append (_("2160p60"));   
            
                if (menuposition > ListBox1 -> GetCount ()) 
                    menuposition = ListBox1 -> GetCount ();
                
                ListBox1 -> SetSelection (menuposition);
                ListBox1 -> EnsureVisible (menuposition); 
            } break;     
    
            case MENU_THREE: {     
                ListBox1 -> Append (_("Disable"));
                ListBox1 -> Append (_("Enable"));
                ListBox1 -> Append (_("Clear"));    
                
                if (menuposition > ListBox1 -> GetCount ()) 
                    menuposition = ListBox1 -> GetCount ();
                            
                ListBox1 -> SetSelection (menuposition);
                ListBox1 -> EnsureVisible (menuposition);            
            } break;      

            case MENU_FOUR: {     
                ListBox1 -> Append (_("x1"));
                ListBox1 -> Append (_("x3/3"));        
                ListBox1 -> Append (_("x2"));
                ListBox1 -> Append (_("x3"));
                ListBox1 -> Append (_("x4"));   
                
                if (menuposition > ListBox1 -> GetCount ()) 
                    menuposition = ListBox1 -> GetCount ();
                
                ListBox1 -> SetSelection (menuposition);
                ListBox1 -> EnsureVisible (menuposition); 
            } break;  
            
            case MENU_FIVE: {     
                ListBox1 -> Append (_(" ")); // Dummy Value
       

                ListBox1 -> SetSelection (menuposition);
                ListBox1 -> EnsureVisible (menuposition); 
            } break;  
            
            case MENU_SIX: {     
                ListBox1 -> Append (_(" ")); // Dummy Value

                ListBox1 -> SetSelection (menuposition);
                ListBox1 -> EnsureVisible (menuposition);   
            } break;              
            
            case MENU_SEVEN: {     
                ListBox1 -> Append (_(" "));// Dummy Value

                ListBox1 -> SetSelection (menuposition);
                ListBox1 -> EnsureVisible (menuposition);   
            } break;   
            
            case MENU_EIGHT: {     
                ListBox1 -> Append (_(" ")); // Dummy Value

                ListBox1 -> SetSelection (menuposition);
                ListBox1 -> EnsureVisible (menuposition);   
            } break;               
        }

        ListBox1 -> SetBackgroundColour (wxColour (50,50,50));
        ListBox1 -> SetForegroundColour (wxColour (255,255,255));

#if UseWiringPi
// Do nothing as we don't render wx buttons when we are on the physical remote
#else
        // Create buttons and bind events to them
        wxButton *SelectButton = new wxButton (panel, ID_SELECTBUT, wxT ("Select"), wxPoint (5, 135), wxSize (70, 30));
        wxButton *CancelButton = new wxButton (panel, ID_CANCELBUT, wxT ("Cancel"), wxPoint (135, 135), wxSize (70, 30));    

        SelectButton -> Bind (wxEVT_COMMAND_BUTTON_CLICKED, &MenuWidget::OnOKEvent, this);
        CancelButton -> Bind (wxEVT_COMMAND_BUTTON_CLICKED, &MenuWidget::OnCancelEvent, this);
#endif

        // Show dialog box, centered
        menudialogbox -> ShowModal ();
        menudialogbox -> Centre ();
    }    
}

//----------------------------------------------------------------------------//

void MenuWidget::setMenuOn ()
{
    menuactive = MENU_ACTIVE;
    Refresh ();    
}

//----------------------------------------------------------------------------//

void MenuWidget::setMenuOff ()
{
    menuactive = MENU_NOT_ACTIVE;
    Refresh ();    
}

//----------------------------------------------------------------------------//

void MenuWidget::setValue (wxString text)
{
    valtext = text;    
    Refresh ();    
}

//----------------------------------------------------------------------------//

void MenuWidget::OnGPIORotaryEvent (wxCommandEvent& event)
{
    if (popupActive) {
        int direction = event.GetInt ();
        
        menuposition = ListBox1 -> GetSelection (); 

        unsigned int count = (ListBox1 -> GetCount () - 1);

        if ((direction == Rotary_CW) && (menuposition < count))
            ListBox1 -> SetSelection (menuposition + 1);
    
        if ((direction == Rotary_CCW) && (menuposition > 0))
            ListBox1 -> SetSelection (menuposition - 1);
    }
}

//----------------------------------------------------------------------------//

void MenuWidget::OnOKEvent (wxCommandEvent& event)
{
    // Check if the popup is active
    if (popupActive) {
 
        menuposition = ListBox1 -> GetSelection ();

        this -> setValue (ListBox1 -> GetString (menuposition));
        menudialogbox -> EndModal (0);
        popupActive = FALSE;
        Refresh ();  

        int sel = ListBox1 -> GetSelection ();  
        wxString selection = ListBox1 -> GetString (sel);

        // This mapping of menutype to UART message 
        switch (menutype) {
            case MENU_ONE:           
                uart_tx (SET_EXPOSURE_REQUEST, sel);
            break;    
            case MENU_TWO:           
                uart_tx (SET_VIDEO_MODE_REQUEST, sel);
            break; 
            case MENU_THREE:           
                uart_tx (SET_OVERLAYS_REQUEST, sel);
            break; 
            case MENU_FOUR:           
                uart_tx (SET_GAIN_REQUEST, sel); 
            break; 
            case MENU_FIVE:           
                //       
            break;     
            case MENU_SIX:           
                //              
            break; 
            case MENU_SEVEN:           
                //             
            break; 
            case MENU_EIGHT:           
                //            
            break; 
        }    
    }
}

//----------------------------------------------------------------------------//

void MenuWidget::OnCancelEvent (wxCommandEvent& event)
{
    if (popupActive) {    
        ListBox1 -> SetSelection (origmenuposition);    
        menudialogbox -> EndModal (0);
        menudialogbox -> Close ();
        
        popupActive = FALSE;
        Refresh ();         
    }
}

//----------------------------------------------------------------------------//

void MenuWidget::uart_tx (int msgType, int data) 
{
//  Dirty Hack Warning!
//  Ideally this should be handled by RemotePlusUARTThread but as 
//  wxCommandEvents are encapsulated in the main GUI thread they are unavaiable
//  to worker threads.  This workaround will be replaced with wxMessageQueue 
//  events in a later release.

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
 
    // Post Initial Connection Sucess Event
    wxCommandEvent custom_event (MENU_CONNECTION_EVENT);
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
    
    int datalen;

    datalen = 2;

    unsigned char message [datalen];

    message [0] = msgType;
    message [1] = data;
    
    if (uart0 != -1) {
         int count = write (uart0, message, datalen);      
        
        if (count < 0) {
            signal = UART_ERROR;
            wxCommandEvent custom_event (MENU_CONNECTION_EVENT);
            custom_event.SetInt (signal); 
            wxTheApp -> GetTopWindow () -> GetEventHandler () -> AddPendingEvent (custom_event);              
        }
    }
    
    close (uart0);    
}

//----------------------------------------------------------------------------//

