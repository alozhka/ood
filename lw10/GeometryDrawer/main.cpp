#include "src/view-model/GeometryViewModel.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char* argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // Создаем и регистрируем ViewModel
    GeometryViewModel geometryViewModel;
    engine.rootContext()->setContextProperty("geometryViewModel", &geometryViewModel);

    // Загружаем главный QML файл
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

    return app.exec();
}
