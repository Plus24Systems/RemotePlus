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

#ifndef REMOTEPLUS_DEFINES_H
#define REMOTEPLUS_DEFINES_H

//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//  RemotePlusApp Defines

#define SCREENX 800
#define SCREENY 480

//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//  RemotePlusMain Defines

#define ACTIVE_NB 1

//----------------------------------------------------------------------------//
//  Defines for positioning the function buttons - need to be static
//  to workaround relative layout juumping.

#define ICONXLEFT 3
#define ICONXRIGHT 766
#define ICONYONE 35
#define ICONYTWO 200
#define ICONYTHREE 370

//----------------------------------------------------------------------------//
//  Defines for Keyboard  
//
//  The defauls is q w e r for the top menu buttons and z x c v for the bottom.
//  Users of German mapped keyboards can change the z to y here.
//  
//  The values are the uppercase decimal asci values, more information 
//  can be found at http://docs.wxwidgets.org/3.1/classwx_key_event.html

#define MENU1KEY    81  // Top 4 buttons
#define MENU2KEY    87
#define MENU3KEY    69
#define MENU4KEY    82
#define MENU5KEY    90  // Bottom 4 buttons - Y = ascii 89 for DE keyboards
#define MENU6KEY    88
#define MENU7KEY    67
#define MENU8KEY    86

#define FUNCOKKEY   79 // O
#define FUNCCANCELKEY 80  // P

//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//  RemotePlusMain : RemotePlusUARTThread Defines

#define UART_NOT_CONNECTED 0
#define UART_CONNECTED 1
#define UART_ERROR -1

//----------------------------------------------------------------------------//
// This section is defined in RemoteServer.c and MUST be a direct copy of
// the values set there.

#define GET_AXIOM_TEMP_REQUEST 0x01
#define GET_AXIOM_TEMP_RESPONSE 0x80

#define GET_AXIOM_STATUS_REQUEST 0x02
#define GET_AXIOM_STATUS_RESPONSE 0x81

#define GET_GAIN_REQUEST 0x03
#define GET_GAIN_RESPONSE 0x82

#define SET_GAIN_REQUEST 0x04
#define SET_EXPOSURE_REQUEST 0x05
#define SET_VIDEO_MODE_REQUEST 0x06
#define SET_OVERLAYS_REQUEST 0x07
#define SET_HDMI_LIVE_STREAM_REQUEST 0x08

//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//  MenuWidget Defines

// Menu names are 1 indexed, actual values are 0 indexed
#define MENU_ONE 0
#define MENU_TWO 1
#define MENU_THREE 2
#define MENU_FOUR 3

#define MENU_FIVE 4
#define MENU_SIX 5
#define MENU_SEVEN 6
#define MENU_EIGHT 7

#define FUNC_CANCEL 98
#define FUNC_OK 99

#define Rotary_CW 100
#define Rotary_CCW 101

#define NOT_CONNECTED 0
#define CONNECTED 1
#define ERROR -1

#define MENU_NOT_ACTIVE 0
#define MENU_ACTIVE 1

#define MENU_TOP 0
#define MENU_BOTTOM 1

//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//  ConnectionWidget Defines

#define UART_NOT_CONNECTED 0
#define UART_CONNECTED 1
#define UART_ERROR -1

//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//  AxiomSystemWidget Defines

#define AXIOM_NOT_STARTED 0
#define AXIOM_STARTING 1
#define AXIOM_RUNNING 99
#define AXIOM_ERROR -1

//----------------------------------------------------------------------------//
//----------------------------------------------------------------------------//
//  Temperature Defines

#define Celsius 0
#define Fahrenheit 1


#endif // REMOTEPLUS_DEFINES_H
//----------------------------------------------------------------------------//
