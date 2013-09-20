import bb.cascades 1.0
import CustomTimer 1.0
import bb.multimedia 1.0

Page {
    property int currentTimeRemaining: 10
    property int targetLapTime: 10
    property bool shoot
    property int lap
    property bool inProgress: true
    Container {
        background: (shoot) ? Color.Red : Color.Black
        Label {
            id: fireNote
            text: qsTr("FIRE!")
            visible: shoot
            textStyle.fontSize: FontSize.PercentageValue
            textStyle.fontSizeValue: 5000
            textStyle.color: Color.White
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            enabled: inProgress
        }
        CustomCountdown{
            id: countdown
            number: currentTimeRemaining
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            visible: inProgress
            isFireScreenOn: shoot
        }
        Label {
            id: lapCounter
            text: "Lap: "+lap
            textStyle.fontSize: FontSize.PercentageValue
            textStyle.fontSizeValue: 500
            textStyle.color: Color.White
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            visible: inProgress
        }
        Container {
            id: banana
            visible: false 
            Label {
                text: "Victory!!!"
                textStyle.fontSize: FontSize.PercentageValue
                textStyle.fontSizeValue: 500
                textStyle.color: Color.Yellow
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
            }
            ImageView {
                imageSource: "asset:///res/banana.gif"
                attachedObjects: [
                    MediaPlayer {
                        id: peanutButter
                        sourceUrl: "res/peanutbutter.mp3"
                    }
                ]
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
            
            }
            onVisibleChanged: {
                if(visible)
                    peanutButter.play()
            }
        }
        Timer {
            id: mainTimer
            interval: 1000
            onTimeout: {
                if(currentTimeRemaining>0){
                    currentTimeRemaining--
                }else{
                    shoot = true 
                    siren.play()
                    resetter.start()
                    currentTimeRemaining = targetLapTime;
                }
            }
            attachedObjects: [
                MediaPlayer {
                    id: siren
                    sourceUrl: "res/siren.wav"
                }
            ]
        }
        Timer {
            id: resetter
            interval: 3000
            onTimeout: {
                shoot = false 
                resetter.stop()
            }
        }
        onCreationCompleted: {
            mainTimer.start()
        }
    }
    onLapChanged: {
        if(lap == 10){
            console.log('race over')
            mainTimer.stop()
            bennyHill.stop()
            inProgress = false 
            banana.setVisible(true)
        } else {
            if(targetLapTime > 5){
                targetLapTime--
            }
            currentTimeRemaining = targetLapTime
            if(!shoot){
                yay.play()
            }
        }  
    }
    attachedObjects: [
        MediaPlayer {
            id: yay
            sourceUrl: "res/yay.wav"
        },
        MediaPlayer {
            id: bennyHill
            sourceUrl: "res/benny.mp3"
            repeatMode: RepeatMode.All
        }
    ]
    onCreationCompleted: {
        bennyHill.play()
    }
}
