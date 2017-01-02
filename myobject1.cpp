#include "myobject1.h"
#include <QDebug>

// class MyObject1

MyObject1::MyObject1(QObject *parent)
    : QObject(parent)
{
    setObjectName("MyObject1");
}

bool MyObject1::init()
{
    qDebug() << __FUNCTION__ << ": ...!";

    return true;
}

void MyObject1::setTitle(const QString &text)
{
    emit titleChanged(text);
}

QObject *createMyObject1(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
    Q_UNUSED(qmlEngine);
    Q_UNUSED(jsEngine);

    MyObject1 *myObject1 = new MyObject1();

    if (!myObject1->init()) {
        //
    }

    return myObject1;
}
