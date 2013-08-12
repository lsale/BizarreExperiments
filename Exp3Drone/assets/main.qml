import bb.cascades 1.0

Page {
    Container {
        layout: DockLayout {

        }
        Label {
            // Localized text with the dynamic translation and locale updates support
            text: qsTr("AR Drone") + Retranslate.onLocaleOrLanguageChanged
            textStyle.base: SystemDefaults.TextStyles.BigText
        }
        Container {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Center
            Button {
                text: "Take off"
                onClicked: {
                    droneController.takeOff();
                }
            }
            Button {
                text: "Land"
                onClicked: {
                    droneController.land();
                }
            }
        }
    }
    /*attachedObjects: [
     * DroneController {
     * 
     * onConnected
     * onDebugMessage{
     * 
     * debugLabel.text =
     * }
     * }
     * ]*/
}
