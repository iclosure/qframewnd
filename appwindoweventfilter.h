#ifndef APPWINDOWEVENTFILTER_H
#define APPWINDOWEVENTFILTER_H

#include <QObject>
#include <QAbstractNativeEventFilter>

class QQuickWindow;
class QQuickItem;

class AppWindowEventFilter : public QObject, public QAbstractNativeEventFilter
{
    Q_OBJECT
public:
    enum RegionType {
        RegionLeft = 0x01,
        RegionRight = 0x02,
        RegionTop = 0x04,
        RegionBottom = 0x08,
        RegionTopLeft = RegionTop | RegionLeft,
        RegionTopRight = RegionTop | RegionRight,
        RegionBottomLeft = RegionBottom | RegionLeft,
        RegionBottomRight = RegionBottom | RegionRight,
        RegionCaption = 0x80
    };
    Q_ENUM(RegionType)
    Q_DECLARE_FLAGS(RegionTypes, RegionType)

    explicit AppWindowEventFilter(QQuickWindow *window, QObject *parent = 0);
    ~AppWindowEventFilter();

    RegionTypes testHit(const QPoint &pos) const;
    int hitFromRegion(RegionTypes regionTypes) const;

signals:

public slots:

protected:
    bool nativeEventFilter(const QByteArray &eventType,
                           void *message, long *result) Q_DECL_OVERRIDE;

private:
    int d_border;
    QQuickWindow *d_window;
    QQuickItem *d_header;
};

#endif // APPWINDOWEVENTFILTER_H
