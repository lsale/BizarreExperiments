import bb.cascades 1.0
import bb.multimedia 1.0

Page {
    Container {
        //background: Color.Red
        layout: DockLayout {

        }

        verticalAlignment: VerticalAlignment.Fill
        ForeignWindowControl {
            id: videoWindowControl
            objectName: "videoWindowControl"
            windowId: "videoWindowControl"
        }
        
        
        ImageView {
            id: hornImageLeft
            imageSource: "asset:///air_horn_flipped.png"
            translationX: -880.0
        }
        ImageView {
            id: hornImageRight
            imageSource: "asset:///air_horn.png"
            horizontalAlignment: HorizontalAlignment.Right
            translationX: 880
        }
        
        
        Container {
            id: textContainer
            
            //background: Color.Blue
            topPadding: 50.0
            bottomPadding: 50.0
            leftPadding: 50.0
            rightPadding: 50.0
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            layout: DockLayout {

            }
            
            Container {
                verticalAlignment: VerticalAlignment.Bottom
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }

                    Label {
                        text: "Drone status: "
                        textStyle.color: Color.White
                    }
                    Label {
                        objectName: "droneStatusLabel"
                        text: "Loading..."
                        textStyle.color: Color.White
                    }
                    ActivityIndicator {
                        objectName: "droneActivityIndicator"
                    }
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    Label {
                        text: "GamePad status: "
                        textStyle.color: Color.White
                    }
                    Label {
                        objectName: "gamePadStatusLabel"
                        text: "Loading..."
                        textStyle.color: Color.White
                    }
                    ActivityIndicator {
                        objectName: "gamePadActivityIndicator"
                    }

                }
            }
        }
        Button {
            text: "Take off"
            onClicked: {
                drone.takeOff();
            }
        }
        Button{
            horizontalAlignment: HorizontalAlignment.Right
            text: "Land"
            onClicked: {
                drone.land();
            }
        }
        
    }
    attachedObjects: [
        MediaPlayer {
            id: hornPlayer
            sourceUrl: "asset:///air_horn_short.wav"
        },
        SequentialAnimation {
            id: hornAnim
            ParallelAnimation {
                id: slideInAnim
                TranslateTransition {
                    target: hornImageLeft
                    toX: -400
                    easingCurve: StockCurve.DoubleBounceOut
                }
                TranslateTransition {
                    target: hornImageRight
                    toX: 400
                    easingCurve: StockCurve.DoubleBounceOut
                }
            }
            ParallelAnimation {
                id: playAnim
                ScaleTransition {
                    target: hornImageLeft
                    toX: 1.2
                    duration: 800
                }
                ScaleTransition {
                    target: hornImageRight
                    toX: 1.2
                    duration: 800
                }
            }
            ParallelAnimation {
                id: slideOutAnim
                ScaleTransition {
                    target: hornImageLeft
                    toX: 1
                }
                ScaleTransition {
                    target: hornImageRight
                    toX: 1
                }
                TranslateTransition {
                    target: hornImageLeft
                    toX: -880
                }
                TranslateTransition {
                    target: hornImageRight
                    toX: 880
                }
            }
        }

    ]
    
    function playHorn()
    {
        hornPlayer.play();
        hornAnim.play();
    }
}
