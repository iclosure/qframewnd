#include "dialog.h"
#include "ui_dialog.h"
#include <QAxObject>
#include <QAxWidget>
#include <QtCore>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    //
    test();
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::test()
{
    QTemporaryFile tmpfile;
    if (!tmpfile.open()) {
        qDebug() << "Opening temp file failed " << endl;
        return;
    }

    QTextStream outHtml(&tmpfile);
    outHtml.setCodec("UTF-8");
    generateHtml(outHtml);
    outHtml.flush();

    QString filePath = QFileInfo(QApplication::applicationDirPath()
                                 .append("/../docs/test1.doc")).absoluteFilePath();

    QString path = QFileInfo(filePath).path();
    if (!QDir(path).exists()) {
        QDir().mkpath(path);
    }

    //3. create a word App
    QAxObject word("Word.Application");
    generateDocumentation(&word);

    word.setProperty("Visible", false);
    word.setProperty("WindowState", 2);
    word.setProperty("DisplayAlerts", false);
    QAxObject *documents = word.querySubObject("Documents");
    if(!documents) {
        qDebug() << "Quering Documents failed " << endl;
        return;
    }

    //create a new document using the html file as a template
    documents->dynamicCall("Add(QString)", tmpfile.fileName());
    generateDocumentation(documents);

    QAxObject *document = word.querySubObject("ActiveDocument");
    if(!document) {
        qDebug() << "Quering ActiveDocument failed " << endl;
        return;
    }
    generateDocumentation(document);

    //save a doc using the input file name
    document->dynamicCall("SaveAs(QString)", filePath);

    //4. exit the word App
    document->dynamicCall("Close(boolean)", false);
    word.dynamicCall("Quit()");

    qDebug() << "The content is saved successfully" << endl;

    tmpfile.close();
}

void Dialog::generateHtml(QTextStream &htmlStream)
{
    htmlStream << "<html>"
                  "<head>"
                  "<style>"
                  "body {"
                  " font-family: Microsoft Yahei;"
                  " font-size: 16px;"
                  "}"
                  "table {"
                  " border-style: solid;"
                  " font-size: 16px;"
                  " border-collapse: collapse;"
                  "}"
                  "td {"
                  " padding-left: 10px;"
                  " padding-top: 2px;"
                  " padding-bottom: 2px;"
                  "}"
                  "td textarea {"
                  " border-style: solid;"
                  "border-color: #333;"
                  "}"
                  "</style>"
                  "</head>"
                  "<body>";
    htmlStream << "<h1>Hello,World!</h1>"
                  "<table align=center width=100% bordercolo='blue' border='0' bordertype='1' bgcolor='darkgray'>"
                  "<tr>"
                  "<td width=30% class='name'>ZhangXiao</td>"
                  "<td width=30% class='age'>25</td>"
                  "<td width=40% class='contry'>ChengDu</td>"
                  "<tr>"
                  "<tr>"
                  "<td width=30% class='name'>HeDong</td>"
                  "<td width=30% class='age'>28</td>"
                  "<td width=40% class='contry'>TianJing</td>"
                  "<tr>"
                  "</table>";
    htmlStream << "</body>"
                  "</html>";
}

bool Dialog::generateDocumentation(QAxBase *base)
{
    if (!base) {
        return false;
    }

    //
    QString filePath = QApplication::applicationDirPath().append("/../docs/");
    const QMetaObject *mobj = base->metaObject();
    filePath.append(mobj->classInfo(mobj->indexOfClassInfo(
                                        "Interface " + QByteArray::number(1))).value());
    filePath.append(QString("_%1.html").arg(base->control()));

    QFileInfo fileInfo(filePath);
    if (!QDir(fileInfo.path()).exists()) {
        QDir().mkpath(fileInfo.path());
    }

    QFile file(fileInfo.absoluteFilePath());
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QTextStream htmlStream(&file);
    htmlStream << base->generateDocumentation();

    file.close();

    return true;
}
