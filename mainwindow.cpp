#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "constants.h"

#include <QRegExp>
#include <QDebug>
#include <QStringList>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QListView>
#include <QMap>
#include <QVariant>
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

    setupModel(); // don't forget it

    mapper_USE = new QDataWidgetMapper(this);
    mapper_USE = new QDataWidgetMapper(this);
    mapper_CHOST = new QDataWidgetMapper(this);
    mapper_CFLAGS = new QDataWidgetMapper(this);
    mapper_CXXFLAGS = new QDataWidgetMapper(this);
    mapper_ACCEPTKEYWORDS = new QDataWidgetMapper(this);
    mapper_FEATURES = new QDataWidgetMapper(this);
    mapper_MAKEOPTS = new QDataWidgetMapper(this);
    mapper_PORTDIR = new QDataWidgetMapper(this);
    mapper_DISTDIR = new QDataWidgetMapper(this);
    mapper_PORTAGETMPDIR = new QDataWidgetMapper(this);
    mapper_PKGDIR = new QDataWidgetMapper(this);
    mapper_PORTLOGDIR = new QDataWidgetMapper(this);
    mapper_PORTDIROVERLAY = new QDataWidgetMapper(this);
    mapper_PORTAGEELOGCLASSES = new QDataWidgetMapper(this);
    mapper_FETCHCOMMAND = new QDataWidgetMapper(this);
    mapper_RESUMECOMMAND = new QDataWidgetMapper(this);
    mapper_GENTOOMIRRORS = new QDataWidgetMapper(this);

    mapper_USE->setModel(model);
    mapper_CHOST->setModel(model);
    mapper_CFLAGS->setModel(model);
    mapper_CXXFLAGS->setModel(model);
    mapper_ACCEPTKEYWORDS->setModel(model);
    mapper_FEATURES->setModel(model);
    mapper_MAKEOPTS->setModel(model);
    mapper_PORTDIR->setModel(model);
    mapper_DISTDIR->setModel(model);
    mapper_PORTAGETMPDIR->setModel(model);
    mapper_PKGDIR->setModel(model);
    mapper_PORTLOGDIR->setModel(model);
    mapper_PORTDIROVERLAY->setModel(model);
    mapper_PORTAGEELOGCLASSES->setModel(model);
    mapper_FETCHCOMMAND->setModel(model);
    mapper_RESUMECOMMAND->setModel(model);
    mapper_GENTOOMIRRORS->setModel(model);

    mapper_USE->addMapping(ui->lineEdit_USE,0);
    mapper_CHOST->addMapping(ui->lineEdit_CHOST,0);
    mapper_CFLAGS->addMapping(ui->lineEdit_CFLAGS,0);
    mapper_CXXFLAGS->addMapping(ui->lineEdit_CXXFLAGS,0);
    mapper_ACCEPTKEYWORDS->addMapping(ui->lineEdit_ACCEPTKEYWORDS,0);
    mapper_FEATURES->addMapping(ui->lineEdit_FEATURES,0);
    mapper_MAKEOPTS->addMapping(ui->lineEdit_MAKEOPTS,0);
    mapper_PORTDIR->addMapping(ui->lineEdit_PORTDIR,0);
    mapper_DISTDIR->addMapping(ui->lineEdit_DISTDIR,0);
    mapper_PORTAGETMPDIR->addMapping(ui->lineEdit_PORTAGETMPDIR,0);
    mapper_PKGDIR->addMapping(ui->lineEdit_PKGDIR,0);
    mapper_PORTLOGDIR->addMapping(ui->lineEdit_PORTLOGDIR,0);
    mapper_PORTDIROVERLAY->addMapping(ui->lineEdit_PORTDIROVERLAY,0);
    mapper_PORTAGEELOGCLASSES->addMapping(ui->lineEdit_PORTAGEELOGCLASSES,0);
    mapper_FETCHCOMMAND->addMapping(ui->textEdit_FETCHCOMMAND,0);
    mapper_RESUMECOMMAND->addMapping(ui->textEdit_RESUMECOMMAND,0);
    mapper_GENTOOMIRRORS->addMapping(ui->textEdit_GENTOOMIRRORS,0);

    mapper_USE->setCurrentIndex(N_USE);
    mapper_CHOST->setCurrentIndex(N_CHOST);
    mapper_CFLAGS->setCurrentIndex(N_CFLAGS);
    mapper_CXXFLAGS->setCurrentIndex(N_CXXFLAGS);
    mapper_ACCEPTKEYWORDS->setCurrentIndex(N_ACCEPTKEYWORDS);
    mapper_FEATURES->setCurrentIndex(N_FEATURES);
    mapper_MAKEOPTS->setCurrentIndex(N_MAKEOPTS);
    mapper_PORTDIR->setCurrentIndex(N_PORTDIR);
    mapper_DISTDIR->setCurrentIndex(N_DISTDIR);
    mapper_PORTAGETMPDIR->setCurrentIndex(N_PORTAGETMPDIR);
    mapper_PKGDIR->setCurrentIndex(N_PKGDIR);
    mapper_PORTLOGDIR->setCurrentIndex(N_PORTLOGDIR);
    mapper_PORTDIROVERLAY->setCurrentIndex(N_PORTDIROVERLAY);
    mapper_PORTAGEELOGCLASSES->setCurrentIndex(N_PORTAGEELOGCLASSES);
    mapper_FETCHCOMMAND->setCurrentIndex(N_FETCHCOMMAND);
    mapper_RESUMECOMMAND->setCurrentIndex(N_RESUMECOMMAND);
    mapper_GENTOOMIRRORS->setCurrentIndex(N_GENTOOMIRRORS);


    connect(model,SIGNAL(itemChanged(QStandardItem*)),this,SLOT(_foo(QStandardItem*)));
 }

void MainWindow::_foo(QStandardItem*) {

    qDebug() << "data changed";

}

void MainWindow::setupModel() {

    model = new QStandardItemModel(this);
    int index=0;

    foreach (const QString &s, list) {

    QStandardItem *item1 = new QStandardItem(list.at(index));
    model->setItem(index,item1);

    index++;

    }

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


void MainWindow::addUseFlag() {

    ui->lineEdit_USE->setText("\"" + ui->lineEdit_USE->text().replace("\"","") + ui->comboBox_UseFlag->currentText() + " \"");
}

void MainWindow::addFeaturesFlag() {

    QStandardItem *item1 = new QStandardItem("YAHOO!");
    model->setItem(2,item1);
    mapper_USE->setCurrentIndex(2);


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
