// UsdHighlighter widget
#ifndef USDHIGHLIGHTER_H
#define USDHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <iostream>
#include "UsdHighlighterSyntax.h"

#define NORMAL_BLOCK 1
#define MULTILINE_BLOCK 2
// pair of start and length, which defines a range
#define RANGE std::pair<qsizetype, qsizetype>

class UsdHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    // UsdHighlighter(QObject *parent = nullptr);
    UsdHighlighter(QTextDocument *parent = nullptr);
    struct SubBlock
    {
        const SyntaxRule *rule = nullptr;
        RANGE range;
        QList<RANGE> captures;
        static QString descriptionText(struct SubBlock subBlock, QString blockText)
        {
            // QString result = QString(subBlock.rule->label);
            QString result;
            QTextStream(&result) << "<p><i>" << blockText.sliced(subBlock.range.first, subBlock.range.second) << "</i></p>"
                                 << "<p>" << QString(subBlock.rule->label) << "</p>";
            return result;
        }
        static QString debugText(struct SubBlock subBlock)
        {
            QString result;
            QTextStream(&result) << "[" << subBlock.range.first << ", " << subBlock.range.first + subBlock.range.second << ") L=" << subBlock.range.second
                                 << " rule: " << subBlock.rule->name;
            return result;
        }
        static void debugPrint(struct SubBlock subBlock)
        {
            // std::cout << "[" << subBlock.start << ", " << subBlock.start + subBlock.length << ") L=" << subBlock.length << "\n"
            //           << " rule: " << subBlock.rule->name.toStdString() << "\n"
            //           << " content '" << subBlock.content.toStdString() << "'\n";
            std::cout << SubBlock::debugText(subBlock).toStdString() << "\n";
        }
    };
    struct UsdTextBlockData : public QTextBlockUserData
    {
        QList<SubBlock> subBlocks;
        static void debugPrint(UsdTextBlockData blockData)
        {
            for (const SubBlock &subBlock : std::as_const(blockData.subBlocks))
            {
                SubBlock::debugPrint(subBlock);
            }
        }
    };

protected:
    void highlightBlock(const QString &text) override;

private:
    const SyntaxRule *multiRule = nullptr;
    UsdTextBlockData *processBlock(const QString &text);
};

#endif // USDHIGHLIGHTER_H
