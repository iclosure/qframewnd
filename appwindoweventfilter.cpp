#include "appwindoweventfilter.h"
#include <QQuickWindow>
#include <QQuickItem>
#ifdef _MSC_VER
//
#elif defined(__APPLE__)
#import <AppKit/AppKit.h>
#else
//
#endif

AppWindowEventFilter::AppWindowEventFilter(QQuickWindow *window, QObject *parent)
    : QObject(parent)
    , d_border(8)
    , d_window(window)
    , d_header(0)
{
    if (d_window && (d_window->flags() & Qt::FramelessWindowHint)) {
        // header
        QVariant header = d_window->property("header");
        if (header.isValid()) {
            d_header = header.value<QQuickItem *>();
        }
        //
        qApp->installNativeEventFilter(this);
    }
}

AppWindowEventFilter::~AppWindowEventFilter()
{
    qApp->installNativeEventFilter(this);
}

AppWindowEventFilter::RegionTypes AppWindowEventFilter::testHit(const QPoint &pos) const
{
    const int width = d_window->width();
    const int height = d_window->height();
    int captionHeight = d_header ? d_header->height() : 0;
    const int doubleBorder = d_border;// * 2;
    RegionTypes regionTypes = 0;
    // left
    if (0 <= pos.x() && pos.x() < doubleBorder) {
        if (pos.x() < d_border || pos.y() < d_border) {
            regionTypes |= RegionLeft;
        }
    }
    // right
    if (width - doubleBorder < pos.x() && pos.x() <= width) {
        if (pos.x() > width - d_border || pos.y() < d_border) {
            regionTypes |= RegionRight;
        }
    }
    // top
    if (0 <= pos.y() && pos.y() < doubleBorder) {
        if (pos.y() < d_border || pos.x() < doubleBorder) {
            regionTypes |= RegionTop;
        }
    }
    // bottom
    if (doubleBorder < pos.y() && pos.y() <= height) {
        if (pos.y() > height - d_border || pos.y() < d_border) {
            regionTypes |= RegionBottom;
        }
    }
    // caption
    if (doubleBorder < pos.x() && pos.x() < width - doubleBorder
            && d_border < pos.y() && pos.y() < captionHeight) {
        regionTypes = RegionCaption;
    }

    return regionTypes;
}

int AppWindowEventFilter::hitFromRegion(AppWindowEventFilter::RegionTypes regionTypes) const
{
#ifdef _MSC_VER
    switch (regionTypes) {
    case RegionLeft: return HTLEFT;
    case RegionRight: return HTRIGHT;
    case RegionTop: return HTTOP;
    case RegionBottom: return HTBOTTOM;
    case RegionTopLeft: return HTTOPLEFT;
    case RegionTopRight: return HTTOPRIGHT;
    case RegionBottomRight: return HTBOTTOMRIGHT;
    case RegionBottomLeft: return HTBOTTOMLEFT;
    case RegionCaption: return HTCAPTION;
    default: return HTCLIENT;
    }
#elif defined(__APPLE__)
    //
#else
    //
#endif
    return 0;
}

bool AppWindowEventFilter::nativeEventFilter(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType);
#ifdef _MSC_VER
    const MSG *msg = reinterpret_cast<MSG *>(message);
    if (msg->hwnd != (HWND)d_window->winId()) {
        return false;
    }

    switch (msg->message) {
    case WM_NCHITTEST:
    {
        if (d_window->windowState() & Qt::WindowFullScreen) {
            break;
        }

        const QPoint pos = QPoint(msg->pt.x, msg->pt.y) / d_window->effectiveDevicePixelRatio()
                - d_window->position();
        *result = hitFromRegion(testHit(pos));
        switch (*result) {
        case HTCAPTION:
        {
            QQuickItem *focusItem = d_window->activeFocusItem();
            //qDebug() << focusItem->metaObject()->className();
            if (focusItem && focusItem->inherits("QQuickPopupItem")) {
                return false;
            } else {
                QVariant acceptCaption = true;
                if (QMetaObject::invokeMethod(d_window, "caption",
                                              Q_RETURN_ARG(QVariant, acceptCaption),
                                              Q_ARG(QVariant, pos))) {
                    if (acceptCaption.toBool()) {
                        return true;
                    }
                }
            }
            return false;
        }
        case HTCLIENT:
        {
            return true;
        }
        default:
        {
            if ( d_window->windowState() & Qt::WindowMaximized) {
                break;
            }
            return true;
        }
        case HTNOWHERE:
            break;
        }
        break;
    }
    case WM_NCPAINT:
    {
        //qDebug() << ":) WM_NCPAINT";
        break;
    }
    case WM_KEYDOWN:
    {
        switch (msg->wParam) {
        case VK_ESCAPE:
        {
            if (d_window->windowState() & Qt::WindowFullScreen) {
                d_window->showNormal();
                return true;
            }
            break;
        }
        default:
            break;
        }
        break;
    }
    case WM_NCLBUTTONDBLCLK:
    {
        if (msg->wParam == HTCAPTION) {
            if (d_window->windowState() & Qt::WindowMaximized) {
                d_window->showNormal();
            } else {
                d_window->showMaximized();
            }
            return true;
        }
        break;
    }
    case WM_DESTROY:
    {
        qApp->removeNativeEventFilter(this);
        break;
    }
    default:
        break;
    }
#elif defined(__APPLE__)
    //
    const NSEvent *msg = reinterpret_cast<NSEvent *>(message);
    /*if (msg->hwnd != (HWND)d_window->winId()) {
        return false;
    }*/
#else
    //
#endif
    return false;
}
