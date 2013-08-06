import bb.cascades 1.0
import bb.multimedia 1.0
import QtMobility.sensors 1.2

Page {

    property real displayWidth: 720
    property real displayHeight: 1280

    property string recordingUrl
    property bool isRecording: false
    property bool isPlaying: false

    Container {
        layout: DockLayout {

        }
        Container {
            id: pitchDisplayContainer
            background: Color.Green
            preferredHeight: 200
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Bottom
        }
        Container {

            id: controlContainer
            //background: Color.Gray
            preferredHeight: (displayWidth / 3) + 40
            leftPadding: 20.0
            topPadding: 20.0
            rightPadding: 20.0
            bottomPadding: 20.0
            
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            
            ImageButton {
                id: toneButton
                defaultImageSource: "asset:///btn_note.png"
                onClicked: {
                    app.playTone();
                }
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1.0
                }
            }
            ImageButton {
                id: recordButton
                defaultImageSource: isRecording ? "asset:///btn_stop.png" : "asset:///btn_record.png"
                
                onClicked: {
                    if (isRecording) {
                        recorder.reset();
                        isRecording = false;
                    } else {
                        recorder.record();
                        isRecording = true;
                    }
                }
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1.0
                }
            }
            ImageButton {
                id: playButton
                defaultImageSource: isPlaying ? "asset:///btn_stop.png" : "asset:///btn_play.png"

				onClicked: {
                    if (isPlaying) {
                        console.log("Stopping sound");
                        app.stopSound();
                    } else {
                        console.log("Playing sound");
                        app.playSound();
                    }
                    isPlaying = !isPlaying;
                }
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1.0
                }
            }
        }
        
        Label {
            id: xReading
        }
        Label {
            objectName: "debugLabel"
            text: ""
            verticalAlignment: VerticalAlignment.Bottom
        }
        attachedObjects: [
            LayoutUpdateHandler {
                onLayoutFrameChanged: {
                    
                    displayWidth = layoutFrame.width;
                    displayHeight = layoutFrame.height;
                    
                    console.log("Display: "+displayWidth+"x"+displayHeight);
                }
            }   
        ]
        
    }
    attachedObjects: [
        AudioRecorder {
            id: recorder
            outputUrl: recordingUrl
            onMediaStateChanged: {
                console.log("XXXX Media state changed to: " + recorder.mediaState);

                if (recorder.mediaState == MediaState.Unprepared) {
                    console.log("XXXX Loading the sound for playback");
                    app.loadSound();
                }
            }
        },
        MediaPlayer {
            id: player
            sourceUrl: recordingUrl
            repeatMode: RepeatMode.All
        },
        Accelerometer {
            id: accelSensor
            active: true
            alwaysOn: true
            //skipDuplicates: true
            
            onReadingChanged: {
                //console.log("Accelerometer reading changed to: " + reading.x);
                xReading.text = reading.x.toFixed(2);
                
                pitchDisplayContainer.preferredHeight = convertAccelerometerToHeight(reading.x.toFixed(2));
                
                if (isPlaying) {
                    app.setPitch(convertAccelerometerToPitch(reading.x.toFixed(2)));
                }
            }
        }
    ]

    function convertAccelerometerToHeight(reading){
    	
    	//-10 = 0, 10 = displayHeight
        var reading = parseFloat(reading);
        if (reading < -10) reading = -10;
        if (reading > 10) reading = 10;
        
        var newHeight  = (parseFloat(reading)+10.0)*64;
    	return newHeight.toFixed(0);
    }
    function convertAccelerometerToPitch(reading) {

        //Reading range is -10 to 10, convert to 0 to 2
        var pitch = ((reading * 0.1) + 1).toFixed(2);
        console.log("New pitch: " + pitch);
        return pitch;
    }
}
