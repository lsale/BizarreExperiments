import bb.cascades 1.0
import bb.multimedia 1.0
import QtMobility.sensors 1.3

Page {

    property string drumUrl
    
    Container {

        background: Color.Black
        layout: DockLayout {

        }

        ImageView {
            imageSource: "asset:///guitar.png"
            onTouch: {
                if (event.isDown()) {

                    //Play the bass note at the current angle
                    app.playGuitar(convertAccelerometerToPitch(accelSensor.reading.y), convertAccelerometerToGain(accelSensor.reading.z));
                }
            }
        }
    }
    attachedObjects: [
        Accelerometer {
            id: accelSensor
            active: true
        }
    ]
    
    function convertAccelerometerToGain(reading)
    {
        //Reading range -10 to 10, convert to 0 to 0.5
        
        console.log("[main.qml] convertAccelerometerToGain - accelerometer reading: "+reading);
        
        var gain=0;        
        if (reading > -10){
            gain = ((reading + 10) / 20).toFixed(2);
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
