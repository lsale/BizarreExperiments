import bb.cascades 1.0
import CustomTimer 1.0
import bb.multimedia 1.0

Page {
    property int counter: 10
    property bool shoot
    property int lap
    property bool victory: true
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
            enabled: victory
        }
        CustomSlider{
            id: countdown
            number: counter
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            visible: victory
        }
//        Label {
//            id: countdown
//            text: counter
//            textStyle.fontSize: FontSize.PercentageValue
//            textStyle.fontSizeValue: 5000
//            textStyle.color: Color.White
//            verticalAlignment: VerticalAlignment.Center
//            horizontalAlignment: HorizontalAlignment.Center
//        }
        Label {
            id: lapCounter
            text: "Lap: "+lap
            textStyle.fontSize: FontSize.PercentageValue
            textStyle.fontSizeValue: 500
            textStyle.color: Color.White
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            visible: victory
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
                if(counter>0){
                    counter--
                }else{
                    shoot = true 
                    siren.play()
                    resetter.start()
                    counter = 10
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
        if(lap<10){
            counter = 10
            if(!shoot)
                yay.play()
        }else{
            console.log('race over')
            mainTimer.stop()
            victory = false 
            banana.setVisible(true)
        }  
    }
    attachedObjects: [
        MediaPlayer {
            id: yay
            sourceUrl: "res/yay.wav"
        }
    ]
}
