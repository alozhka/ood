import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

StatusBar {
    property var geometryViewModel

    RowLayout {
        anchors.fill: parent

        Label {
            text: "Фигур: " + geometryViewModel.shapesCount
        }
        Label {
            text: "Выделено: " + geometryViewModel.selectedCount
        }
        Item {
            Layout.fillWidth: true
        }
        Label {
            text: geometryViewModel.statusMessage
        }
    }
}