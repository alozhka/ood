import QtQuick 2.15
import QtQuick.Controls 2.15

Rectangle {
    id: geometryCanvas

    property var geometryViewModel

    width: 800
    height: 600
    color: "white"
    border.color: "#cccccc"

    // Область для фигур
    Item {
        id: shapesContainer
        anchors.fill: parent

        // Делегат для отрисовки фигур
        Repeater {
            model: geometryViewModel.shapesModel
            delegate: Loader {
                id: shapeLoader

                x: model.x
                y: model.y
                width: model.width
                height: model.height
                z: model.zIndex

                source: {
                    switch (model.type) {
                        case "rectangle": return "shapes/RectangleShape.qml"
                        case "triangle": return "shapes/TriangleShape.qml"
                        case "ellipse": return "shapes/EllipseShape.qml"
                        default: return ""
                    }
                }

                // Передача данных модели в компонент фигуры
                property var shapeData: model
            }
        }
    }

    MouseArea {
        id: canvasMouseArea
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton | Qt.RightButton

        onClicked: {
            if (mouse.button === Qt.LeftButton) {
                geometryViewModel.deselectAll()
            }
        }
    }
}