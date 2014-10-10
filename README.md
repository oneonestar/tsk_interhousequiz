# README #

## Introduction ##

This is the repository of Inter-house quiz competition system, originally developed for SKH Tang Shiu Kin Secondary School of Hong Kong by OneOne Star, Steven Chien, Charlie Shum and Alfred Tai.
This README would normally document whatever steps are necessary to get your application up and running.

## Summary ##

The system consists of four major components: the database, event driven web server, main quiz server and buzzing system.

### Database ###

A SQLite or Redis database is used for storage of questions and answers as well of scores obtained by participants.

### Event-driven web server ###

A python web server utilizing Twisted library is used to receive instruction from quiz server and push changes to client web browsers.

### Quiz Server ###

A single threaded, non-blocking server written in C is used to act as a centralized server to store states of the game and communicate with various sub systems. The server uses libevent for buffering and create non-blocking sockets, SQLite API for database communication.

### User Interface App ###

The App acts as the control panel for the game maker where he can:

- Assign scores to participants
- Display questions
- Display answers
- Control the display of elements in web page
- Initiate and stop buzzing

 All by communicating directly with the quiz server. The application is written in Java and undergoing transition to become an Android App.

### Buzzing System ###

A buzzing system consists of physical buttons where the first player who pressed the button gets to answer the question. The system consists of a Arduino Uno with program written in C++ and Raspberry Pi where communication between quiz server and Arduino is being bridged by program written in Python. The buzzing system is being controlled by the User Interface App.

## Version ##

The system was initially released in fall 2013.

## Setup ##

Sub system of the system can be started and restarted individually. In case of the buzzing system, it is started alone and connected to the network where the quiz server is hosted.

### Start up ###
1. Web Server

Enter the directory where the server exists. Start the server by issuing the command as root: python server.py. Make sure that port 80 is not occupied by other program(i.e. Apache).

2. Database (redis)

Enter the direction of the database server, Start the server by ./redis-server. Make sure the database dump file exists in the same folder.

3. Buzzing system

First make sure the Arduino is properly connected to the Raspbery Pi and powered. Execute the python program on Raspberry Pi and it will start waiting for connection.

4. Quiz Server

The Quiz Server should be started when all sub systems are running. Execute by ./quiz_server [IP addr of web server] [IP addr of the buzzing system(raspberry pi)]. The quiz server communicates with the web server through port 8889 and buzzing server through 8888.

5. User Control Panel

The control panel is starting by inputting the IP address of quiz server. The default port of communication is 9000. 

When all the above are started properly, the system should be ready to go.

## Configuration# #

The system occupies port 80, 8888, 8889 and 9000. Please make sure they are all open and not occupied on both sides.

## Dependencies ##

The system uses the below libraries:

- libevent
- SQLite
- Ncurses
- GLib
- Various Linux system libraries

It is assumed the system will be used on Linux/UNIX platform.

## Database setup ##

The SQLite database for scores will be initialized if not exist or else the existing database will be used and scores will be automatically loaded upon initialization.

The SQLite database for questions should exist prior to initialization.

## Compilation ##

Compile the quiz server by executing make in the main folder. Make sure the aforementioned dependencies are installed properly.

## Credit ##

- OneOne Star: Web Server and database
- Steven Chien: Quiz Server, SQLite database embedding and software part of buzzing system
- Charlie Shum: Java user control panel/Android control panel
- Alfred Tai: Hardware part of buzzing System

## License ##

Copyright 2013, 2014 Star Poon, Steven Chen, Charlie Shum, Alfred Tai.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.