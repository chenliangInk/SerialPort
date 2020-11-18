#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtGui/QFontDatabase>
#include <QtGui/QFont>

#include "serialport.h"


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    int res = QFontDatabase::addApplicationFont(":/font/CC0_阿里普惠体Medium.ttf");  // 添加字体，返回字体编号
    if (res != -1) {
        QStringList font = QFontDatabase::applicationFontFamilies(res);  // 根据编号获取添加的字体名
        app.setFont(QFont(font[0], 12));  // 设置字体
    }

    QQmlApplicationEngine engine;
    // 注册SerialPort单例
    qmlRegisterSingletonType<SerialPort>("liang", 1, 0, "SerialPort", [](QQmlEngine* engine, QJSEngine* scriptEngine)-> QObject* {
        Q_UNUSED(engine)
        Q_UNUSED(scriptEngine)
        SerialPort* serialPort = new SerialPort();
        return serialPort;
    });

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
