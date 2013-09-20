Experiment 3 – How can we control an AR drone using a GamePad?
===
An AR drone is flown across the audience to find members who have fallen asleep. The AR drone is controlled by a Bluetooth Gamepad. The video feed from the AR drone is shown on the big screen.

Technologies: Bluetooth, Gamepad API, WiFi, Augmented Reality (kind of)

Controlling the Drone
---
The Drone we use is a Parrot AR Drone. The developer site is here: https://projects.ardrone.org/. Some things you should know about the Drone: 

1. The Drone acts as a wifi access point. Its IP address is always 192.168.1.1
2. To control the Drone you send UDP datagrams to it on port 5556. These commands are known as AT commands.
3. Before you can send any movement commands (like take off), you must initialise the Drone
4. If the Drone's LEDs are red it means that the Drone is in emergency cut out mode and must be sent a reset command before it'll respond to anything else
5. The Drone expects to receive commands every 30ms, even if the command is "I'm still here, don't do anything"
6. If the Drone doesn't receive a command within 2 seconds it will cut out as it thinks that noone is in control (which would be a bad thing)

Decoding the video stream
---
TBC

Gamepad events
---
TBC

Project structure
---
TBC

Dependent libraries
---
TBC - ffmpeg, x264  





Useful links/sample code
---
Controlling the AR Drone during flight: https://www.robotappstore.com/Knowledge-Base/How-to-Control-ARDrone-Movements-During-Flight/99.html

Using ffmpeg to decode the drone video feed: http://ardrone-ailab-u-tokyo.blogspot.co.uk/2012/07/212-ardrone-20-video-decording-ffmpeg.html

BlackBerry 10 Screen API: https://developer.blackberry.com/native/documentation/core/com.qnx.doc.screen/topic/manual/cscreen_api_components.html

Decoding the drone video stream without using the SDK: https://projects.ardrone.org/boards/1/topics/show/4282

OpenCV and BlackBerry: http://supportforums.blackberry.com/t5/Native-Development/OpenCV-sample-project-and-troubles/td-p/2146603/page/2

FFMPEG for BlackBerry 10: https://github.com/MartinMReed/libffbb

Compiling the ARDrone SDK for Android on OSX: https://projects.ardrone.org/boards/1/topics/show/5568#message-5569

YADrone (Java based): https://github.com/MahatmaX/YADrone

CuteDrone: https://code.google.com/p/cutedrone/

CV Drone: https://github.com/puku0x/cvdrone

Qt project using Drone and OpenCV: https://code.google.com/p/firefleye-uav-systems/source/browse/#git%2FMainController%2FVideoProcessing

Nokia AR Drone Qt project: https://projects.developer.nokia.com/ardrone/

Code for getting screen_context_t from Cascades app (awesome code from Sean McVeigh): https://github.com/blackberry/Presentations/blob/master/2013-BlackBerryJam-Americas/JAM40/Heartbeat/src/applicationui.cpp

BPS tutorial: https://developer.blackberry.com/cascades/documentation/dev/bps/bps_tutorial.html

Gamepad: https://github.com/blackberry/NDK-Samples/tree/master/Gamepad

VideoWindow: https://github.com/blackberry/NDK-Samples/tree/master/VideoWindow

Drone API: https://projects.ardrone.org/projects/show/ardrone-api

