import bb.cascades 1.0

Container {
    background: Color.Transparent
    property int number
    property variant textColor: Color.White
    layout: DockLayout {
    }
    Label {
        id: counter
        text: number
        textStyle.color: textColor
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Center
        textStyle.fontSize: FontSize.PercentageValue
        textStyle.fontSizeValue: 1000
    }
    onNumberChanged: {
        if(number>7){
            textColor = Color.Green 
        }else if(number>3){
            textColor = Color.Yellow            
        }else {
            textColor = Color.Red
        }
    }
}
    