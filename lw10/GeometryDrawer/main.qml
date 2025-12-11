import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15

import "src/view"

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: "Geometry Drawer"

    // Импортируем наши компоненты
    property var geometryViewModel: null // Будет установлено из C++

    MenuBar {
        geometryViewModel: parent.geometryViewModel
    }

    ToolBar {
        geometryViewModel: parent.geometryViewModel
    }

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        // Левая панель для слоев
        LayersPanel {
            id: leftPanel
            SplitView.minimumWidth: 150
            SplitView.preferredWidth: 200
            geometryViewModel: parent.parent.geometryViewModel
        }

        // Центральная область с холстом
        ScrollView {
            id: canvasArea
            SplitView.fillWidth: true

            GeometryCanvas {
                geometryViewModel: parent.parent.parent.geometryViewModel
            }
        }
    }

    StatusBar {
        geometryViewModel: parent.geometryViewModel
    }
}