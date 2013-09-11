import bb.cascades 1.0
import bb.multimedia 1.0
import QtMobility.sensors 1.2

Page {

    property string drumUrl
    property bool isRecording: false
    property bool isPlaying: false
    
    Container {
        background: Color.Black

        layout: DockLayout {

        }

        ImageView {
            imageSource: "asset:///guitar.png"
            onTouch: {
                if (event.isDown()) {

                    //Play the bass note at the current angle
                    app.playBass(convertAccelerometerToPitch(accelSensor.reading.y));
                }
            }
        }
        ImageView {
            imageSource: "asset:///btn_record.png"
            verticalAlignment: VerticalAlignment.Bottom
            horizontalAlignment: HorizontalAlignment.Right
            preferredWidth: 300
            onTouch: {
                if (event.isDown()) {
                
                    if (!isRecording)
                    {
                        if (isPlaying){
                            app.stopDrum();
                        }
                        recorder.record();
                    } 
                    else 
                    {
                        //stop recording
                        recorder.reset();
                    }
                    isRecording = !isRecording;
                }
            }
            scalingMethod: ScalingMethod.AspectFit
        }

        ImageView {
            imageSource: "asset:///btn_play.png"
            verticalAlignment: VerticalAlignment.Top
            horizontalAlignment: HorizontalAlignment.Right
            onTouch: {
                if (event.isDown()) {
                    if (isPlaying) {
                        app.stopDrum()
                    } else {
                        app.startDrum();
                    }
                    isPlaying = !isPlaying;
                }
            }
        }

    }
    attachedObjects: [
        Accelerometer {
            id: accelSensor
            active: true
            alwaysOn: true
        },
        AudioRecorder {
            id: recorder
            outputUrl: drumUrl
            onMediaStateChanged: {
                console.log("XXXX Media state changed to: " + recorder.mediaState);

                if (recorder.mediaState == MediaState.Unprepared) {
                    console.log("XXXX Loading the drum sound for playback");
                    app.loadDrum();
                }
            }
        }
    ]
    
    function convertAccelerometerToPitch(reading) {
        
        //Reading range is -10 to 10, convert to 0 to 2
        console.log("Reading is: "+reading);
        
        var pitch = ((reading * 0.05) + 1).toFixed(2);
        console.log("New pitch: " + pitch);
        return pitch;
    }
}
