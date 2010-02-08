#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "constants.h"

#include <QRegExp>
#include <QDebug>
#include <QStringList>
#include <QStandardItemModel>
#include <QAbstractItemModel>
#include <QListView>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    AddUseList();

    HideComment1st(true); // to populate 'list'
    HideComment1st(false);

    highlighter = new Highlighter(ui->textEdit_makeconfig->document());

    connect(ui->checkBox_Comments,SIGNAL(clicked(bool)),this,SLOT(HideComment1st(bool)));
    connect(ui->comboBox_UseFlag,SIGNAL(currentIndexChanged(int)),this,SLOT(describeUseFlag(int)));
    connect(ui->pushButton_addUseFlag,SIGNAL(clicked()),this,SLOT(addUseFlag()));
    connect(ui->pushButton_Features,SIGNAL(clicked()),this,SLOT(addFeaturesFlag()));
    connect(ui->lineEdit_USE,SIGNAL(textChanged(QString)),this,SLOT(changeConfig(QString)));
    connect(ui->pushButton_UpdateGeneral,SIGNAL(clicked()),this,SLOT(UpdateGeneral()));
    connect(ui->pushButton_UpdatePortage,SIGNAL(clicked()),this,SLOT(UpdatePortage()));
    connect(ui->pushButton_UpdateDownload,SIGNAL(clicked()),this,SLOT(UpdateDownload()));

    connect(ui->pushButton_Save,SIGNAL(clicked()),this,SLOT(SaveConf()));

    SetupModel(); // don't forget it

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

}

void MainWindow::SetupModel() {

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

void MainWindow::ListUpdate(QFile *f) {

    list.clear();

    f->seek(0);

    while (!f->atEnd()) {
        alltext = f->readLine();
        alltext.remove(QRegExp("^#.*$"));
        list << alltext;
    }
    list.removeDuplicates();
}

void MainWindow::HideComment1st(bool b) {

    f = new QFile(MAKE_CONF_PATH);
    f->open(QIODevice::ReadOnly | QIODevice::Text);

    if (b) {

        ListUpdate(f); // return on private variable 'list'
        alltext = list.join("");

        ui->textEdit_makeconfig->setReadOnly(true);
        ui->checkBox_Comments->setText("&Hide comments (read-only)");

    } else {
        alltext = f->readAll();
        ui->textEdit_makeconfig->setReadOnly(false);
        ui->checkBox_Comments->setText("&Hide comments");

    }

    ui->textEdit_makeconfig->setText(alltext);
    f->close();

  }

void MainWindow::HideComment(bool b) {
}

void MainWindow::AddUseList() {

    QRegExp rx("^[a-zA-Z0-9]+");
    QRegExp rx1("^[^#].*");

    QString s;

    f = new QFile(USE_DESC_PATH);
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

}

void MainWindow::describeUseFlag(int i) {

    statusBar()->showMessage(useflagdescription.at(i));
}


void MainWindow::addUseFlag() {

    QString _t(ui->lineEdit_USE->text());

    _t = _t.trimmed(); // remove white spaces
    _t = _t.replace("\"",""); // remove "
    _t =  _t + " " + ui->comboBox_UseFlag->currentText(); // insert combobox content
    _t = _t.replace("=","=\""); // insert " again
    _t = _t.insert(_t.length(),"\""); // insert " at end

    ui->lineEdit_USE->setText(_t);
}

void MainWindow::addFeaturesFlag() {

    QString _t(ui->lineEdit_FEATURES->text());

    _t = _t.trimmed();
    _t = _t.replace("\"","");
    _t =  _t + " " + ui->comboBox_Features->currentText();
    _t = _t.replace("=","=\"");
    _t = _t.trimmed();
    _t = _t.insert(_t.length(),"\"");

    ui->lineEdit_FEATURES->setText(_t);


}

void MainWindow::changeConfig(QString s) {
}

void MainWindow::SaveConf() {

    f = new QFile(MAKE_CONF_PATH);
    f->open(QIODevice::ReadWrite | QIODevice::Text);
    f->write(ui->textEdit_makeconfig->toPlainText().toAscii());
    ListUpdate(f);
    f->close();

    model->item(N_USE,0)->setText(list.at(N_USE));
    model->item(N_CHOST,0)->setText(list.at(N_CHOST));
    model->item(N_CFLAGS,0)->setText(list.at(N_CFLAGS));
    model->item(N_CXXFLAGS,0)->setText(list.at(N_CXXFLAGS));
    model->item(N_ACCEPTKEYWORDS,0)->setText(list.at(N_ACCEPTKEYWORDS));
    model->item(N_FEATURES,0)->setText(list.at(N_FEATURES));
    model->item(N_MAKEOPTS,0)->setText(list.at(N_MAKEOPTS));
    model->item(N_PORTDIR,0)->setText(list.at(N_PORTDIR));
    model->item(N_DISTDIR,0)->setText(list.at(N_DISTDIR));
    model->item(N_PORTAGETMPDIR,0)->setText(list.at(N_PORTAGETMPDIR));
    model->item(N_PKGDIR,0)->setText(list.at(N_PKGDIR));
    model->item(N_PORTLOGDIR,0)->setText(list.at(N_PORTLOGDIR));
    model->item(N_PORTDIROVERLAY,0)->setText(list.at(N_PORTDIROVERLAY));
    model->item(N_PORTAGEELOGCLASSES,0)->setText(list.at(N_PORTAGEELOGCLASSES));
    model->item(N_FETCHCOMMAND,0)->setText(list.at(N_FETCHCOMMAND));
    model->item(N_RESUMECOMMAND,0)->setText(list.at(N_RESUMECOMMAND));
    model->item(N_GENTOOMIRRORS,0)->setText(list.at(N_GENTOOMIRRORS));
}

void MainWindow::UpdateGeneral() {

    f = new QFile(MAKE_CONF_PATH);
    f->open(QIODevice::ReadOnly | QIODevice::Text);
    alltext = f->readAll();
    f->close();

    // Update USE flag
    if (QString::compare(ui->lineEdit_USE->text(),list.at(N_USE))) {
        model->item(N_USE,0)->setText(ui->lineEdit_USE->text());
        alltext.replace(list.at(N_USE),ui->lineEdit_USE->text()+"\n");
    }
    // Update CHOST flag
    model->item(N_CHOST,0)->setText(ui->lineEdit_CHOST->text());
    alltext.replace(list.at(N_CHOST),ui->lineEdit_CHOST->text()+"\n");

    // Update CXXFLAGS flag
    model->item(N_CXXFLAGS,0)->setText(ui->lineEdit_CXXFLAGS->text());
    alltext.replace(list.at(N_CXXFLAGS),ui->lineEdit_CXXFLAGS->text()+"\n");

    // Update ACCEPTKEYWORDS flag
    model->item(N_ACCEPTKEYWORDS,0)->setText(ui->lineEdit_ACCEPTKEYWORDS->text());
    alltext.replace(list.at(N_ACCEPTKEYWORDS),ui->lineEdit_ACCEPTKEYWORDS->text()+"\n");

    // Update FEATURES flag
    model->item(N_FEATURES,0)->setText(ui->lineEdit_FEATURES->text());
    alltext.replace(list.at(N_FEATURES),ui->lineEdit_FEATURES->text()+"\n");

    // Update MAKEOPTS flag
    model->item(N_MAKEOPTS,0)->setText(ui->lineEdit_MAKEOPTS->text());
    alltext.replace(list.at(N_MAKEOPTS),ui->lineEdit_MAKEOPTS->text()+"\n");

    // refresh
    ui->textEdit_makeconfig->setText(alltext);

    // reopen to save updates (TODO: test seek(0))
    f = new QFile(MAKE_CONF_PATH);
    f->open(QIODevice::WriteOnly | QIODevice::Text);
    f->write(alltext.toAscii());
    f->close();
}

void MainWindow::UpdatePortage() {

    f = new QFile(MAKE_CONF_PATH);
    f->open(QIODevice::ReadOnly | QIODevice::Text);
    alltext = f->readAll();
    f->close();

    // Update PORTDIR flag
    model->item(N_PORTDIR,0)->setText(ui->lineEdit_PORTDIR->text());
    alltext.replace(list.at(N_PORTDIR),ui->lineEdit_PORTDIR->text()+"\n");

    // Update DISTDIR flag
    model->item(N_DISTDIR,0)->setText(ui->lineEdit_DISTDIR->text());
    alltext.replace(list.at(N_DISTDIR),ui->lineEdit_DISTDIR->text()+"\n");

    // Update PORTAGE_TMPDIR flag
    model->item(N_PORTAGETMPDIR,0)->setText(ui->lineEdit_PORTAGETMPDIR->text());
    alltext.replace(list.at(N_PORTAGETMPDIR),ui->lineEdit_PORTAGETMPDIR->text()+"\n");

    // Update PKGDIR flag
    model->item(N_PKGDIR,0)->setText(ui->lineEdit_PKGDIR->text());
    alltext.replace(list.at(N_PKGDIR),ui->lineEdit_PKGDIR->text()+"\n");

    // Update PORT_LOGDIR flag
    model->item(N_PORTLOGDIR,0)->setText(ui->lineEdit_PORTLOGDIR->text());
    alltext.replace(list.at(N_PORTLOGDIR),ui->lineEdit_PORTLOGDIR->text()+"\n");

    // Update PORTDIR_OVERLAY flag
    model->item(N_PORTDIROVERLAY,0)->setText(ui->lineEdit_PORTDIROVERLAY->text());
    alltext.replace(list.at(N_PORTDIROVERLAY),ui->lineEdit_PORTDIROVERLAY->text()+"\n");

    // Update PORTAGE_ELOG_CLASSES flag
    model->item(N_PORTAGEELOGCLASSES,0)->setText(ui->lineEdit_PORTAGEELOGCLASSES->text());
    alltext.replace(list.at(N_PORTAGEELOGCLASSES),ui->lineEdit_PORTAGEELOGCLASSES->text()+"\n");

    // refresh
    ui->textEdit_makeconfig->setText(alltext);

    // reopen to save updates (TODO: test seek(0))
    f = new QFile(MAKE_CONF_PATH);
    f->open(QIODevice::WriteOnly | QIODevice::Text);
    f->write(alltext.toAscii());
    f->close();
}

void MainWindow::UpdateDownload() {


    f = new QFile(MAKE_CONF_PATH);
    f->open(QIODevice::ReadOnly | QIODevice::Text);
    alltext = f->readAll();
    f->close();

    // Update RESUMECOMMAND flag
    model->item(N_RESUMECOMMAND,0)->setText(ui->textEdit_RESUMECOMMAND->toPlainText());
    alltext.replace(list.at(N_RESUMECOMMAND),ui->textEdit_RESUMECOMMAND->toPlainText()+"\n");

    // Update GENTOOMIRRORS flag
    model->item(N_GENTOOMIRRORS,0)->setText(ui->textEdit_GENTOOMIRRORS->toPlainText());
    alltext.replace(list.at(N_GENTOOMIRRORS),ui->textEdit_GENTOOMIRRORS->toPlainText()+"\n");

    // Update FETCHCOMMAND flag
    model->item(N_FETCHCOMMAND,0)->setText(ui->textEdit_FETCHCOMMAND->toPlainText());
    alltext.replace(list.at(N_FETCHCOMMAND),ui->textEdit_FETCHCOMMAND->toPlainText()+"\n");

    // refresh
    ui->textEdit_makeconfig->setText(alltext);

    // reopen to save updates (TODO: test seek(0))
    f = new QFile(MAKE_CONF_PATH);
    f->open(QIODevice::WriteOnly | QIODevice::Text);
    f->write(alltext.toAscii());
    f->close();
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
