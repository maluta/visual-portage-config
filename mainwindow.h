#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QStandardItemModel>
#include <QDataWidgetMapper>
#include "highlighter.h"



namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void changeEvent(QEvent *e);

private slots:
    void HideComment1st(bool);
    void HideComment(bool);
    void describeUseFlag(int);
    void addUseFlag();
    void addFeaturesFlag();
    void changeConfig(QString);
    void UpdateGeneral();
    void UpdateDownload();
    void UpdatePortage();
    void TabChanged(int);
    void SaveConf();

private:
    Ui::MainWindow *ui;
    QFile *f;
    Highlighter *highlighter;

    QStringList useflagdescription;
    QStringList list;
    QString alltext;

    void AddUseList();
    void ListUpdate(QFile*);
    void SetupModel();

    QStandardItemModel *model;

    QDataWidgetMapper *mapper_USE;
    QDataWidgetMapper *mapper_CHOST;
    QDataWidgetMapper *mapper_CFLAGS;
    QDataWidgetMapper *mapper_CXXFLAGS;
    QDataWidgetMapper *mapper_ACCEPTKEYWORDS;
    QDataWidgetMapper *mapper_FEATURES;
    QDataWidgetMapper *mapper_MAKEOPTS;
    QDataWidgetMapper *mapper_PORTDIR;
    QDataWidgetMapper *mapper_DISTDIR;
    QDataWidgetMapper *mapper_PORTAGETMPDIR;
    QDataWidgetMapper *mapper_PKGDIR;
    QDataWidgetMapper *mapper_PORTLOGDIR;
    QDataWidgetMapper *mapper_PORTDIROVERLAY;
    QDataWidgetMapper *mapper_PORTAGEELOGCLASSES;
    QDataWidgetMapper *mapper_FETCHCOMMAND;
    QDataWidgetMapper *mapper_RESUMECOMMAND;
    QDataWidgetMapper *mapper_GENTOOMIRRORS;

};

#endif // MAINWINDOW_H
