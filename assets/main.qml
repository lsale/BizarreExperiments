import bb.cascades 1.0
import QtMobility.sensors 1.2

Page {
    property real pitch: 0.5
    property real heartRate: 0
    property bool isPlaying: false

    Container {

        id: mainContainer

        background: Color.create("#ffe4e4e4")

        Container {
            id: topContainer

            //background: Color.Green
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight

            }
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill

            //Heart rate
            Container {

                id: heartContainer
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1.0
                }
                layout: DockLayout {

                }

                horizontalAlignment: HorizontalAlignment.Center

                ImageView {
                    id: heartImage
                    imageSource: "asset:///images/heart_grey.png"
                    horizontalAlignment: HorizontalAlignment.Center
                    scaleX: 0.9
                    scaleY: 0.9

                    animations: [
                        SequentialAnimation {
                            id: pulseAnim
                            ScaleTransition {
                                fromX: 0.9
                                toX: 1.0
                                fromY: 0.9
                                toY: 1.0
                            }
                            ScaleTransition {
                                fromX: 1.0
                                toX: 0.9
                                fromY: 1.0
                                toY: 0.9
                            }
                        }
                    ]
                }
                Container {
                    //background: Color.Green
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center

                    Label {
                        id: heartRateLabel
                        text: heartRate.toFixed(0)
                        textStyle.color: Color.Red
                        textStyle.fontSizeValue: 50.0
                        textStyle.fontWeight: FontWeight.Bold
                        textStyle.fontSize: FontSize.PointValue
                        textStyle.textAlign: TextAlign.Center
                        textStyle.lineHeight: 0
                        preferredHeight: 1.0
                        verticalAlignment: VerticalAlignment.Top
                        horizontalAlignment: HorizontalAlignment.Center

                    }
                    Label {
                        text: "120"
                        textStyle.fontSize: FontSize.PointValue
                        textStyle.fontSizeValue: 20.0
                        textStyle.color: Color.Green
                        horizontalAlignment: HorizontalAlignment.Center

                    }

                }
                
                onTouch: {
                    if (touch.isDown()){
                        
                        //Connect to the heart rate monitor
                        app.connectToHRMonitor();
                    }
                }

            }

            Container {
                id: playerContainer
                layout: DockLayout {

                }

                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1.0
                }
                ImageView {
                    imageSource: "asset:///images/play.png"
                    scaleX: 0.85
                    scaleY: 0.85
                    horizontalAlignment: HorizontalAlignment.Center
                }
                Label {
                    id: pitchLabel
                    text: ((pitch - 1) * 100).toFixed(0) + "%"
                    textStyle.color: (pitch < 1) ? Color.Red : Color.Green
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    textStyle.fontSize: FontSize.XXLarge
                    textStyle.fontWeight: FontWeight.Bold
                    translationX: 25.0
                }

                onTouch: {
                    
                    console.log("player container received touch event");
                    
                    if (event.isDown()) {
                        if (isPlaying) {
                            app.stopTiger();
                        } else {
                            app.playTiger(pitch);
                        }
                        isPlaying = !isPlaying;
                    }
                }
            }
        }

        Label {
            text: (heartRate < 120) ? "Too slow" : "Slow down!"
            textStyle.base: SystemDefaults.TextStyles.BigText
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom
        }
    }

    attachedObjects: [
        Compass {
            id: compass
            active: false
            alwaysOn: false
            onReadingChanged: {
                heartRate = reading.azimuth;
                var newPitch = convertAzimuthToPitch(heartRate);

                if (pitch != newPitch && isPlaying) {

                    pitch = newPitch;
                    console.log("Changing pitch");
                    app.changeTigerPitch(pitch);
                    pulseAnim.play();
                }
            }
        }
    ]

    function convertAzimuthToPitch(azimuth) {
        var pitch = (azimuth * (1.5 / 360)) + 0.5;
        return pitch.toFixed(2);
    }
    
    
    function logHeartRate(rate) {
        console.log("XXXX QML has received rate:" + rate);
        
        heartRate = rate;
        var newPitch = convertAzimuthToPitch(rate);
        
        console.log("XXXX New pitch is: "+newPitch)
        
        if (newPitch != pitch && isPlaying){
            pitch = newPitch;
            console.log("Changing pitch");
            app.changeTigerPitch(pitch);        
            pulseAnim.play();
        }
      
    }
}
