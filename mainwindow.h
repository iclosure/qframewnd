#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class MainWindow : public QMainWindow
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

    explicit MainWindow(QWidget *parent = 0);

    MainWindow::RegionTypes testHit(const QPoint &pos) const;
    int hitFromRegion(MainWindow::RegionTypes regionTypes) const;

signals:

public slots:

protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) Q_DECL_OVERRIDE;

private:
    int d_border;
};

#endif // MAINWINDOW_H
