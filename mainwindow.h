#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>

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
    void parse();
    void addUseFlag();
    void addFeaturesFlag();
    void changeConfig(QString);

private:
    Ui::MainWindow *ui;
    QFile *f;
    Highlighter *highlighter;

    QStringList useflagdescription;
    QStringList list;
    QString alltext;

    void AddUseList();


};

#endif // MAINWINDOW_H
