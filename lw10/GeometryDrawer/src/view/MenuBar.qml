import QtQuick 2.15
import QtQuick.Controls 2.15

MenuBar {
    property var geometryViewModel

    Menu {
        title: "Файл"
        MenuItem {
            text: "Новый"
            onTriggered: geometryViewModel.newDocument()
        }
        MenuItem {
            text: "Открыть..."
            onTriggered: geometryViewModel.openDocument()
        }
        MenuItem {
            text: "Сохранить"
            onTriggered: geometryViewModel.saveDocument()
        }
        MenuItem {
            text: "Сохранить как..."
            onTriggered: geometryViewModel.saveDocumentAs()
        }
        MenuSeparator { }
        MenuItem {
            text: "Выход"
            onTriggered: Qt.quit()
        }
    }
    Menu {
        title: "Правка"
        MenuItem {
            text: "Отменить"
            onTriggered: geometryViewModel.undo()
            enabled: geometryViewModel.canUndo
        }
        MenuItem {
            text: "Повторить"
            onTriggered: geometryViewModel.redo()
            enabled: geometryViewModel.canRedo
        }
        MenuSeparator { }
        MenuItem {
            text: "Удалить"
            onTriggered: geometryViewModel.deleteSelected()
            enabled: geometryViewModel.hasSelection
        }
    }
}