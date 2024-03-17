// UsdHighlighter widget
#ifndef UsdHighlighter_H
#define UsdHighlighter_H

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QTextCharFormat>
#include <QRegularExpression>

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
    QList<SyntaxRule> syntaxRules;
    // QList<SyntaxRule> multilineRules;
    void initSyntaxRules();
    const SyntaxRule *multiRule = nullptr;
};

#endif // UsdHighlighter_H
