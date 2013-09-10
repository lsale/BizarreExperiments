import bb.cascades 1.0
import QtMobility.sensors 1.2

Page {
    property real pitch: 0.5
    property real heartRate: 0
    property bool isPlaying: false
    property bool isConnectedToHRMonitor: false
    property int targetHeartRate: 140

    Container {

        id: mainContainer

        background: Color.create("#ffe4e4e4")
        
        Container {
            id: topContainer

            //background: Color.Blue
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight

            }
            horizontalAlignment: HorizontalAlignment.Fill
            
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
                    imageSource: (isConnectedToHRMonitor) ? "asset:///images/heart_red.png" : "asset:///images/heart_grey.png"
                    horizontalAlignment: HorizontalAlignment.Center
                    scaleX: 0.9
                    scaleY: 0.9

                    animations: [
                        SequentialAnimation {
                            id: pulseAnim
                            ScaleTransition {
                                fromX: 0.9
                                toX: 0.95
                                fromY: 0.9
                                toY: 0.95
                                duration: 100
                            }
                            ScaleTransition {
                                fromX: 0.95
                                toX: 0.9
                                fromY: 0.95
                                toY: 0.9
                                duration: 100
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
                        visible: isConnectedToHRMonitor
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
                        text: targetHeartRate
                        visible: isConnectedToHRMonitor
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
                    imageSource: "asset:///images/play.png"
                    scaleX: 0.85
                    scaleY: 0.85
                    horizontalAlignment: HorizontalAlignment.Center
                }
                Label {
                    id: pitchLabel
                    visible: isPlaying
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
        Container {
            //background: Color.Green
            topPadding: 20
            leftPadding: 20
            rightPadding: 20
            bottomPadding: 20
            
            visible: isPlaying && isConnectedToHRMonitor
            
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            
            layoutProperties: StackLayoutProperties {
                spaceQuota: 1
            }
            Button {
                text: "Decrease target"
                verticalAlignment: VerticalAlignment.Center
                onClicked: {
                    targetHeartRate--;
                }
            }
            
            Label {
                text: (heartRate < targetHeartRate) ? "Too slow" : "Good job!"
                textStyle.base: SystemDefaults.TextStyles.BigText
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1
                }
                textStyle.textAlign: TextAlign.Center
            }
            Button {
                text: "Increase target"
                verticalAlignment: VerticalAlignment.Center    
                onClicked: {
                    targetHeartRate++;
                }        
            }
        
        }
    }
        
    function convertHeartRateToPitch(rate){
        
        return ((1/targetHeartRate) * rate).toFixed(2);
    }
    
    
    function logHeartRate(rate) {
        console.log("XXXX QML has received rate:" + rate);
        if (!isConnectedToHRMonitor) isConnectedToHRMonitor = true;
        
        pulseAnim.play();
        heartRate = rate;
        
        //Calculate the music pitch
        var newPitch = convertHeartRateToPitch(rate);
                
        console.log("XXXX New pitch is: "+newPitch)
        
        if (newPitch != pitch && isPlaying){
            console.log("Changing pitch");
            pitch = newPitch;
            app.changeTigerPitch(pitch);        
        }
      
    }
}
