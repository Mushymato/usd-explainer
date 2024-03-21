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
    //! constructor
    MainWindow(QWidget *parent = nullptr);

public slots:
    //! display help info
    void about();
    //! load file into editor
    void openFile(const QString &path = QString());

private:
    //! setup editor and syntax highlighter object
    void setupEditor();
    //! setup file open menu
    void setupFileMenu();
    //! setup help menu
    void setupHelpMenu();

    //! USD text editor instance
    UsdTextEdit *editor;
    //! USD syntax highlighter instance
    UsdHighlighter *highlighter;
};

#endif // MAINWINDOW_H
