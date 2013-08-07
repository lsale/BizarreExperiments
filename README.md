The Bizarre App Experiments
==================

A series of 'app experiments' brought to you by Don Turner and Luca Sale of the BlackBerry Developer Relations team.

Experiment 1 – Can you control the music with your heart rate?
---
Person on treadmill is wearing a Bluetooth heart rate monitor. The app listens to the heart rate monitor and plays "Eye of the tiger" at a speed relative to their heart rate. The person must play the music at the correct speed by increasing their heart rate to 140 bpm by running on the treadmill.

Technologies: Bluetooth, OpenAL, Audio playback

Experiment 2 – Can you run 10 laps without running out of time?
---
There are 2 cones approximately 10m apart. The person has to run to one cone and back again to complete a lap. The person is carrying an NFC card. To complete a lap they must tap it onto a BlackBerry mounted on one of the cones. The app listens for NFC 'taps' and shows the person's lap times, as well as decreasing the target lap time. If the person drops below the target they are fired at with Nerf guns. 

Technologies: NFC

Experiment 3 – Has anyone fallen asleep?
---
An AR drone is flown across the audience to find members who have fallen asleep. The AR drone is controlled by a Bluetooth Gamepad. The video feed from the AR drone is shown on the big screen. The app analyses the video feed to detect people with their eyes shut. When found they are fired upon with a Nerf gun.

Technologies: Bluetooth, Gamepad API, WiFi, Augmented Reality
 
Experiment 4 – Can we compose a song using sounds from the audience?
---
Audience members are asked to make sounds (e.g. hi-hat, snare etc) which are recorded using a BlackBerry App. Each sound's pitch and repeat rate is controlled by the accelerometer x and y axis, so by moving the device we change the sound. By manipulating each sound we should be able to make a song.

Technologies: Microphone, Audio playback, OpenAL, Accelerometer

Experiment 5 – Can you avoid being squirted by a water cannon?
---
A water cannon is placed in the middle of the stage. It is controlled by a BlackBerry 10 app (via Arduino). The audience is split into 2 halves: left and right. Audience members connect to a WiFi router and then direct their browser to the BlackBerry IP. Each member can then 'vote' for the other side to be fired upon. The first side to reach 100 votes will be squirted.

Technologies: Networking, WiFi, Arduino
