# WMM
Wrist Motion Monitor, senior design project for Mississippi State University

When making commits, pulling, or pushing changes, either use the git command line (https://git-scm.com/download/win) or github desktop (https://desktop.github.com/) 
This removes the file size limit when adding files in browser! 

guide for using git command line (https://www.youtube.com/watch?v=SWYqp7iY_Tc) 

guide for using github desktop (https://www.youtube.com/watch?v=77W2JSL7-r8) 


# Flutter/Unity/Bluetooth demo setup (currently only usable on android) 

* Clone the repository (either download the .zip file from github in browser or clone using github desktop/git command line) 
* Load WMM-master\arduino\BLE\BLE_server_multiconnect\BLE_server_multiconnect.ino onto ESP32 
* Open WMM-master\app\flutter-unity-view-widget-master\example on android studio
* Run `<flutter pub get>` on android studio terminal (should get a pop up when you first open the project on android studio asking to install dependecies, this works as well) 
* Compile the project onto your phone, should get a screen that looks like this
* <img src="Tutorial-1.jpg" width="225" height="475">
* Select Bluetooth Setup
* Tap on the search icon in bottom right corner
* <img src="Tutorial-2.jpg" width="225" height="475">
* Connect to the ESP32
* <img src="Tutorial-3.jpg" width="225" height="475">
* Tap the edit symbol adjacent to MTU size option, should change it from 20 bytes to 223 bytes
* Tap the button with the raised hand, this should open the unity project 
* <img src="Tutorial-4.jpg" width="225" height="475">
* Press the "Press to start demo!" button at the bottom of the screen 
* Should be simulating the IMU's orientation! 
