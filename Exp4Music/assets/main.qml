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
                    app.playGuitar(1.0, 1.0);
                    //app.playGuitar(convertAccelerometerToPitch(accelSensor.reading.y), convertAccelerometerToGain(accelSensor.reading.z));
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
                console.log("[main.qml] AudioRecorder.onMediaStateChanged - Media state changed to: " + recorder.mediaState);

                if (recorder.mediaState == MediaState.Unprepared) {
                    console.log("[main.qml] AudioRecorder.onMediaStateChanged - Loading the drum sound for playback");
                    app.loadDrum();
                }
            }
        }
    ]
    
    function convertAccelerometerToGain(reading)
    {
        //Reading range -10 to 10, convert to 0 to 0.5
        
        console.log("[main.qml] convertAccelerometerToGain - accelerometer reading: "+reading);
        
        var gain=0;        
        if (reading > -10){
            gain = ((reading + 10) / 40).toFixed(2);
        }
        console.log("[main.qml] convertAccelerometerToGain - gain: "+gain);
        return gain;
    }
    
    function convertAccelerometerToPitch(reading) {
        
        //Reading range is -10 to 10, convert to 0 to 2
        console.log("[main.qml] convertAccelerometerToPitch - accelerometer reading: "+reading);

        var pitch = ((reading * 0.05) + 1.25).toFixed(2);
        console.log("[main.qml] convertAccelerometerToPitch - pitch: " + pitch);
        return pitch;
    }
}
