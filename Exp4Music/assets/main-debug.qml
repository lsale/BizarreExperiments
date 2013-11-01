import bb.cascades 1.0
import bb.multimedia 1.0
import QtMobility.sensors 1.2

Page {

    property string sampleUrl
    property bool isRecording: (audioRecorder.mediaState == MediaState.Started)
    property bool isPlaying: false

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
            Accelerometer {
                id: accelSensor
                active: true
                alwaysOn: true
            }
        ]

        Header {
            title: "Guitar"
        }
        Label {
            text: "Status: " + ((isPlaying) ? "Playing" : "Stopped")
        }
        Button {
            text: "Strum guitar"
            onClicked: //Play the bass note at the current angle
            app.playGuitar(1.0, 1.0);
            //app.playGuitar(convertAccelerometerToPitch(accelSensor.reading.y), convertAccelerometerToGain(accelSensor.reading.z));
            horizontalAlignment: HorizontalAlignment.Center
        }

        Header {
            title: "Sampler"
        }
        Label {
            text: "Status: " + ((isRecording) ? "Recording" : "Stopped")
        }
        Button {
            text: (isRecording) ? "Stop" : "Record sample"
            onClicked: {
                if (isRecording) {
                    audioRecorder.reset();
                    app.loadSample(); //Load the sample into OpenAL
                } else {
                    audioRecorder.record()
                }
            }
            horizontalAlignment: HorizontalAlignment.Center
        }
        CheckBox {
            text: "Play looped?"
        }
        Button {
            text: (isPlaying) ? "Stop" : "Play sample"
            horizontalAlignment: HorizontalAlignment.Center
            enabled: (! isRecording)
            onClicked: {
                //(isPlaying) ? audioPlayer.stop() : audioPlayer.play()
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
