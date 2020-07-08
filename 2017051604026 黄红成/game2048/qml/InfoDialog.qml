import QtQuick.Controls 2.5
import QtQuick 2.3
import QtQuick.Layouts 1.3
Dialog {
    id: root


    property string text: 'Game Over'
    anchors.centerIn: parent
    width: 200
    height: 120
    modal: true
    focus: false
    closePolicy: Dialog.CloseOnEscape
    enter: Transition {
            NumberAnimation { property: "opacity"; from: 0.0; to: 1.0 ;duration:200}
        }
    background: Rectangle {
        color: "#e4e3e3"
        radius: 3

    }
    contentItem: ColumnLayout {
        anchors.fill: parent
        Label {
            text: root.text
            font.pixelSize: 34
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 15
        }
        Button {
            text: 'OK'
            Layout.alignment: Qt.AlignHCenter
            onClicked:root.accept()
            Layout.preferredHeight: 20
            font.family: "SimHei"
            font.pixelSize: 18
            background: Rectangle{

                opacity: 0
            }
            Layout.bottomMargin: 15
        }
    }


}
