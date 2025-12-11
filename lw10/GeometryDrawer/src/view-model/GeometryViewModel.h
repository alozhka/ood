#pragma once

#include <QObject>
#include <QString>

class GeometryViewModel : public QObject {
    Q_OBJECT

    // Properties для QML
    Q_PROPERTY(QObject* shapesModel READ shapesModel CONSTANT)
    Q_PROPERTY(bool canUndo READ canUndo NOTIFY canUndoChanged)
    Q_PROPERTY(bool canRedo READ canRedo NOTIFY canRedoChanged)
    Q_PROPERTY(bool hasSelection READ hasSelection NOTIFY hasSelectionChanged)
    Q_PROPERTY(int shapesCount READ shapesCount NOTIFY shapesCountChanged)
    Q_PROPERTY(int selectedCount READ selectedCount NOTIFY selectedCountChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)

public:
    explicit GeometryViewModel(QObject *parent = nullptr) : QObject(parent), m_statusMessage("Готов к работе") {}

    // Getters для свойств
    QObject* shapesModel() const { return nullptr; } // Заглушка - пока возвращаем nullptr
    bool canUndo() const { return false; }
    bool canRedo() const { return false; }
    bool hasSelection() const { return false; }
    int shapesCount() const { return 0; }
    int selectedCount() const { return 0; }
    QString statusMessage() const { return m_statusMessage; }

public slots:
    // Actions для фигур
    void addRectangle() { setStatusMessage("Добавлен прямоугольник"); }
    void addTriangle() { setStatusMessage("Добавлен треугольник"); }
    void addEllipse() { setStatusMessage("Добавлен эллипс"); }
    void deleteSelected() { setStatusMessage("Удалено выделение"); }

    // Actions для работы с документами
    void newDocument() { setStatusMessage("Новый документ"); }
    void openDocument() { setStatusMessage("Открыт документ"); }
    void saveDocument() { setStatusMessage("Документ сохранен"); }
    void saveDocumentAs() { setStatusMessage("Документ сохранен как"); }

    // Actions для Undo/Redo
    void undo() { setStatusMessage("Отмена операции"); }
    void redo() { setStatusMessage("Повтор операции"); }

    // Actions для выделения
    void selectShape(int id) { setStatusMessage(QString("Выделена фигура %1").arg(id)); }
    void deselectAll() { setStatusMessage("Снято выделение"); }

signals:
    // Signals для properties
    void canUndoChanged();
    void canRedoChanged();
    void hasSelectionChanged();
    void shapesCountChanged();
    void selectedCountChanged();
    void statusMessageChanged();

private:
    void setStatusMessage(const QString& message) {
        if (m_statusMessage != message) {
            m_statusMessage = message;
            emit statusMessageChanged();
        }
    }

    QString m_statusMessage;
};