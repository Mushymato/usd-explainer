// app window
#include <QtWidgets>
#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setupFileMenu();
    setupHelpMenu();
    setupEditor();

    setCentralWidget(editor);
    setWindowTitle(tr("USD Explainer"));
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("About USD Explainer"),
                       tr("<p>Highlight and explain syntax components in a USD file</p>"));
}

// void MainWindow::newFile()
// {
//     editor->clear();
// }

void MainWindow::openFile(const QString &path)
{
    QString fileName = path;

    if (fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "USD (*.usda *.usdz *.usd)");

    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text))
            editor->setPlainText(file.readAll());
    }
}

void MainWindow::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    editor = new QTextEdit;
    editor->setFont(font);

    explainer = new UsdExplainer(editor->document());

    QFile file("test/Bogus.usd");
    if (file.open(QFile::ReadOnly | QFile::Text))
        editor->setPlainText(file.readAll());
}

void MainWindow::setupFileMenu()
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);

    // fileMenu->addAction(tr("&New"), QKeySequence::New,
    //                     this, &MainWindow::newFile);
    fileMenu->addAction(tr("&Open..."), QKeySequence::Open,
                        this, [this]()
                        { openFile(); });
    fileMenu->addAction(tr("E&xit"), QKeySequence::Quit,
                        qApp, &QApplication::quit);
}

void MainWindow::setupHelpMenu()
{
    QMenu *helpMenu = new QMenu(tr("&Help"), this);
    menuBar()->addMenu(helpMenu);

    helpMenu->addAction(tr("&About"), this, &MainWindow::about);
    helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
}
