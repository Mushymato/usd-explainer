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
    //! constructor
    UsdHighlighter(QTextDocument *parent = nullptr);
    //! struct describing info pertaining a section of the text block matched to a syntax rule
    struct SubBlock
    {
        //! syntax rule that matched this SubBlock
        const SyntaxRule *rule = nullptr;
        //! range (start, length) of this SubBlock relative to the block
        RANGE range;
        //! range of capturing groups in this SubBlock
        QList<RANGE> captures;
        //! text description to be shown in tooltip
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
            std::cout << SubBlock::debugText(subBlock).toStdString() << "\n";
        }
    };
    //! QTextBlockUserData subclass for storage on QTextDocument
    struct UsdTextBlockData : public QTextBlockUserData
    {
        //! list of discovered SubBlocks for this text block
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
    //! process text block for syntax highlighting
    void highlightBlock(const QString &text) override;

private:
    //! cached multi-line syntax rule
    const SyntaxRule *multiRule = nullptr;
    //! process text block for syntax highlighting and construct new UsdTextBlockData
    UsdTextBlockData *processBlock(const QString &text);
};

#endif // USDHIGHLIGHTER_H
