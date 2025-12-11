import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ToolBar {
    property var geometryViewModel

    RowLayout {
        anchors.fill: parent

        ToolButton {
            text: "Прямоугольник"
            icon.source: "data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='24' height='24' viewBox='0 0 24 24' fill='none' stroke='currentColor' stroke-width='2'%3E%3Crect x='3' y='3' width='18' height='18' rx='2' ry='2'%3E%3C/rect%3E%3C/svg%3E"
            onClicked: geometryViewModel.addRectangle()
        }
        ToolButton {
            text: "Треугольник"
            icon.source: "data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='24' height='24' viewBox='0 0 24 24' fill='none' stroke='currentColor' stroke-width='2'%3E%3Cpolygon points='12 2 22 20 2 20'%3E%3C/polygon%3E%3C/svg%3E"
            onClicked: geometryViewModel.addTriangle()
        }
        ToolButton {
            text: "Эллипс"
            icon.source: "data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='24' height='24' viewBox='0 0 24 24' fill='none' stroke='currentColor' stroke-width='2'%3E%3Cellipse cx='12' cy='12' rx='10' ry='6'%3E%3C/ellipse%3E%3C/svg%3E"
            onClicked: geometryViewModel.addEllipse()
        }
        ToolSeparator { }
        ToolButton {
            text: "Удалить"
            icon.source: "data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='24' height='24' viewBox='0 0 24 24' fill='none' stroke='currentColor' stroke-width='2'%3E%3Cpolyline points='3 6 5 6 21 6'%3E%3C/polyline%3E%3Cpath d='M19 6v14a2 2 0 0 1-2 2H7a2 2 0 0 1-2-2V6m3 0V4a2 2 0 0 1 2-2h4a2 2 0 0 1 2 2v2'%3E%3C/path%3E%3C/svg%3E"
            onClicked: geometryViewModel.deleteSelected()
            enabled: geometryViewModel.hasSelection
        }
        ToolSeparator { }
        ToolButton {
            text: "Отменить"
            icon.source: "data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='24' height='24' viewBox='0 0 24 24' fill='none' stroke='currentColor' stroke-width='2'%3E%3Cpath d='M3 7v6h6'%3E%3C/path%3E%3Cpath d='M21 17a9 9 0 00-9-9 9 9 0 00-6 2.3L3 13'%3E%3C/path%3E%3C/svg%3E"
            onClicked: geometryViewModel.undo()
            enabled: geometryViewModel.canUndo
        }
        ToolButton {
            text: "Повторить"
            icon.source: "data:image/svg+xml,%3Csvg xmlns='http://www.w3.org/2000/svg' width='24' height='24' viewBox='0 0 24 24' fill='none' stroke='currentColor' stroke-width='2'%3E%3Cpath d='M21 7v6h-6'%3E%3C/path%3E%3Cpath d='M3 17a9 9 0 019-9 9 9 0 016 2.3l3 2.7'%3E%3C/path%3E%3C/svg%3E"
            onClicked: geometryViewModel.redo()
            enabled: geometryViewModel.canRedo
        }

        Item { Layout.fillWidth: true }
    }
}