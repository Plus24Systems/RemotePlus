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

#ifndef REMOTEPLUSMAIN_H
#define REMOTEPLUSMAIN_H

#include <wx/wx.h>
#include <wx/thread.h>
#include <wx/dynarray.h>
#include <wx/numdlg.h>
#include <wx/progdlg.h>
#include <wx/event.h>

#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/frame.h>
#include <wx/gbsizer.h>
#include <wx/filename.h>

#include "MenuWidget.h"
#include "ConnectionWidget.h"
#include "AxiomSystemWidget.h"
#include "TemperatureWidget.h"

#include "RemotePlusDefines.h"

//----------------------------------------------------------------------------//

wxDECLARE_EVENT (UART_TX, wxCommandEvent);
wxDECLARE_EVENT (GPIO_EVENT, wxCommandEvent);

//----------------------------------------------------------------------------//

class RemotePlusMain;

class RemotePlusUARTThread : public wxThread
{
    public:
        RemotePlusUARTThread (RemotePlusMain *widget) : wxThread (wxTHREAD_JOINABLE)
        {
            uart_widget = widget;
        }

        virtual ExitCode Entry () wxOVERRIDE;
        void uart_tx (int data);
        void uart_tx (wxCommandEvent& event);    
    private:
        RemotePlusMain *uart_widget;
        int uart0;
};

//----------------------------------------------------------------------------//

class RemotePlusMain : public wxFrame, private wxLog
{
    public:
        RemotePlusMain (const wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size);
        virtual ~RemotePlusMain ();
    
        void OnSize (wxSizeEvent& event);
        void OnPaint (wxPaintEvent& event);   

        //  Used for testing on PC, not used on Remote    
        void OnKeyDown (wxKeyEvent& event);     

        //  Event handler for the two events above
        void MenuEventDispatch (int item);

        //  Used to reset the menu states prior to activating a menu
        void ResetMenus ();

    private:
        static const long ID_MAIN;
        static const long ID_CONFIG;
        static const long ID_DEBUG;
        static const long ID_NOTEBOOKMAIN;
       
        static const long ID_MIDDLE;        
        static const long ID_CONFIGMIDDLE;     
     
        static const long ID_TMENU1;
        static const long ID_TMENU2;
        static const long ID_TMENU3;
        static const long ID_TMENU4;        
        
        static const long ID_BMENU1;
        static const long ID_BMENU2;
        static const long ID_BMENU3;
        static const long ID_BMENU4; 

        static const long ID_CSPANEL;    
        static const long ID_CSBOX;

        //Connection Status Widget
        static const long ID_CONSTAT;
    
        //Temp Status Widget
        static const long ID_TEMP;    
        
        // AXIOM Firmware Widget 
        static const long ID_SYSTEM;
            
        // This is the main wx widget for the app
        wxNotebook* MainNotebook;    

        // This panel is the main camera settings view  
        wxPanel* MainSettings;

        // This panel contains the Remote Plus config settings view   
        wxPanel* ConfigSettings;   
        
        wxPanel* MainMiddle;
        wxPanel* ConfigMain;
        wxPanel* ConfigData;
                       
        // Function Button Icons - Main
        static const long ID_CancelBitmap;     
        static const long ID_OKBitmap;         
        static const long ID_NextBitmap;     
        static const long ID_BackBitmap;    
        static const long ID_ConfigBitmap;     
        static const long ID_HomeBitmap;         
        
        // Function Button Icons - Config        
        static const long ID_ConfigCancelBitmap;         
        static const long ID_ConfigOKBitmap;         
        static const long ID_ConfigNextBitmap;     
        static const long ID_ConfigBackBitmap;    
        static const long ID_ConfigConfigBitmap;     
        static const long ID_ConfigHomeBitmap;                   
        
        wxStaticBitmap* CancelBitmap;
        wxStaticBitmap* OKBitmap;    
        wxStaticBitmap* NextBitmap;
        wxStaticBitmap* BackBitmap;    
        wxStaticBitmap* ConfigBitmap;
        wxStaticBitmap* HomeBitmap;                        
                
        wxStaticBitmap* ConfigCancelBitmap;        
        wxStaticBitmap* ConfigOKBitmap;    
        wxStaticBitmap* ConfigNextBitmap;
        wxStaticBitmap* ConfigBackBitmap;    
        wxStaticBitmap* ConfigConfigBitmap;
        wxStaticBitmap* ConfigHomeBitmap;  
                        
        MenuWidget* TopMenu1;
        MenuWidget* TopMenu2;
        MenuWidget* TopMenu3;
        MenuWidget* TopMenu4;
    
        MenuWidget* BotMenu1;  
        MenuWidget* BotMenu2;          
        MenuWidget* BotMenu3;  
        MenuWidget* BotMenu4; 

        ConnectionWidget* ConStat;
        AxiomSystemWidget* AxiomSystem;
                                
        RemotePlusUARTThread uart_thread; 
                        
        int activeMenu;    
        int keyactive;                            
};

//----------------------------------------------------------------------------//

#endif // REMOTEPLUSMAIN_H
