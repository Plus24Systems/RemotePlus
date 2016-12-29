///----------------------------------------------------------------------------//
///  Application: Remote Plus for AXIOM Cameras                                //
///                                                                            //
///  Author:   Phil Kerr                                                       //
///  Company:  Plus24 Systems Ltd.                                             //
///  Website:  www.plus24systems.com/RemotePlus                                //
///  Copyright (C) 2016 Phil Kerr - Plus24 Systems Ltd.                        //
///                                                                            //
///   This program is free software: you can redistribute it and/or modify     //
///   it under the terms of the GNU General Public License as published by     //
///   the Free Software Foundation, either version 3 of the License, or        //
///   (at your option) any later version.                                      //
///                                                                            //
///   This program is distributed in the hope that it will be useful,          //
///   but WITHOUT ANY WARRANTY; without even the implied warranty of           //
///   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
///   GNU General Public License for more details.                             //
///                                                                            //
///   You should have received a copy of the GNU General Public License        //
///   along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
///                                                                            //
///                                                                            //
///----------------------------------------------------------------------------//

#include "RemotePlusApp.h"
#include "RemotePlusMain.h"
#include "MenuWidget.h"

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>

//----------------------------------------------------------------------------//

IMPLEMENT_APP (RemotePlusApp)

wxDEFINE_EVENT (GPIO_EVENT, wxCommandEvent);
wxDEFINE_EVENT (GPIO_OK_EVENT, wxCommandEvent);
wxDEFINE_EVENT (GPIO_CANCEL_EVENT, wxCommandEvent);
wxDEFINE_EVENT (GPIO_ROTARY_EVENT, wxCommandEvent);

//----------------------------------------------------------------------------//

//  Main widgets
const long RemotePlusMain::ID_NOTEBOOKMAIN = wxNewId();
const long RemotePlusMain::ID_MAIN = wxNewId();
const long RemotePlusMain::ID_DEBUG = wxNewId();

const long RemotePlusMain::ID_MIDDLE = wxNewId();
const long RemotePlusMain::ID_CONFIGMIDDLE = wxNewId();

//  Sub-Widget Componenets
const long RemotePlusMain::ID_CSPANEL = wxNewId();
const long RemotePlusMain::ID_CSBOX = wxNewId();

const long RemotePlusMain::ID_CONSTAT = wxNewId();
const long RemotePlusMain::ID_TEMP = wxNewId();
const long RemotePlusMain::ID_SYSTEM = wxNewId();

//  Menu Button Widgets
const long RemotePlusMain::ID_TMENU1 = wxNewId();
const long RemotePlusMain::ID_TMENU2 = wxNewId();
const long RemotePlusMain::ID_TMENU3 = wxNewId();
const long RemotePlusMain::ID_TMENU4 = wxNewId();

const long RemotePlusMain::ID_BMENU1 = wxNewId();
const long RemotePlusMain::ID_BMENU2 = wxNewId();
const long RemotePlusMain::ID_BMENU3 = wxNewId();
const long RemotePlusMain::ID_BMENU4 = wxNewId();

//  Function Button Bitmaps - Main
const long RemotePlusMain::ID_CancelBitmap = wxNewId();
const long RemotePlusMain::ID_OKBitmap = wxNewId();
const long RemotePlusMain::ID_NextBitmap = wxNewId();
const long RemotePlusMain::ID_BackBitmap = wxNewId();
const long RemotePlusMain::ID_ConfigBitmap = wxNewId();
const long RemotePlusMain::ID_HomeBitmap = wxNewId();

//  Function Button Bitmaps - Config
const long RemotePlusMain::ID_ConfigCancelBitmap = wxNewId();
const long RemotePlusMain::ID_ConfigOKBitmap = wxNewId();
const long RemotePlusMain::ID_ConfigNextBitmap = wxNewId();
const long RemotePlusMain::ID_ConfigBackBitmap = wxNewId();
const long RemotePlusMain::ID_ConfigConfigBitmap = wxNewId();
const long RemotePlusMain::ID_ConfigHomeBitmap = wxNewId();

//----------------------------------------------------------------------------//

RemotePlusMain::RemotePlusMain (const wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size) : wxFrame(NULL, wxID_ANY, title, pos, size), uart_thread (this)
{
    SetClientSize (wxSize (SCREENX, SCREENY));
    SetBackgroundColour (wxColour (0, 0, 0));    

    //=============================================================//
    //  Main display
    MainNotebook = new wxNotebook (this, ID_NOTEBOOKMAIN, wxDefaultPosition, wxSize (SCREENX, SCREENY), 0, _T("ID_NOTEBOOKMAIN"));
    MainNotebook -> SetBackgroundColour (wxColour (0,0,0)); 
    MainNotebook -> SetForegroundColour (wxColour (255,255,255));
    MainNotebook -> SetMinSize (wxSize (SCREENX, SCREENY));
    MainNotebook -> SetMaxSize (wxSize (SCREENX, SCREENY));

    //=============================================================//    
    //  
    MainSettings = new wxPanel (MainNotebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
    MainSettings -> SetBackgroundColour (wxColour (0, 0, 0));
    MainSettings -> SetForegroundColour (wxColour (255,255,255)); 

    //=============================================================//    
    //  Function Button defines
    NextBitmap = new wxStaticBitmap (MainSettings, wxID_ANY, wxBitmap ("/usr/share/icons/RemotePlus/go-next.png", wxBITMAP_TYPE_PNG), wxPoint (ICONXLEFT, ICONYONE), wxSize (24, 24), wxSIMPLE_BORDER);    
    HomeBitmap = new wxStaticBitmap (MainSettings, wxID_ANY, wxBitmap ("/usr/share/icons/RemotePlus/user-home.png", wxBITMAP_TYPE_PNG), wxPoint (ICONXRIGHT, ICONYONE), wxSize (24, 24), wxSIMPLE_BORDER); 

    CancelBitmap = new wxStaticBitmap(MainSettings, wxID_ANY, wxBitmap ("/usr/share/icons/RemotePlus/gtk-cancel.png", wxBITMAP_TYPE_PNG), wxPoint (ICONXLEFT, ICONYTWO), wxSize (24, 24), wxSIMPLE_BORDER);    
    OKBitmap = new wxStaticBitmap(MainSettings, wxID_ANY, wxBitmap ("/usr/share/icons/RemotePlus/gtk-ok.png", wxBITMAP_TYPE_PNG), wxPoint (ICONXRIGHT, ICONYTWO), wxSize (24, 24), wxSIMPLE_BORDER); 

    BackBitmap = new wxStaticBitmap(MainSettings, wxID_ANY, wxBitmap("/usr/share/icons/RemotePlus/go-previous.png", wxBITMAP_TYPE_PNG), wxPoint(ICONXLEFT, ICONYTHREE), wxSize (24, 24), wxSIMPLE_BORDER);    
    ConfigBitmap = new wxStaticBitmap(MainSettings, wxID_ANY, wxBitmap ("/usr/share/icons/RemotePlus/applications-utilities.png", wxBITMAP_TYPE_PNG), wxPoint(ICONXRIGHT, ICONYTHREE), wxSize (24, 24), wxSIMPLE_BORDER); 

    TopMenu1 = new MenuWidget (MainSettings, ID_TMENU1, MENU_ONE, MENU_TOP, MENU_NOT_ACTIVE);
    TopMenu2 = new MenuWidget (MainSettings, ID_TMENU2, MENU_TWO, MENU_TOP, MENU_NOT_ACTIVE);
    TopMenu3 = new MenuWidget (MainSettings, ID_TMENU3, MENU_THREE, MENU_TOP, MENU_NOT_ACTIVE);
    TopMenu4 = new MenuWidget (MainSettings, ID_TMENU4, MENU_FOUR, MENU_TOP, MENU_NOT_ACTIVE);

    BotMenu1 = new MenuWidget (MainSettings, ID_BMENU1, MENU_FIVE, MENU_BOTTOM, MENU_NOT_ACTIVE);
    BotMenu2 = new MenuWidget (MainSettings, ID_BMENU2, MENU_SIX, MENU_BOTTOM, MENU_NOT_ACTIVE);
    BotMenu3 = new MenuWidget (MainSettings, ID_BMENU3, MENU_SEVEN, MENU_BOTTOM, MENU_NOT_ACTIVE);
    BotMenu4 = new MenuWidget (MainSettings, ID_BMENU4, MENU_EIGHT, MENU_BOTTOM, MENU_NOT_ACTIVE);

    //=============================================================//  
    // Remote Center    
    MainMiddle = new wxPanel(MainSettings, wxID_ANY, wxPoint (30, 20), wxDefaultSize, wxTAB_TRAVERSAL);
    MainMiddle -> SetBackgroundColour (wxColour (50,50,50));
    MainMiddle -> SetForegroundColour (wxColour (255,255,255));
    MainMiddle -> SetMinSize (wxSize (150,295));
    MainMiddle -> SetMaxSize (wxSize (650,295));      
    
    //=============================================================//  
    //  
    TemperatureWidget *Temp = new TemperatureWidget(MainMiddle, ID_TEMP);

    //=============================================================//  
    //  
    ConStat = new ConnectionWidget (MainMiddle, ID_CONSTAT, NOT_CONNECTED);      
   
    //=============================================================//  
    //      
    AxiomSystem = new AxiomSystemWidget (MainMiddle, ID_SYSTEM, AXIOM_NOT_STARTED);    

    //=============================================================//     
    //      
    wxGridBagSizer* mainsizer = new wxGridBagSizer(8, 8);
    mainsizer -> SetEmptyCellSize (wxSize (26, 30));
      
    //=============================================================//  
    //            
    mainsizer -> Add (MainMiddle, wxGBPosition (1, 1), wxGBSpan (7, 7), wxALL|wxEXPAND, 0);

    mainsizer -> Add(TopMenu1, wxGBPosition (0, 1), wxGBSpan (1, 1), wxALIGN_LEFT|wxALIGN_BOTTOM|wxLEFT|wxRIGHT|wxTOP, 0);
    mainsizer -> Add(TopMenu2, wxGBPosition (0, 3), wxGBSpan (1, 1), wxALIGN_LEFT|wxALIGN_BOTTOM|wxLEFT|wxRIGHT|wxTOP, 0);
    mainsizer -> Add(TopMenu3, wxGBPosition (0, 5), wxGBSpan (1, 1), wxALIGN_LEFT|wxALIGN_BOTTOM|wxLEFT|wxRIGHT|wxTOP, 0);
    mainsizer -> Add(TopMenu4, wxGBPosition (0, 7), wxGBSpan (1, 1), wxALIGN_LEFT|wxALIGN_BOTTOM|wxLEFT|wxRIGHT|wxTOP, 0);

    mainsizer -> Add(BotMenu1, wxGBPosition (8, 1), wxGBSpan (1, 1), wxALIGN_LEFT|wxALIGN_BOTTOM|wxLEFT|wxRIGHT|wxTOP, 0);
    mainsizer -> Add(BotMenu2, wxGBPosition (8, 3), wxGBSpan (1, 1), wxALIGN_LEFT|wxALIGN_BOTTOM|wxLEFT|wxRIGHT|wxTOP, 0);
    mainsizer -> Add(BotMenu3, wxGBPosition (8, 5), wxGBSpan (1, 1), wxALIGN_LEFT|wxALIGN_BOTTOM|wxLEFT|wxRIGHT|wxTOP, 0);
    mainsizer -> Add(BotMenu4, wxGBPosition (8, 7), wxGBSpan (1, 1), wxALIGN_LEFT|wxALIGN_BOTTOM|wxLEFT|wxRIGHT|wxTOP, 0);

    MainSettings -> SetSizer (mainsizer);       
        
    //=============================================================//
    //
    wxGridBagSizer* middlesizer = new wxGridBagSizer (8, 8);
    middlesizer -> SetEmptyCellSize (wxSize (26, 30));    
    middlesizer -> Add (ConStat, wxGBPosition (0, 0), wxGBSpan (2, 2), wxALL|wxEXPAND, 1);
    middlesizer -> Add (AxiomSystem, wxGBPosition (0, 6), wxGBSpan (2, 2), wxALL|wxEXPAND, 1);
    middlesizer -> Add (Temp, wxGBPosition (0, 12), wxGBSpan (2, 2), wxALL|wxEXPAND, 1);    
    MainMiddle -> SetSizer (middlesizer);      
        
    //=============================================================//
    // Add panels to the main notebook display
    MainNotebook -> AddPage (MainSettings, _("Main"), false);

    SetClientSize (wxSize (SCREENX, SCREENY));
    SetMinSize (wxSize (SCREENX, SCREENY));
    SetMaxSize (wxSize (SCREENX, SCREENY));
    SetSize (SCREENX, SCREENY);

    //=============================================================//
    // Create & start the UART Thread
    if (uart_thread.Create () != wxTHREAD_NO_ERROR || uart_thread.Run () != wxTHREAD_NO_ERROR) {
        wxLogError(wxT("Can't create/run gpio thread!"));
        return;
    }  

    //=============================================================//    
    //  Bind keypresses (used for development only)
    this -> Bind (wxEVT_CHAR_HOOK, &RemotePlusMain::OnKeyDown, this);       

    //=============================================================//    
    //  Bind for Connection Widget UART Events
    this -> Bind (CONNECTION_EVENT, &ConnectionWidget::OnConnectionEvent, ConStat);
    this -> Bind (AXIOM_CONNECTION_EVENT, &ConnectionWidget::OnConnectionEvent, ConStat);  
    this -> Bind (TEMP_CONNECTION_EVENT, &ConnectionWidget::OnConnectionEvent, ConStat);  
    
    //=============================================================//    
    //  Bind for Axiom System Widget UART Events
    this -> Bind (AXIOMSYS_EVENT, &AxiomSystemWidget::OnAxiomSysEvent, AxiomSystem);    
    
    //=============================================================//
    //  Bind for Temperature Widget UART Events
    this -> Bind (TEMP_EVENT, &TemperatureWidget::OnTemperatureEvent, Temp);
    
    //=============================================================//
    //  Bind for Menu Widget Set Gain Events - non functional, to fix
    //this -> Bind (GET_GAIN_EVENT, &MenuWidget::setGainValue, TopMenu4);    
    
    // Sets the default active menu
    activeMenu = -1;
}

//----------------------------------------------------------------------------//

void RemotePlusMain::OnPaint (wxPaintEvent& event)
{
//  NOP
}

//----------------------------------------------------------------------------//

RemotePlusMain::~RemotePlusMain ()
{
//  NOP
}

//----------------------------------------------------------------------------//

void RemotePlusMain::OnSize (wxSizeEvent& event)
{
    Refresh ();
}

//----------------------------------------------------------------------------//

void RemotePlusMain::OnKeyDown (wxKeyEvent& event)
{
    switch ((int) event.GetKeyCode ()) {
        case MENU1KEY: 
            MenuEventDispatch (MENU_ONE);
        break;  

        case MENU2KEY: 
            MenuEventDispatch (MENU_TWO);
        break;  
        
        case MENU3KEY: 
            MenuEventDispatch (MENU_THREE);
        break;  

        case MENU4KEY: 
            MenuEventDispatch (MENU_FOUR);
        break;  

        case MENU5KEY: 
            MenuEventDispatch (MENU_FIVE);
        break;  

        case MENU6KEY: 
            MenuEventDispatch (MENU_SIX);
        break;  
        
        case MENU7KEY: 
            MenuEventDispatch (MENU_SEVEN);
        break;  

        case MENU8KEY: 
            MenuEventDispatch (MENU_EIGHT);
        break;  
        
        case FUNCOKKEY: 
            MenuEventDispatch (FUNC_OK);
        break;  

        case FUNCCANCELKEY: 
            MenuEventDispatch (FUNC_CANCEL);
        break;
    }

    event.Skip ();
}

//----------------------------------------------------------------------------//

void RemotePlusMain::ResetMenus ()
{
    TopMenu1 -> setMenuOff (); 
    TopMenu2 -> setMenuOff (); 
    TopMenu3 -> setMenuOff (); 
    TopMenu4 -> setMenuOff (); 
    BotMenu1 -> setMenuOff ();       
    BotMenu2 -> setMenuOff ();    
    BotMenu3 -> setMenuOff ();    
    BotMenu4 -> setMenuOff ();    
}

///----------------------------------------------------------------------------//

void RemotePlusMain::MenuEventDispatch (int item)
{    
    if (item >= 0 && item <= 8)
        activeMenu = item;

    switch (item) {
        case MENU_ONE: {
            ResetMenus ();
            this -> Bind (MENU_CONNECTION_EVENT, &ConnectionWidget::OnConnectionEvent, ConStat);                  
            TopMenu1 -> setMenuOn ();
            TopMenu1 -> showMenuDialogBox (MENU_ONE);
            TopMenu1 -> setMenuOff (); 
            activeMenu = MENU_ONE;
        } break;

        case MENU_TWO: {
            ResetMenus ();
            this -> Bind (MENU_CONNECTION_EVENT, &ConnectionWidget::OnConnectionEvent, ConStat);              
            TopMenu2 -> setMenuOn ();
            TopMenu2 -> showMenuDialogBox (MENU_TWO);
            TopMenu2 -> setMenuOff (); 
        } break;
            
        case MENU_THREE: {
            ResetMenus ();
            this -> Bind (MENU_CONNECTION_EVENT, &ConnectionWidget::OnConnectionEvent, ConStat);              
            TopMenu3 -> setMenuOn ();
            TopMenu3 -> showMenuDialogBox (MENU_THREE);
            TopMenu3 -> setMenuOff (); 
        } break;
        
        case MENU_FOUR: {
            ResetMenus ();
            this -> Bind (MENU_CONNECTION_EVENT, &ConnectionWidget::OnConnectionEvent, ConStat);              
            TopMenu4 -> setMenuOn ();
            TopMenu4 -> showMenuDialogBox (MENU_FOUR);
            TopMenu4 -> setMenuOff (); 
        } break;                    

        case MENU_FIVE: {
            ResetMenus ();
            this -> Bind (MENU_CONNECTION_EVENT, &ConnectionWidget::OnConnectionEvent, ConStat);              
            BotMenu1 -> setMenuOn ();
            BotMenu1 -> showMenuDialogBox (MENU_FIVE);
            BotMenu1 -> setMenuOff (); 
        } break;

        case MENU_SIX: {
            ResetMenus ();
            this -> Bind (MENU_CONNECTION_EVENT, &ConnectionWidget::OnConnectionEvent, ConStat);              
            BotMenu2 -> setMenuOn ();
            BotMenu2 -> showMenuDialogBox (MENU_SIX);
            BotMenu2 -> setMenuOff (); 
        } break;
            
        case MENU_SEVEN: {
            ResetMenus ();
            this -> Bind (MENU_CONNECTION_EVENT, &ConnectionWidget::OnConnectionEvent, ConStat);              
            BotMenu3 -> setMenuOn ();
            BotMenu3 -> showMenuDialogBox (MENU_SEVEN);
            BotMenu3 -> setMenuOff (); 
        } break;
        
        case MENU_EIGHT: {
            ResetMenus ();
            this -> Bind (MENU_CONNECTION_EVENT, &ConnectionWidget::OnConnectionEvent, ConStat);              
            BotMenu4 -> setMenuOn ();
            BotMenu4 -> showMenuDialogBox (MENU_EIGHT);
            BotMenu4 -> setMenuOff (); 
        } break;                    
            
            
        case FUNC_OK: {
            ResetMenus ();          
            activeMenu = -1;
            wxCommandEvent gpio_ok_event (GPIO_OK_EVENT);
            
            switch (activeMenu) {
                case MENU_ONE:           
                    this -> Bind (GPIO_OK_EVENT, &MenuWidget::OnOKEvent, TopMenu1);             
                    gpio_ok_event.SetInt (MENU_ONE);                
                break;
                
                case MENU_TWO:                 
                    this -> Bind (GPIO_OK_EVENT, &MenuWidget::OnOKEvent, TopMenu2);              
                    gpio_ok_event.SetInt (MENU_TWO);   
                break;               
                 
                case MENU_THREE:           
                    this -> Bind (GPIO_OK_EVENT, &MenuWidget::OnOKEvent, TopMenu3);             
                    gpio_ok_event.SetInt (MENU_THREE);                
                break;
                
                case MENU_FOUR:                 
                    this -> Bind (GPIO_OK_EVENT, &MenuWidget::OnOKEvent, TopMenu4);              
                    gpio_ok_event.SetInt (MENU_FOUR);   
                break;                

                case MENU_FIVE:           
                    this -> Bind (GPIO_OK_EVENT, &MenuWidget::OnOKEvent, BotMenu1);             
                    gpio_ok_event.SetInt (MENU_FIVE);                
                break;
                
                case MENU_SIX:                 
                    this -> Bind (GPIO_OK_EVENT, &MenuWidget::OnOKEvent, BotMenu2);              
                    gpio_ok_event.SetInt (MENU_SIX);   
                break;               
                 
                case MENU_SEVEN:           
                    this -> Bind (GPIO_OK_EVENT, &MenuWidget::OnOKEvent, BotMenu3);             
                    gpio_ok_event.SetInt (MENU_SEVEN);                
                break;
                
                case MENU_EIGHT:                 
                    this -> Bind (GPIO_OK_EVENT, &MenuWidget::OnOKEvent, BotMenu4);              
                    gpio_ok_event.SetInt (MENU_EIGHT);   
                break;
            }
             
            wxTheApp -> GetTopWindow () -> GetEventHandler () -> AddPendingEvent (gpio_ok_event);                  
        } break;
            
        case FUNC_CANCEL: {
            ResetMenus ();          
            activeMenu = -1;            
            wxCommandEvent gpio_cancel_event (GPIO_CANCEL_EVENT);

            switch (activeMenu) {
                case MENU_ONE:           
                    this -> Bind (GPIO_CANCEL_EVENT, &MenuWidget::OnCancelEvent, TopMenu1);              
                    gpio_cancel_event.SetInt (MENU_ONE);            
                break;
               
                case MENU_TWO:                 
                    this -> Bind (GPIO_CANCEL_EVENT, &MenuWidget::OnCancelEvent, TopMenu2);              
                    gpio_cancel_event.SetInt (MENU_ONE);     
                break;               
                 
                case MENU_THREE:           
                    this -> Bind (GPIO_CANCEL_EVENT, &MenuWidget::OnCancelEvent, TopMenu3);              
                    gpio_cancel_event.SetInt (MENU_ONE);                 
                break;
                
                case MENU_FOUR:                 
                    this -> Bind (GPIO_CANCEL_EVENT, &MenuWidget::OnCancelEvent, TopMenu4);              
                    gpio_cancel_event.SetInt (MENU_ONE);    
                break;  
                
                case MENU_FIVE:           
                    this -> Bind (GPIO_CANCEL_EVENT, &MenuWidget::OnCancelEvent, BotMenu1);              
                    gpio_cancel_event.SetInt (MENU_FIVE);            
                break;
               
                case MENU_SIX:                 
                    this -> Bind (GPIO_CANCEL_EVENT, &MenuWidget::OnCancelEvent, BotMenu2);              
                    gpio_cancel_event.SetInt (MENU_SIX);     
                break;               
                 
                case MENU_SEVEN:           
                    this -> Bind (GPIO_CANCEL_EVENT, &MenuWidget::OnCancelEvent, BotMenu3);              
                    gpio_cancel_event.SetInt (MENU_SEVEN);                 
                break;
                
                case MENU_EIGHT:                 
                    this -> Bind (GPIO_CANCEL_EVENT, &MenuWidget::OnCancelEvent, BotMenu4);              
                    gpio_cancel_event.SetInt (MENU_EIGHT);    
                break;                
            }            

            wxTheApp -> GetTopWindow () -> GetEventHandler () -> AddPendingEvent (gpio_cancel_event); 
        } break;               
    }
}

//----------------------------------------------------------------------------//

wxThread::ExitCode RemotePlusUARTThread::Entry ()
{
    wxFileName UART ("/dev/ttyUSB0");  
    
    int signal;
    
    // Set initial value
    signal = UART_ERROR;

    // Cosmetic delay to allow the user to se that the conenction status changes.    
    wxMilliSleep (450);    

    // Open the UART interface
    uart0 = open ("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);

    // This inital check opens the UART channel fully.
    if (uart0 == -1) {
        signal = UART_ERROR;       
    } else if (uart0 >= 0) {
        signal = UART_CONNECTED;
    }    
 
    // Post Initial Connection Sucess Event
    wxCommandEvent custom_event (CONNECTION_EVENT);
    custom_event.SetInt (signal); 
    wxTheApp -> GetTopWindow () -> GetEventHandler () -> AddPendingEvent (custom_event);   

    struct termios options;

    tcgetattr (uart0, &options);
    options.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_iflag = ICRNL;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush (uart0, TCIFLUSH);
    tcsetattr (uart0, TCSANOW, &options); 


    // Send an inital Get the gain settings
    uart_tx (GET_GAIN_REQUEST);

    // Main UART rx loop
    while (1) {
        if (uart0 != -1) {
            unsigned char rx_buffer [256];
            char signal [100];


            int rx_length = read (uart0, (void*) rx_buffer, 255);

            if (rx_length < 0) {

            }
            else if (rx_length == 0) {
                //No data waiting
            } else {
                rx_buffer [rx_length] = '\0';
                
                if (rx_buffer [0] == GET_AXIOM_TEMP_RESPONSE) {
                    int loop;
                    for (loop = 1 ; loop < rx_length ; loop++)
                        signal [loop -1] = rx_buffer [loop];
                    
                    // Post Temp Message Event
                    wxCommandEvent temp_event (TEMP_EVENT);
                    temp_event.SetString (signal); 
                    wxTheApp -> GetTopWindow () -> GetEventHandler () -> AddPendingEvent (temp_event);  
                }
                
                if (rx_buffer [0] == GET_AXIOM_STATUS_RESPONSE) {
                    int loop;
                    for (loop = 1 ; loop < rx_length ; loop++)
                        signal [loop -1] = rx_buffer [loop];
                        
                    // Post AXIOM Firmware Event
                    wxCommandEvent axiomsys_event (AXIOMSYS_EVENT);
                    axiomsys_event.SetString (signal); 
                    wxTheApp -> GetTopWindow () -> GetEventHandler () -> AddPendingEvent (axiomsys_event);  
                }

                if (rx_buffer [0] == GET_GAIN_RESPONSE) {
                    int loop;
                    for (loop = 1 ; loop < rx_length ; loop++)
                        signal [loop -1] = rx_buffer [loop];
                                              
                    // Post GAIN Event
                    wxCommandEvent get_gain_event (GET_GAIN_EVENT);
                    get_gain_event.SetString (signal); 
                    wxTheApp -> GetTopWindow () -> GetEventHandler () -> AddPendingEvent (get_gain_event);  
                }                
            }
        }
    }  

    return (ExitCode)0;
}

//----------------------------------------------------------------------------//

void RemotePlusUARTThread::uart_tx (int data) 
{
    int datalen;

    datalen = 1;

    unsigned char message [datalen + 4];

    int signal;
    
    // Set initial value
    signal = UART_ERROR;
    
    message [0] = data;
    if (uart0 != -1) {
         int count = write (uart0, message, 1);      
        
        if (count < 0) {
        
            wxCommandEvent custom_event (CONNECTION_EVENT);
            custom_event.SetInt (signal); 
            wxTheApp -> GetTopWindow () -> GetEventHandler () -> AddPendingEvent (custom_event);   
        }
    }
}

//----------------------------------------------------------------------------//
