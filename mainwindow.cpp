#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QRegExp>
#include <QDebug>
#include <QStringList>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QListView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    AddUseList();

    HideComment1st(true); // to populate 'list'
    HideComment1st(false);

    highlighter = new Highlighter(ui->textEdit_makeconfig->document());

    connect(ui->checkBox_Comments,SIGNAL(clicked(bool)),this,SLOT(HideComment(bool)));
    connect(ui->comboBox_UseFlag,SIGNAL(currentIndexChanged(int)),this,SLOT(describeUseFlag(int)));
    connect(ui->pushButton_addUseFlag,SIGNAL(clicked()),this,SLOT(addUseFlag()));
    connect(ui->pushButton_Features,SIGNAL(clicked()),this,SLOT(addFeaturesFlag()));

    connect(ui->lineEdit_USE,SIGNAL(textChanged(QString)),this,SLOT(changeConfig(QString)));

    parse();

 }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::HideComment1st(bool b) {

    f = new QFile("/etc/make.conf");
    f->open(QIODevice::ReadOnly | QIODevice::Text);

    if (b) {

        while (!f->atEnd()) {
            alltext = f->readLine();
            alltext.remove(QRegExp("^#.*$"));
            list << alltext;
        }
        list.removeDuplicates();
        alltext = list.join("");


    } else {
        alltext = f->readAll();
    }
    ui->textEdit_makeconfig->setText(alltext);
    f->close();
    delete f;
}

void MainWindow::HideComment(bool b) {

    if (b)
        ui->textEdit_makeconfig->setText(list.join(""));
    else
        ui->textEdit_makeconfig->setText(alltext);

}

void MainWindow::AddUseList() {

    QRegExp rx("^[a-zA-Z0-9]+");
    QRegExp rx1("^[^#].*");

    QString s;

    f = new QFile("/usr/portage/profiles/use.desc");
    f->open(QIODevice::ReadOnly | QIODevice::Text);
    while (!f->atEnd()) {
        s = f->readLine();
        rx.indexIn(s,0);
        rx1.indexIn(s,0);
        if (rx.cap(0).startsWith(""))
             ui->comboBox_UseFlag->addItem(rx.cap(0));
             useflagdescription << rx1.cap(0);
    }
    useflagdescription.removeDuplicates();
    useflagdescription.removeFirst();
    useflagdescription.removeFirst();
    f->close();
    delete f;
}

void MainWindow::describeUseFlag(int i) {

    statusBar()->showMessage(useflagdescription.at(i));
}

void MainWindow::parse()
{
    QRegExp rx("=\"?(.*)\"?");

    foreach (const QString &s, list) {

        rx.indexIn(s,0);

        if (s.startsWith("USE"))
            ui->lineEdit_USE->setText(rx.cap(1));

        if (s.startsWith("CHOST"))
            ui->lineEdit_CHOST->setText(rx.cap(1));

        if (s.startsWith("CFLAGS"))
            ui->lineEdit_CFLAGS->setText(rx.cap(1));

        if (s.startsWith("CXXFLAGS"))
            ui->lineEdit_CXXFLAGS->setText(rx.cap(1));

        if (s.startsWith("ACCEPT_KEYWORDS"))
            ui->lineEdit_ACCEPTKEYWORDS->setText(rx.cap(1));

        if (s.startsWith("FEATURES"))
            ui->lineEdit_FEATURES->setText(rx.cap(1));

        if (s.startsWith("MAKEOPTS"))
            ui->lineEdit_MAKEOPTS->setText(rx.cap(1));

        if (s.startsWith("PORTDIR"))
            ui->lineEdit_PORTDIR->setText(rx.cap(1));

        if (s.startsWith("DISTDIR"))
            ui->lineEdit_DISTDIR->setText(rx.cap(1));

        if (s.startsWith("PORTAGE_TMPDIR"))
            ui->lineEdit_PORTAGETMPDIR->setText(rx.cap(1));

        if (s.startsWith("PKGDIR"))
            ui->lineEdit_PKGDIR->setText(rx.cap(1));

        if (s.startsWith("PORT_LOGDIR"))
            ui->lineEdit_PORTLOGDIR->setText(rx.cap(1));

        if (s.startsWith("PORTDIR_OVERLAY"))
            ui->lineEdit_PORTDIROVERLAY->setText(rx.cap(1));

        if (s.startsWith("PORTAGE_ELOG_CLASSES"))
            ui->lineEdit_PORTAGEELOGCLASSES->setText(rx.cap(1));

        if (s.startsWith("FETCHCOMMAND")) {
            ui->plainTextEdit_FETCHCOMMAND->clear();
            ui->plainTextEdit_FETCHCOMMAND->appendPlainText(rx.cap(1));
        }
        if (s.startsWith("RESUMECOMMAND")) {
            ui->plainTextEdit_RESUMECOMMAND->clear();
            ui->plainTextEdit_RESUMECOMMAND->appendPlainText(rx.cap(1));
        }
        if (s.startsWith("GENTOO_MIRRORS")) {
            ui->plainTextEdit_GENTOOMIRRORS->clear();
            ui->plainTextEdit_GENTOOMIRRORS->appendPlainText(rx.cap(1));

        }
    }
}

void MainWindow::addUseFlag() {

    ui->lineEdit_USE->setText("\"" + ui->lineEdit_USE->text().replace("\"","") + ui->comboBox_UseFlag->currentText() + " \"");
}

void MainWindow::addFeaturesFlag() {

    ui->lineEdit_FEATURES->setText("\"" + ui->lineEdit_FEATURES->text().replace("\"","") + ui->comboBox_Features->currentText() + " \"");
}



void MainWindow::changeConfig(QString s) {

    qDebug() << s;
    //QRegExp rx("^[a-zA-Z0-9]+");
    //rx.indexIn(s,0);
    //qDebug() << rx.cap(0);
    //qDebug() << s;

    //ui->textEdit_makeconfig->clear();
    //ui->textEdit_makeconfig->append(alltext);



}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
