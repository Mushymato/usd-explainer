// app window
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "UsdExplainer.h"
#include <QMainWindow>
#include <QTextEdit>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

public slots:
    void about();
    // void newFile();
    void openFile(const QString &path = QString());

private:
    void setupEditor();
    void setupFileMenu();
    void setupHelpMenu();

    QTextEdit *editor;
    UsdExplainer *explainer;
};

#endif // MAINWINDOW_H
