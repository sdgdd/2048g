import QtQuick 2.0
import QtQuick.Layouts 1.3
Rectangle {
    id: currentScore
    property string score: "0"
    property string title: qsTr("得分")
    color: "#888282"
    width: 90 ; height: 65
    ColumnLayout {
        anchors.fill: parent
        spacing: 5
        Text {
            color: "#bbc2bc"
            text: title

            font.pixelSize: 18
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin:3
        }
        Text {
            text: currentScore.score
            font.pixelSize: 22
            color: "white"
            Layout.alignment: Qt.AlignHCenter

        }
    }
    radius: 5
}
