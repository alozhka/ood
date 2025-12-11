import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: layersPanel

    property var geometryViewModel

    color: "#f0f0f0"

    ScrollView {
        anchors.fill: parent
        anchors.margins: 5

        ColumnLayout {
            width: parent.width

            Label {
                text: "Слои:"
                font.bold: true
            }

            ListView {
                id: layersList
                Layout.fillWidth: true
                Layout.fillHeight: true
                model: geometryViewModel.shapesModel

                delegate: Rectangle {
                    width: parent.width
                    height: 25
                    color: model.selected ? "#e0e0e0" : "transparent"
                    border.color: "#cccccc"
                    border.width: 1

                    Text {
                        anchors.left: parent.left
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.margins: 5
                        text: `${model.type} ${model.id}`
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: geometryViewModel.selectShape(model.id)
                    }
                }
            }
        }
    }
}