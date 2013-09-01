import bb.cascades 1.0

Container {
    property string address
    layout: DockLayout {
        
    }
	background: Color.Yellow
	minHeight: 768
	minWidth: 1280
	Label {
	    horizontalAlignment: HorizontalAlignment.Center
	    verticalAlignment: VerticalAlignment.Center
    	text: address+"\nYOU WIN!!!"
    	multiline: true
    	textStyle.color: Color.Red
    	textStyle.fontSize: FontSize.PercentageValue
    	textStyle.fontSizeValue: 500
        textStyle.textAlign: TextAlign.Center
        animations: [
            FadeTransition {
                id: blink
                fromOpacity: 1.0
                toOpacity: 0.0
                duration: 600
                repeatCount: AnimationRepeatCount.Forever
            }
        ]
        onCreationCompleted: {
            blink.play();
        }
    }
}
