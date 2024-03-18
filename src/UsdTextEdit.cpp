// UsdTextEdit

#include "UsdTextEdit.h"
#include "UsdHighlighter.h"
#include <QObject>
#include <QFile>

UsdTextEdit::UsdTextEdit(QWidget *parent) : QTextEdit(parent)
{
    QObject::connect(this, &UsdTextEdit::cursorPositionChanged, this, &UsdTextEdit::printTextBlockData);
}

void UsdTextEdit::setUsdFile(const QString &filePath)
{
    QFile file(filePath);
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        setPlainText(file.readAll());
    }
}

void UsdTextEdit::printTextBlockData()
{
    QTextCursor cursor = textCursor();
    QTextBlock block = cursor.block();
    UsdHighlighter::UsdTextBlockData *blockData = (UsdHighlighter::UsdTextBlockData *)block.userData();
    int positionInBlock = cursor.positionInBlock();
    std::cout << "printTextBlockData: " << cursor.positionInBlock() << "\n";
    // blockData->debugPrint();
    for (UsdHighlighter::SubBlock &subBlock : blockData->subBlocks)
    {
        if (subBlock.start <= positionInBlock && subBlock.start + subBlock.length > positionInBlock)
        {
            UsdHighlighter::SubBlock::debugPrint(subBlock);
        }
    }
}