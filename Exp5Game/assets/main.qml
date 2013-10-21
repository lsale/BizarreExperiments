import bb.cascades 1.0
import bb.multimedia 1.0

Page {
    property bool hasGameStarted
    property int donScore: 0
    property int lucaScore: 0
    property int maxScore: 100
    property string deviceIPAddress 
    onDonScoreChanged: {
        if(donScore>maxScore)
            winnerScreen.visible = true
        leftMid.translationX = donScore
    }
    onLucaScoreChanged: {
        if(lucaScore>maxScore)
            winnerScreen.visible = true
        rightMid.translationX = -lucaScore
    }
    Container {
        id: game
        Winner {
        	id: winnerScreen
        	visible: false
        	onVisibleChanged: {
             if(visible)
             	ohBoy.play()
         }
        }
        Container{
            id: startSplash
            minHeight: 720
            minWidth: 720
            preferredWidth: mainApp.getDisplayHeight()
            preferredHeight: mainApp.getDisplayWidth()
            
            background: Color.Black
            opacity: 0.5
            layout: DockLayout {
            }
            Label {
                text: "Get ready!\nConnect to "
                multiline: true
                textStyle.fontSize: FontSize.PercentageValue
                textStyle.fontSizeValue: 300
                textStyle.color: Color.White
                textStyle.textAlign: TextAlign.Center
                horizontalAlignment: HorizontalAlignment.Center

            }
            Label {
                text: "http://"+deviceIPAddress+":1337/"
                multiline: true
                textStyle.fontSize: FontSize.PercentageValue
                textStyle.fontSizeValue: 300
                textStyle.color: Color.Green
                textStyle.textAlign: TextAlign.Center
                horizontalAlignment: HorizontalAlignment.Center
                verticalAlignment: VerticalAlignment.Center
            
            }
            Button {
                text: "Start!"
                verticalAlignment: VerticalAlignment.Bottom
                horizontalAlignment: HorizontalAlignment.Center
                onClicked: {
                    hasGameStarted = mainApp.startGame()
                    startSplash.setVisible(false)
                }
            }
        }
        Container {
            objectName: "mainContainer"
            function scoreUpdate(score, player){
                if(hasGameStarted){
                    switch(player){
                        case 0:
                            donScore = score
                            break;
                        case 1:
                            lucaScore = score
                            break;
                    }
                }
            }
            function updatePlayer(player){
                winnerScreen.address = player
            }
            layout: DockLayout {
            }
            
            Container {
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                ImageView {
                    imageSource: "asset:///bgjungle.png"
                }
            }
            Container {
                //background: Color.Red
                horizontalAlignment: HorizontalAlignment.Fill
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
                }
                Container {
                    //background: Color.Blue
                    id: left
                    horizontalAlignment: HorizontalAlignment.Fill
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight

                    }
                    Label {
                        text: "Luca"
                        textStyle.color: Color.White
                        textStyle.fontSize: FontSize.PercentageValue
                        textStyle.fontSizeValue: 200
                        horizontalAlignment: HorizontalAlignment.Center
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1
                        }
                        textStyle.textAlign: TextAlign.Center
                    }
                    Label {
                        text: "Don"
                        textStyle.color: Color.White
                        textStyle.fontSize: FontSize.PercentageValue
                        textStyle.fontSizeValue: 200
                        
                        horizontalAlignment: HorizontalAlignment.Center
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1
                        }
                        textStyle.textAlign: TextAlign.Center
                    }
                    
                }
                Container {
                    //background: Color.Yellow
                    id: center
                    horizontalAlignment: HorizontalAlignment.Fill
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    Label {
                        text: lucaScore
                        textStyle.color: Color.White
                        textStyle.fontSize: FontSize.PercentageValue
                        textStyle.fontSizeValue: 400
                        horizontalAlignment: HorizontalAlignment.Fill
                        textStyle.textAlign: TextAlign.Right
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1
                        }
                    }
                    Label {
                        text: ":"
                        textStyle.color: Color.White
                        textStyle.fontSize: FontSize.PercentageValue
                        textStyle.fontSizeValue: 300
                        textStyle.textAlign: TextAlign.Center
                        horizontalAlignment: HorizontalAlignment.Center
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: -1
                        }
                    }
                    Label {
                        text: donScore
                        textStyle.color: Color.White
                        textStyle.fontSize: FontSize.PercentageValue
                        textStyle.fontSizeValue: 400
                        horizontalAlignment: HorizontalAlignment.Center
                        layoutProperties: StackLayoutProperties {
                            spaceQuota: 1
                        }
                    }
                }
            }
            Container {
                id: botContainer
                verticalAlignment: VerticalAlignment.Bottom
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                
                }
                bottomPadding: 100.0
                clipContentToBounds: false
                Container {
                    id: leftMid
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 2.0
                    }
                    ImageView {
                        imageSource: "asset:///face_monkey.png"
                        horizontalAlignment: HorizontalAlignment.Center
                    
                    }
                
                }
                Container {
                    id: centerMid
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1.0
                    }
                    clipContentToBounds: false
                    ImageView {
                        id: croc
                        imageSource: "asset:///croc_right.png"
                        loadEffect: ImageViewLoadEffect.None
                        onTranslationXChanging: {
                            if(translationX > 29)
                                croc.imageSource = "asset:///croc_left.png"
                            else if(translationX < -29)
                                croc.imageSource = "asset:///croc_right.png"
                        }
                        animations: [
                            ParallelAnimation {
                                RotateTransition {
                                    fromAngleZ: -5
                                    toAngleZ: 5
                                    duration: 500
                                    repeatCount: AnimationRepeatCount.Forever
                                }
                                SequentialAnimation {
                                    TranslateTransition {
                                        fromX: -30
                                        toX: 30
                                        duration: 3000
                                        easingCurve: StockCurve.Linear
                                    }
                                    TranslateTransition {
                                        fromX: 30
                                        toX: -30
                                        duration: 3000
                                        easingCurve: StockCurve.Linear
                                    }
                                    repeatCount: AnimationRepeatCount.Forever
                                }
                                
                                onCreationCompleted: {
                                    play()
                                } 
                            }
                        ]
                    }
                }
                Container {
                    id: rightMid
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 2.0
                    }
                    ImageView {
                        imageSource: "asset:///cheer_bear.png"
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                }
            }
        }
        onCreationCompleted: {
            deviceIPAddress = mainApp.getIPAddress();
        }
        attachedObjects: [
            MediaPlayer{
                id: ohBoy
                sourceUrl: "asset:///ohmygod.mp3"
                
            }
        ]
    }
    
}
