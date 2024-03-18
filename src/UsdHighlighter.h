// UsdHighlighter widget
#ifndef UsdHighlighter_H
#define UsdHighlighter_H

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QTextCharFormat>
#include <QRegularExpression>

#define NORMAL_BLOCK 1
#define MULTILINE_BLOCK 2

class UsdHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    // UsdHighlighter(QObject *parent = nullptr);
    UsdHighlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
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
    };
    struct UsdTextBlockData : public QTextBlockUserData
    {
        QList<SubBlock> subBlocks;
    };
    struct BogusBlockData : public QTextBlockUserData
    {
        int someValue;
    };
    QList<SyntaxRule> syntaxRules;
    // QList<SyntaxRule> multilineRules;
    const SyntaxRule *multiRule = nullptr;

    void initSyntaxRules();
    UsdTextBlockData *processBlock(const QString &text);
};

#endif // UsdHighlighter_H
