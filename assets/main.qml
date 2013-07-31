import bb.cascades 1.0
import QtMobility.sensors 1.2

Page {
    property real pitch: 0.5
    property real heartRate: 0

    Container {

        id: mainContainer

        background: Color.create("#ffe4e4e4")
        /*bottomPadding: 50.0
         * leftPadding: 50.0
         * topPadding: 50.0
         * rightPadding: 50.0*/

        //DEBUG STUFF
        /*Container {
         * id: debugContainer
         * visible: false
         * Label {
         * id: compassHeadingLabel
         * text: "Compass heading"
         * topMargin: 100
         * rightMargin: 100
         * }
         * Label {
         * id: pitchLabel
         * text: pitch
         * leftMargin: 100
         * }
         * }*/

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
                    imageSource: "asset:///heart.png"
                    horizontalAlignment: HorizontalAlignment.Center
                    scaleX: 0.9
                    scaleY: 0.9

                    onTouch: {
                        if (event.isDown()) {
                            app.playTiger(pitch);
                        }
                    }

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

            }
            
            Container {
                id: playerContainer
                layout: DockLayout {
                    
                }
                
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1.0
                }
                ImageView {
                    imageSource: "asset:///play.png"
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
            active: true
            alwaysOn: true
            onReadingChanged: {
                heartRate = reading.azimuth;
                var newPitch = convertAzimuthToPitch(heartRate);

                if (pitch != newPitch) {

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
}
