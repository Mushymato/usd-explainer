// app window
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "UsdHighlighter.h"
#include "UsdTextEdit.h"
#include <QMainWindow>

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

    UsdTextEdit *editor;
    UsdHighlighter *highlighter;
};

#endif // MAINWINDOW_H
