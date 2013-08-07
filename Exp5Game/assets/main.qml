import bb.cascades 1.0

Page {
    Container {
        layout: DockLayout {
        }
        ImageView {
            imageSource: "images/background.jpg"
        }
        Container {
            layout: DockLayout {
            }
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                objectName: "teamAPullCount"
                text: "0"
                textStyle.fontSize: FontSize.XXLarge
                textStyle.fontWeight: FontWeight.Bold
                textStyle.color: Color.White
            }
            Label {
                objectName: "teamBPullCount"
                text: "0"
                textStyle.fontSize: FontSize.XXLarge
                textStyle.fontWeight: FontWeight.Bold
                horizontalAlignment: HorizontalAlignment.Right
                textStyle.color: Color.White
            }
            ImageView {
                imageSource: "images/rope.png"
                verticalAlignment: VerticalAlignment.Center
                objectName: "rope"
            }
            Container {
                layout: DockLayout {
                }
                verticalAlignment: VerticalAlignment.Bottom
                horizontalAlignment: HorizontalAlignment.Fill
                Button {
                    text: "Team A"
                    onClicked: {
                        game.tugA();
                    }
                }
                Button {
                    text: "Team B"
                    horizontalAlignment: HorizontalAlignment.Right
                    onClicked: {
                        game.tugB();
                    }
                }
            }
        }
    }
}
