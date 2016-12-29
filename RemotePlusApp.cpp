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

#include "RemotePlusApp.h"
#include "RemotePlusMain.h"

const long RemotePlusApp::ID_REMOTEPLUS = wxNewId();

//----------------------------------------------------------------------------//

RemotePlusApp::RemotePlusApp ()
{
//  NOP
}

//----------------------------------------------------------------------------//

bool RemotePlusApp::OnInit ()
{
    bool wxsOK = true; 
    wxInitAllImageHandlers ();
    
    if (wxsOK) {

    	RemotePlusMain* remoteplus = new RemotePlusMain (ID_REMOTEPLUS, "RemotePlus", wxPoint (0, 0), wxSize (SCREENX, SCREENY));
    	
        remoteplus -> Show (true);
        
        SetTopWindow (remoteplus);
    }
    
    return wxsOK;    
}

//----------------------------------------------------------------------------//
