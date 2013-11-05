import bb.cascades 1.0
import bb.multimedia 1.0
import QtMobility.sensors 1.3

Page {

    property string sampleUrl
    property bool isGuitarPlaying: false
    
    property bool isSampleRecording: (audioRecorder.mediaState == MediaState.Started)
    property bool isSamplePlaying: false
    

    Container {

        attachedObjects: [
            AudioRecorder {
                id: audioRecorder
                outputUrl: sampleUrl
                onMediaStateChanged: {
                    console.log("[main.qml] AudioRecorder.onMediaStateChanged 
                    - Media state changed to: " + getMediaStateDescription(audioRecorder.mediaState));
/*                    if (audioRecorder.mediaState == MediaState.Unprepared) {
                        console.log("[main.qml] AudioRecorder.onMediaStateChanged - Loading the sample sound for playback");
                    }*/
                }
            },
            MediaPlayer {
                id: audioPlayer
                sourceUrl: sampleUrl
                repeatMode: RepeatMode.All
            },
            Accelerometer {
                id: accelSensor
                active: true
                skipDuplicates: true
            }
        ]

        Header {
            title: "Guitar"
        }
        Label {
            text: "Status: " + ((isGuitarPlaying) ? "Playing" : "Stopped")
        }
        Button {
            text: "Strum guitar"
            onTouch: {
                if (event.isDown()){
                    //app.playGuitar(1.0, 1.0);
                    app.playGuitar(convertAccelerometerToPitch(accelSensor.reading.y), convertAccelerometerToGain(accelSensor.reading.z));
                }
            }
            horizontalAlignment: HorizontalAlignment.Center
        }

        Header {
            title: "Sampler"
        }
        Label {
            text: "Status: " + ((isSampleRecording) ? "Recording" : "Stopped")
        }
        Button {
            text: (isSampleRecording) ? "Stop" : "Record sample"
            onClicked: {
                if (isSampleRecording) {
                    audioRecorder.reset();
                    app.loadSample(); //Load the sample into OpenAL
                    //audioPlayer.reset();
                    
                } else {
                    audioRecorder.record()
                }
            }
            horizontalAlignment: HorizontalAlignment.Center
        }
        CheckBox {
            id: chkLooped
            text: "Play looped?"
        }
        Button {
            text: (isSamplePlaying) ? "Stop" : "Play sample"
            horizontalAlignment: HorizontalAlignment.Center
            enabled: (!isSampleRecording)
            onClicked: {
                if (isSamplePlaying){
                    app.stopSample();
                } else {
                    
                    //audioPlayer.play();
                    
                    if (chkLooped.checked){
                        app.startSampleLoop();
                    } else {
                        app.playSample();
                    }
                }
                isSamplePlaying  = !isSamplePlaying;
            }
        }
    }

    function convertAccelerometerToGain(reading) {
        //Reading range -10 to 10, convert to 0 to 0.5

        console.log("[main.qml] convertAccelerometerToGain - accelerometer reading: " + reading);

        var gain = 0;
        if (reading > -10) {
            gain = ((reading + 10) / 40).toFixed(2);
        }
        console.log("[main.qml] convertAccelerometerToGain - gain: " + gain);
        return gain;
    }

    function convertAccelerometerToPitch(reading) {

        //Reading range is -10 to 10, convert to 0 to 2
        console.log("[main.qml] convertAccelerometerToPitch - accelerometer reading: " + reading);

        var pitch = ((reading * 0.05) + 1.25).toFixed(2);
        console.log("[main.qml] convertAccelerometerToPitch - pitch: " + pitch);
        return pitch;
    }

    function getMediaStateDescription(mediaState) {
        switch (mediaState) {
            case MediaState.Paused:
                return "Paused";
            case MediaState.Prepared:
                return "Prepared";
            case MediaState.Started:
                return "Started";
            case MediaState.Stopped:
                return "Stopped";
            case MediaState.Unprepared:
                return "Unprepared";
            default:
                return "Unknown"
        }
    }
}
