import bb.cascades 1.0
import bb.multimedia 1.0

Page {
    property string dataUrl
    property bool isRecording: (audioRecorder.mediaState == MediaState.Started)
    property bool isPlaying: (audioPlayer.mediaState == MediaState.Started)
    
    Container {
        attachedObjects: [
            AudioRecorder {
                id: audioRecorder
                outputUrl: dataUrl + "/recording.wav"
            },
            MediaPlayer {
                id: audioPlayer
                sourceUrl: dataUrl + "/recording.wav"
            }
        ]
        Header {
            title: "Recording"
        }
        Label {
            id: recorderState
            text: "State: " + getMediaStateDescription(audioRecorder.mediaState)
        }
        Button {
            id: btnRecord
            text: (isRecording) ? "Stop" : "Record sample"
            onClicked: {
                if (isRecording) {
                    audioRecorder.reset();
                    //Must also reset the player so that the new duration is picked up
                    audioPlayer.reset();
                    audioPlayer.prepare();
                
                } else {
                    audioRecorder.record()
                }
            }
            horizontalAlignment: HorizontalAlignment.Center
        }
        
        Header {
            title: "Sample Playback"
            topMargin: 50
        }
        Label {
            text: "State: " + getMediaStateDescription(audioPlayer.mediaState)
        }
        Label {
            text: "Duration: " + audioPlayer.duration +"ms"
        }
        Button {
            text: (isPlaying) ? "Stop" : "Play"
            horizontalAlignment: HorizontalAlignment.Center
            enabled: (! isRecording)
            onClicked: (isPlaying) ? audioPlayer.stop() : audioPlayer.play()
        }
        Header{
            topMargin: 50
            title: "Guitar Playback"
        }
        Button {
            text: "Play"
            horizontalAlignment: HorizontalAlignment.Center
            onClicked: {
                
            }
        }
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
