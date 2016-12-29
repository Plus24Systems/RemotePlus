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

#ifndef MENUWIDGET_H
#define MENUWIDGET_H

//----------------------------------------------------------------------------//

#include <wx/wx.h>
#include <wx/popupwin.h>
#include <wx/listbox.h>
#include <wx/event.h>
#include <wx/thread.h>
#include <wx/dynarray.h>
#include <wx/numdlg.h>
#include <wx/progdlg.h>
#include <wx/event.h>
#include <wx/filename.h>

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

#include "RemotePlusDefines.h"

//----------------------------------------------------------------------------//

//#define MENU_EXPOSURE 0
//#define MENU_MODE 1
//#define MENU_OVERLAYS 2
//#define MENU_GAIN 3
//#define MENU_HDMI_LIVE_STREAM 4

//#define NOT_CONNECTED 0
//#define CONNECTED 1
//#define ERROR -1

//#define MENU_NOT_ACTIVE 0
//#define MENU_ACTIVE 1

//#define MENU_TOP 0
//#define MENU_BOTTOM 1

//----------------------------------------------------------------------------//

wxDECLARE_EVENT (GET_GAIN_EVENT, wxCommandEvent);
wxDECLARE_EVENT (GPIO_OK_EVENT, wxCommandEvent);
wxDECLARE_EVENT (GPIO_CANCEL_EVENT, wxCommandEvent);
wxDECLARE_EVENT (GPIO_ROTARY_EVENT, wxCommandEvent);

//wxDECLARE_EVENT(MENU1_POPUP_EVENT, wxCommandEvent);
wxDECLARE_EVENT(MENU2_POPUP_EVENT, wxCommandEvent);
wxDECLARE_EVENT(MENU3_POPUP_EVENT, wxCommandEvent);
wxDECLARE_EVENT(MENU4_POPUP_EVENT, wxCommandEvent);
wxDECLARE_EVENT(MENU5_POPUP_EVENT, wxCommandEvent);
wxDECLARE_EVENT(MENU6_POPUP_EVENT, wxCommandEvent);
wxDECLARE_EVENT(MENU7_POPUP_EVENT, wxCommandEvent);
wxDECLARE_EVENT(MENU8_POPUP_EVENT, wxCommandEvent);

// The following are used to feedback any UART errors to the connection widget
//  from the other widgets - this is a temp. measure and will be replaced.
wxDECLARE_EVENT (MENU_CONNECTION_EVENT, wxCommandEvent);

//----------------------------------------------------------------------------//

class MenuWidget : public wxPanel
{
    public:

        MenuWidget(wxWindow* parent, wxWindowID id, int type, int row, int active);
        virtual ~MenuWidget();
        
        void showMenuDialogBox (int type); 

        void OnOKEvent (wxCommandEvent& event);
        void OnCancelEvent (wxCommandEvent& event);
        void OnGPIORotaryEvent (wxCommandEvent& event);         

        void uart_tx (int msgType, int data);
        
        void setMenuOn ();
        void setMenuOff ();        
        void setValue (wxString text);
        
        void OnPaint (wxPaintEvent& event);               
        
        void OnLeftKey ();
        void OnRightKey ();          

        wxDialog * menudialogbox;        
        int menutype;
        
    private:
        wxPanel *panel; 
        wxListBox* ListBox1; 
                
        static const long ID_POPUP;
        static const long ID_LISTBOX1;
        static const long ID_SELECTBUT;
        static const long ID_CANCELBUT;    

        wxString functext;
        wxString valtext;
            
        unsigned int menuposition;
        unsigned int origmenuposition;
        int menurow;    
        int menuactive;        
        bool popupActive;
        
        int uart0;        
/*
        wxPanel *panel; 
        wxListBox* ListBox1; 

        wxString functext;
        wxString valtext;
            
        int menutype;
        int menurow;    
        int menuposition;
        int origmenuposition;
        int menuactive;        
        bool popupActive;

        static const long ID_MENU1;
        static const long ID_MENU2;
        static const long ID_MENU3;
        static const long ID_MENU4;
        static const long ID_MENU5;
        static const long ID_MENU6;
        static const long ID_MENU7;
        static const long ID_MENU8;

        static const long ID_POPUP;

        static const long ID_LISTBOX1;
        static const long ID_SELECTBUT;
        static const long ID_CANCELBUT;     
*/
};

#endif // MENUWIDGET_H

//----------------------------------------------------------------------------//

