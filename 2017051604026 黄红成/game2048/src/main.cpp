#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFont>
#include "tilemodel.h"
#include "gamecontroller.h"
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QFont font;
    font.setFamily("Simhei");
    font.setPixelSize(16);
    QGuiApplication app(argc, argv);
    app.setFont(font);
    qmlRegisterType<GameController>("com.game2048.GameController",1,0,"GameController");
    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
