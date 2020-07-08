import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.5
import com.game2048.GameController 1.0

Window {

    visible: true
    x: Screen.width/2 - width/2
    y: Screen.height/2 - height/2
    width: 550
    height: 680
    title: qsTr("2048")

    property int tileWidth : 425/4;
    property var tileArray: []

    function init() {
        for (var i=0;i<4;i++) {
            tileArray[i] = [];
            for (var j=0;j<4;j++)
                tileArray[i][j] = 0;
        }
    }
    Rectangle {
        id: container
        anchors.fill: parent
        color: helper.myColors.bglight
        focus: true
        Keys.onPressed: {
            switch(event.key) {
            case Qt.Key_Left:
                controller.moveLeft();
                break;
            case Qt.Key_Right:
                controller.moveRight();
                break;
            case Qt.Key_Up:
                controller.moveUp();
                break;
            case Qt.Key_Down:
                controller.moveDown();
                break;
            }
        }

        GameController {
            id: controller

            onGameIsOver: {
                console.log("game over")
                dialog.open()
            }

            onGenerateNewTile: tileGrid.createTile(i,j,value)

            onMove: tileGrid.moveTile(src,dst)

            onMerge: tileGrid.mergeTile(src,dst,value)

        }

        InfoDialog {
            id: dialog
            text: qsTr("Game Over")

            onAccepted: container.forceActiveFocus()

        }

        Rectangle {
            id:title
            y: 25
            width: 150
            height: 120
            anchors.left: board.left
            color: "#c2b718"
            radius: 5

            Text {
                anchors.fill: title
                text: "2048"
                font.pointSize: 100
                font.bold: true
                fontSizeMode: Text.Fit
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                color: "#ffffff"
            }
        }

        Grid {
            id: scoreGrid
            rows: 2
            columns: 2
            anchors.right: board.right
            anchors.rightMargin: 15
            anchors.top: title.top
            columnSpacing: 25
            rowSpacing: 20

            ScoreLabel {
                id: currentScore
                width: 90 ; height: 65
                score: controller.score
                title: qsTr("得分")
            }

            ScoreLabel {
                id: maxScore
                width: 90 ; height: 65
                score: controller.maxScore
                title: qsTr("最高分")
            }



            Button {
                id: newGameButton
                width: 90; height: 30
                focus: true
                contentItem: Text {

                    text: qsTr("新游戏")
                    color: "white"
                    horizontalAlignment: Text.AlignHCenter
                }

                background: Rectangle {
                    radius: 5
                    color: "#f36c6c"
                }

                onClicked: {
                    tileGrid.destroyAllTiles();
                    controller.gameStart();
                }
            }
        }

        Rectangle {
            id: board
            y: 165
            width: 500
            height: 500
            color: helper.myColors.bgdark
            radius: 6
            anchors.horizontalCenter: parent.horizontalCenter

            Grid {
                id: tileGrid
                x: 15;
                y: 15;
                rows: 4; columns: 4; spacing: 15


                function createTile(i,j,value) {
                    var tileComponent = Qt.createComponent("Tile.qml");
                    var aniTile = tileComponent.createObject(tileGrid,{
                                                                 "x": j * (15+tileWidth),
                                                                 "y": i * (15+tileWidth),
                                                                 "color": helper.getColor(value),
                                                                 "tileText": helper.getText(value)
                                                             });
                    aniTile.newTileAnimFlag = true;
                    tileArray[i][j] = aniTile;
                }

                function moveTile(src, dst) {
                    //console.log("move"+ src + "to" +dst);
                    var i = Math.floor(src/4);
                    var j = src%4;
                    tileArray[i][j].x = cells.itemAt(dst).x;
                    tileArray[i][j].y = cells.itemAt(dst).y;

                    var m = Math.floor(dst/4);
                    var n = dst%4;
                    tileArray[m][n] = tileArray[i][j];
                    tileArray[i][j] = 0;
                }

                function mergeTile(src, dst, value) {
                    var i = Math.floor(src/4);
                    var j = src%4;
                    tileArray[i][j].destroyFlag = true;
                    tileArray[i][j].x = cells.itemAt(dst).x;
                    tileArray[i][j].y = cells.itemAt(dst).y;
                    tileArray[i][j].z = -1;
                    i = Math.floor(dst/4);
                    j = dst%4;

                    tileArray[i][j].color = helper.getColor(value);
                    tileArray[i][j].tileText = helper.getText(value);
                    tileArray[i][j].doMerge();
                }
                function destroyAllTiles() {
                    for (var i=0;i<4;i++) {
                        for (var j=0;j<4;j++) {
                            if (tileArray[i][j] !== 0) {
                                tileArray[i][j].destroyFlag = true;
                                tileArray[i][j].opacity = 0;
                            }
                        }
                    }
                }
                Repeater {
                    id: cells
                    model: 16
                    Rectangle {
                        width: tileWidth; height: tileWidth  //Rectangle.width - 5 * 15 / 4
                        radius: 3
                        color: helper.myColors.bggray
                    }
                }
            }
        }

        Item {
            id: helper
            focus: false
            property var myColors: {"bglight": "#FAF8EF",
                                    "bggray": Qt.rgba(238/255, 228/255, 218/255, 0.35),
                                    "bgdark": "#BBADA0",
                                    "fglight": "#EEE4DA",
                                    "fgdark": "#776E62",
                                    "bgbutton": "#8F7A66", // Background color for the "New Game" button
                                    "fgbutton": "#F9F6F2" // Foreground color for the "New Game" button
                                    }
            property var tileColors: ["#EEE4DA",
                "#EDE0C8", "#F2B179", "#F59563", "#F67C5F",
                "#F65E3B", "#EDCF72", "#EDCC61", "#EDC850",
                "#EDC53F", "#EDC22E", "#3C3A32"]

            property var arrForM0: {
                var arr = [];
                for (var i=0;i<16;i++)
                    arr[i] = Math.pow(2,i+1).toString();
                return arr;
            }

            function getColor(val) {
                var ret_color;
                if (val < 1)
                    ret_color = myColors.valueOf("bggray");
                else if (val >= 1 && val <= tileColors.length)
                    ret_color = tileColors[val-1];
                else
                    ret_color = tileColors[tileColors.length-1];
                return ret_color;
            }

            function getText(val) {
                var arr = arrForM0;
                if (val >= 1 && val <= tileColors.length)
                    return arr[val-1];
                else
                    return ""

            }
        }
    }



    Component.onCompleted: {
        init();
        controller.gameStart();

    }
}
