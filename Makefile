##----------------------------------------------------------------------------##
##  Application: RemotePlus for AXIOM Cameras                                 ##
##                                                                            ##
##  Author:   Phil Kerr                                                       ##
##  Company:  Plus24 Systems Ltd.                                             ##
##  GitHub:   https://github.com/Plus24Systems/RemotePlus                     ##
##  Copyright (C) 2016 Phil Kerr - Plus24 Systems Ltd.                        ##
##                                                                            ##
##   This program is free software: you can redistribute it and/or modify     ##
##   it under the terms of the GNU General Public License as published by     ##
##   the Free Software Foundation, either version 3 of the License, or        ##
##   (at your option) any later version.                                      ##
##                                                                            ##
##   This program is distributed in the hope that it will be useful,          ##
##   but WITHOUT ANY WARRANTY; without even the implied warranty of           ##
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            ##
##   GNU General Public License for more details.                             ##
##                                                                            ##
##   You should have received a copy of the GNU General Public License        ##
##   along with this program.  If not, see <http:##www.gnu.org/licenses/>.    ##
##                                                                            ##
##                                                                            ##
##----------------------------------------------------------------------------##

CXX = $(shell wx-config --cxx)

CFLAGS=-c -O2 -Wall `wx-config --cxxflags`
LDFLAGS=`wx-config --libs` -lwiringPi

##----------------------------------------------------------------------------##

all: RemotePlus

##----------------------------------------------------------------------------##

RemotePlus: RemotePlusApp.o \
            RemotePlusMain.o \
            MenuWidget.o \
            ConnectionWidget.o \
            TemperatureWidget.o \
            AxiomSystemWidget.o
	$(CXX)  RemotePlusApp.o \
	        RemotePlusMain.o \
	        MenuWidget.o \
	        ConnectionWidget.o \
	        TemperatureWidget.o \
	        AxiomSystemWidget.o \
	        $(LDFLAGS) -o RemotePlus

##----------------------------------------------------------------------------##

RemotePlusApp.o: RemotePlusApp.cpp
	$(CXX) $(CFLAGS) RemotePlusApp.cpp
    
RemotePlusMain.o: RemotePlusMain.cpp 
	$(CXX) $(CFLAGS) RemotePlusMain.cpp
    
MenuWidget.o: MenuWidget.cpp 
	$(CXX) $(CFLAGS) MenuWidget.cpp 
    
RecordClockWidget.o: RecordClockWidget.cpp 
	$(CXX) $(CFLAGS) RecordClockWidget.cpp 
    
ConnectionWidget.o: ConnectionWidget.cpp 
	$(CXX) $(CFLAGS) ConnectionWidget.cpp 
    
TemperatureWidget.o: TemperatureWidget.cpp 
	$(CXX) $(CFLAGS) TemperatureWidget.cpp 
    
AxiomSystemWidget.o: AxiomSystemWidget.cpp
	$(CXX) $(CFLAGS) AxiomSystemWidget.cpp

##----------------------------------------------------------------------------##    
install:
	cp RemotePlus /usr/local/bin/
	mkdir -p /usr/share/icons/RemotePlus/
	cp icons/24/* /usr/share/icons/RemotePlus/

##----------------------------------------------------------------------------##    
clean:
	rm *o RemotePlus    
	
##----------------------------------------------------------------------------##
