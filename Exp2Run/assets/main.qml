import bb.cascades 1.0
import CustomTimer 1.0
import bb.multimedia 1.0

NavigationPane {
    id: navigationPane
    property int counter: 3
    property bool defaultScreen: true
    property variant page
    function reset(){
        counter = 3
        appDisplayTimer.stop()
        defaultScreen = true;
        startButton.enabled = true 
    }
    Page {
        Container {
            id: mainContainer
            objectName: "mainPane"

            background: (defaultScreen) ? Color.Black : Color.Green
            // ======== SIGNAL()s ========
            
            signal startEmulatingTag(string data)
            signal updateEmulatingTag(string data)
            signal stopEmulatingTag()
            
            // ======== SLOT()s ========
            
            function tagEmulationSelectEvent() {}
            function tagEmulationReadEvent() {}
            function tagEmulationLeftEvent() {}
            
            // ======== Local functions ========
            
            function raceTagDetected(content) {
                console.log(content);
                if (content === "start" )  {
                    startRace();
                } else if (content === "lap") {
                    clock();
                }
            }
            
            function startRace(){
                appDisplayTimer.start()
                startButton.enabled = false
            }
            
            function clock(){
                if(page != null ){
                    page.lap++
                }
            }
            
            Label {
                // Localized text with the dynamic translation and locale updates support
                text: qsTr("Get ready to lose some fat") + Retranslate.onLocaleOrLanguageChanged
                textStyle.base: SystemDefaults.TextStyles.BigText
                textStyle.color: Color.White
                horizontalAlignment: HorizontalAlignment.Center
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1.0
                
                }
                multiline: true
                textStyle.textAlign: TextAlign.Center
            }
            Label {
                id: countdown
                text: counter
                textStyle.fontSize: FontSize.PercentageValue
                textStyle.fontSizeValue: 5000
                textStyle.color: Color.White
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Center
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 3.0
                
                }
                attachedObjects: [
                    MediaPlayer {
                        id: beep
                        sourceUrl: "res/beep.wav"
                    },
                    MediaPlayer {
                        id: start
                        sourceUrl: "res/go.wav"
                    }
                ]
            }
            Timer {
                id: appDisplayTimer
                interval: 1000
                onTimeout: {
                    if(counter>0) {
                        beep.play()
                        counter--
                        beep.stop()
                    }else {
                        countdown.setText("GO!")
                        defaultScreen = false
                        start.play()
                        appDisplayTimer.stop()
                        raceTimer.start()
                    }
                }
                
            }
            Timer {
                id: raceTimer
                interval: 500
                onTimeout: {
                	raceTimer.stop()
                    page = race.createObject();
                    navigationPane.push(page);
                }
                attachedObjects: ComponentDefinition {
                    id: race
                    source: "race.qml"
                }
            }
        }
        actions: [
            ActionItem {
                id: startButton
                title: "Start"
                ActionBar.placement: ActionBarPlacement.OnBar
                onTriggered: {
                    appDisplayTimer.start()
                    startButton.enabled = false 
                }
                imageSource: "asset:///res/play.png"
            },
            ActionItem {
                title: "Reset"
                ActionBar.placement: ActionBarPlacement.OnBar
                onTriggered: {
                    reset();
                }
                imageSource: "asset:///res/refresh.png"
            }
            
        ]
    }
    onPopTransitionEnded: { 
        page.destroy();
        reset();
    }
}
