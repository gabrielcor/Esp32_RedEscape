# ESP32-REDscape
## Introduction
ESP-32-REDscape is based on <a href="https://github.com/playfultechnology/node-redscape">Node-REDEscape</a> - but it is mostly focused on running on ESP32 processors and with a more prescriptive set of rules.
The idea behind this version is to speed up use and adoption for non-technical users.

## Example Uses
* Send messages to and from prop controllers based on ESP32.
* Create a browser-based dashboard for gamesmasters that shows the status of all puzzles in the room, providing options to remotely solve or reset them
* Start/stop a game timer, make manual adjustments, and display time remaining on an in-room display for players that can be styled to match the theme of your room
* Display pre-defined or custom hints on various devices - a monitor, LCD display, printer etc.
* Define events that are triggered after a certain amount of time, with a random chance, or on completion of certain pre-requisites
* Interface with lighting and sound systems using industry-standard communications such as DMX and MIDI, Philips Hue lighting, etc.

## Screenshots
### Dashboard
!["Example GM Dashboard"](https://github.com/playfultechnology/node-redscape/blob/master/Documentation/screenshots/dashboard_example.png)

## Features
* Free and open source - Completely free to download and use. 
* Robust - The Node-RED framework on which PropControl is built was originally developed by IBM, and now supported by the <a href="https://openjsf.org/">OpenJS Foundation</a>. It is being used by tens of thousands of users across the world, and is actively maintained and developed.  
* Flexible - The system is completely customisable and extendable, so you never have to worry about having your imagination limited by your control software. Want to make a game that streams live video? Integrate with a social media feed? No problem - there's a node for that. 
* Supports a wide range of platforms - The prop control server can be run on Windows, Linux, or Raspberry Pi. Individual prop controllers can be based on Raspberry Pi, any model of Arduino, ESP32, ESP8266, Teensy, and many more. Devices may be connected via ethernet, Wi-Fi, serial connection, and messages transferred via JSON, CSV, binary, or simple text.

## To Get Started
The <a href="https://github.com/playfultechnology/propcontrol/wiki">Wiki</a> contains a growing list of "recipes" - examples of different common functionality that you can learn from, copy, and adapt. 
Choose a recipe from the wiki sidebar and read the notes. To create a flow using that recipe, highlight and copy the JSON file to your clipboard and paste it into the import menu of your own Node-RED instance. 
