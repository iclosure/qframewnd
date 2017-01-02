#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#ifdef _MSC_VER
#include <qt_windows.h>
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , d_border(8)
{
    //8800f001
    setWindowFlags(Qt::FramelessWindowHint);
}

MainWindow::RegionTypes MainWindow::testHit(const QPoint &pos) const
{
    const int captionHeight = 30;
    const int doubleBorder = d_border * 2;
    RegionTypes regionTypes = 0;
    // left
    if (0 < pos.x() && pos.x() < doubleBorder) {
        if (pos.x() < d_border || pos.y() < d_border) {
            regionTypes |= RegionLeft;
        }
    }
    // right
    if (width() - doubleBorder < pos.x() && pos.x() < width()) {
        if (pos.x() > width() - d_border || pos.y() < d_border) {
            regionTypes |= RegionRight;
        }
    }
    // top
    if (0 < pos.y() && pos.y() < doubleBorder) {
        if (pos.y() < d_border || pos.x() < doubleBorder) {
            regionTypes |= RegionTop;
        }
    }
    // bottom
    if (doubleBorder < pos.y() && pos.y() < height()) {
        if (pos.y() > height() - d_border || pos.y() < d_border) {
            regionTypes |= RegionBottom;
        }
    }
    // caption
    if (doubleBorder < pos.x() && pos.x() < width() - doubleBorder
            && d_border < pos.y() && pos.y() < captionHeight) {
        regionTypes = RegionCaption;
    }

    return regionTypes;
}

int MainWindow::hitFromRegion(MainWindow::RegionTypes regionTypes) const
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
    default: return HTNOWHERE;
    }
#elif defined(__APPLE__)
    return 0;
#else
    //
#endif
    return 0;
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
#ifdef _MSC_VER
    Q_UNUSED(eventType);
    if ( windowState() && (Qt::WindowMaximized | Qt::WindowFullScreen)) {
        return false;
    }

    const MSG *msg = reinterpret_cast<MSG *>(message);
    switch (msg->message) {
    case WM_NCHITTEST:
    {
        const QPoint pos = QPoint(msg->pt.x, msg->pt.y) / qApp->devicePixelRatio() - this->pos();
        int hit = hitFromRegion(testHit(pos));
        if (hit != HTNOWHERE) {
            *result = hit;
            return true;
        }
        break;
    }
    case WM_NCPAINT:
    {
        qDebug() << ":)";
        break;
    }
    default:
        break;
    }

    return false;
#else
    return false;
#endif
}
