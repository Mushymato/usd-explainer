// UsdTextEdit

#include "UsdTextEdit.h"
#include "UsdHighlighter.h"
#include <QObject>
#include <QFile>
#include <QToolTip>

UsdTextEdit::UsdTextEdit(QWidget *parent) : QTextEdit(parent)
{
    QObject::connect(this, &UsdTextEdit::cursorPositionChanged, this, &UsdTextEdit::handleTextCursorChange);
}

void UsdTextEdit::setUsdFile(const QString &filePath)
{
    QFile file(filePath);
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        setPlainText(file.readAll());
    }
}

void UsdTextEdit::printTextBlockData(QTextCursor currentCursor, QPoint pos)
{
    QTextBlock block = currentCursor.block();
    if (!block.isValid() || !block.isVisible())
        return;
    int positionInBlock = currentCursor.positionInBlock();
    std::cout << "printTextBlockData: '" << block.text().toStdString() << "' pos: " << positionInBlock << "\n";
    UsdHighlighter::UsdTextBlockData *blockData = (UsdHighlighter::UsdTextBlockData *)block.userData();
    UsdHighlighter::UsdTextBlockData::debugPrint(*blockData);
    for (UsdHighlighter::SubBlock &subBlock : blockData->subBlocks)
    {
        if (subBlock.range.first <= positionInBlock && subBlock.range.first + subBlock.range.second > positionInBlock)
        {
            UsdHighlighter::SubBlock::debugPrint(subBlock);
            QToolTip::showText(pos, UsdHighlighter::SubBlock::descriptionText(subBlock, block.text()), this);
        }
    }
}

// void UsdTextEdit::mouseReleaseEvent(QMouseEvent *event)
// {
//     QTextCursor currentCursor = textCursor();
//     printTextBlockData(currentCursor, event->globalPosition().toPoint());
// }

void UsdTextEdit::handleTextCursorChange()
{
    QTextCursor currentCursor = textCursor();
    QRect rect = cursorRect(currentCursor);
    QPoint pos = viewport()->mapToGlobal(rect.topLeft());
    printTextBlockData(currentCursor, pos);
}