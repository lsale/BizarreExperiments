import bb.cascades 1.0
import bb.multimedia 1.0

Page {

    property string recordingUrl
    property bool isRecording: false
    property bool isPlaying: false

    Container {
        Button {
            text: isRecording ? "Stop" : "Record"
            onClicked: {
                if (isRecording) {
                    recorder.reset();
                    isRecording = false;
                } else {
                    recorder.record();
                    isRecording = true;
                }
            }
        }
        Button {
            id: playButton
            text: isPlaying ? "Stop" : "Play"
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
        }
    }
    attachedObjects: [
        AudioRecorder {
            id: recorder
            outputUrl: recordingUrl
            onMediaStateChanged: {
                console.log("Media state changed to: " + recorder.mediaState);
            }
        }
    ]
}
