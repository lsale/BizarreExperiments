import bb.cascades 1.0
import bb.multimedia 1.0

Page {
    property int dScore: 0
    property int lScore: 0
    property int maxScore: 30
    property int globalPosition: (dScore-lScore)*10
    onDScoreChanged: {
        if(dScore>maxScore)
            winnerScreen.visible = true
        leftMid.translationX = globalPosition
    }
    onLScoreChanged: {
        if(lScore>maxScore)
            winnerScreen.visible = true
        rightMid.translationX = -globalPosition
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
        Container {
            objectName: "mainContainer"
            function scoreUpdate(score, player){
                switch(player){
                    case 0:
                        dScore = score
                        break;
                    case 1:
                        lScore = score
                        break;
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
                
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                
                }
                Container {
                    id: left
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 2.0
                    }
                    Label {
                        text: "Luca"
                        textStyle.color: Color.White
                        textStyle.fontSize: FontSize.PercentageValue
                        textStyle.fontSizeValue: 200
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                
                }
                Container {
                    id: center
                    topPadding: 70.0
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 1.0
                    }
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    
                    }
                    Label {
                        text: lScore
                        textStyle.color: Color.White
                        textStyle.fontSize: FontSize.PercentageValue
                        textStyle.fontSizeValue: 400
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                    Label {
                        text: ":"
                        textStyle.color: Color.White
                        textStyle.fontSize: FontSize.PercentageValue
                        textStyle.fontSizeValue: 300
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                    Label {
                        text: dScore
                        textStyle.color: Color.White
                        textStyle.fontSize: FontSize.PercentageValue
                        textStyle.fontSizeValue: 400
                        horizontalAlignment: HorizontalAlignment.Center
                    }
                }
                Container {
                    id: right
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 2.0
                    }
                    Label {
                        text: "Don"
                        textStyle.color: Color.White
                        textStyle.fontSize: FontSize.PercentageValue
                        textStyle.fontSizeValue: 200
                        horizontalAlignment: HorizontalAlignment.Center
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
        attachedObjects: [
            MediaPlayer{
                id: ohBoy
                sourceUrl: "asset:///ohmygod.mp3"
                
            }
        ]
    }
    
}
