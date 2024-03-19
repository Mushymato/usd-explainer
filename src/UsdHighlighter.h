// UsdHighlighter widget
#ifndef UsdHighlighter_H
#define UsdHighlighter_H

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QTextCharFormat>
#include <QRegularExpression>
#include <iostream>

#define NORMAL_BLOCK 1
#define MULTILINE_BLOCK 2

class UsdHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    // UsdHighlighter(QObject *parent = nullptr);
    UsdHighlighter(QTextDocument *parent = nullptr);
    struct SyntaxRule
    {
        QString name;
        QTextCharFormat format;
        QRegularExpression pattern;
        QRegularExpression patternEnd;
        bool isRange = false;
        bool isMultiline = false;
    };
    struct SubBlock
    {
        const SyntaxRule *rule = nullptr;
        QString content;
        qsizetype start = 0;
        qsizetype length = 0;
        static QString descriptionText(struct SubBlock subBlock)
        {
            QString result;
            QTextStream(&result) << "[" << subBlock.start << ", " << subBlock.start + subBlock.length << ") L=" << subBlock.length << "\n"
                                 << " rule: " << subBlock.rule->name << "\n"
                                 << " content '" << subBlock.content;
            return result;
        }
        static void debugPrint(struct SubBlock subBlock)
        {
            // std::cout << "[" << subBlock.start << ", " << subBlock.start + subBlock.length << ") L=" << subBlock.length << "\n"
            //           << " rule: " << subBlock.rule->name.toStdString() << "\n"
            //           << " content '" << subBlock.content.toStdString() << "'\n";
            std::cout << SubBlock::descriptionText(subBlock).toStdString() << "'\n";
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
    QList<SyntaxRule> syntaxRules;
    // QList<SyntaxRule> multilineRules;
    const SyntaxRule *multiRule = nullptr;
    void initSyntaxRules();
    UsdTextBlockData *processBlock(const QString &text);
};

#endif // UsdHighlighter_H
