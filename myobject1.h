#ifndef MYOBJECT1_H
#define MYOBJECT1_H

#include <QObject>
#include <QQuickWindow>
#include <QQuickItem>
#include <QMainWindow>

// class MyObject1

class QQmlEngine;
class QJSEngine;

class MyObject1 : public QObject
{
    Q_OBJECT
public:
    explicit MyObject1(QObject *parent = 0);

    Q_INVOKABLE bool init();

signals:
    void titleChanged(const QString &text);

public slots:
    void setTitle(const QString &text);

private:
};

QObject *createMyObject1(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

#endif // MYOBJECT1_H
