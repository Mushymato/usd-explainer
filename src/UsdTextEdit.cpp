// UsdTextEdit

#include "UsdTextEdit.h"
#include "UsdHighlighter.h"
#include <QObject>
#include <QFile>
#include <QToolTip>

UsdTextEdit::UsdTextEdit(QWidget *parent) : QTextEdit(parent)
{
    // QObject::connect(this, &UsdTextEdit::cursorPositionChanged, this, &UsdTextEdit::printTextBlockData);
    // setMouseTracking(true);
}

void UsdTextEdit::setUsdFile(const QString &filePath)
{
    QFile file(filePath);
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        setPlainText(file.readAll());
    }
}

void UsdTextEdit::printTextBlockData(QPoint pos)
{
    QTextCursor currentCursor = textCursor();
    QTextBlock block = currentCursor.block();
    UsdHighlighter::UsdTextBlockData *blockData = (UsdHighlighter::UsdTextBlockData *)block.userData();
    int positionInBlock = currentCursor.positionInBlock();
    std::cout << "printTextBlockData: " << positionInBlock << " (" << pos.x() << ", " << pos.y() << ")\n";
    //  blockData->debugPrint();
    for (UsdHighlighter::SubBlock &subBlock : blockData->subBlocks)
    {
        if (subBlock.start <= positionInBlock && subBlock.start + subBlock.length > positionInBlock)
        {
            UsdHighlighter::SubBlock::debugPrint(subBlock);
            QToolTip::showText(pos, UsdHighlighter::SubBlock::descriptionText(subBlock));
        }
    }
}

void UsdTextEdit::mouseReleaseEvent(QMouseEvent *event)
{
    printTextBlockData(event->globalPosition().toPoint());
}
