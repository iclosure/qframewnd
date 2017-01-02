#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class QTextStream;
class QAxBase;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    void test();

    void generateHtml(QTextStream &htmlStream);

    bool generateDocumentation(QAxBase *base);

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
