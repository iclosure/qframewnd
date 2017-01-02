#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include "myobject1.h"
#include "mainwindow.h"
#include "appwindoweventfilter.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_CompressHighFrequencyEvents);
    QApplication app(argc, argv);
#if 1

    //
    if (qgetenv("QT_QUICK_CONTROLS_1_STYLE").isEmpty()) {
        qputenv("QT_QUICK_CONTROLS_1_STYLE", "Flat");
    }

    qmlRegisterSingletonType<MyObject1>("com.smartsoft.model", 1, 0, "MyObject1", createMyObject1);
    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));
    QObjectList objects = engine.rootObjects();
    if (objects.isEmpty()) {
        Q_ASSERT(false);
        return -1;
    }
    AppWindowEventFilter filter(qobject_cast<QQuickWindow *>(objects.first()));
    Q_UNUSED(filter);
#else
    MainWindow mainWindow;
    mainWindow.resize(app.desktop()->width() * 2 / 3,
                      app.desktop()->height() * 2 / 3);
    mainWindow.show();

#endif

    return app.exec();
}
