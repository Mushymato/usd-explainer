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

void MainWindow::openFile(const QString &path)
{
    QString fileName = path;

    if (fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", "USD (*.usda *.usdz *.usd)");

    if (!fileName.isEmpty())
    {
        editor->setUsdFile(fileName);
    }
}

void MainWindow::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    editor = new UsdTextEdit;
    editor->setFont(font);

    highlighter = new UsdHighlighter(editor->document());

    editor->setUsdFile("test/bogus.usda");
}

void MainWindow::setupFileMenu()
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);

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
