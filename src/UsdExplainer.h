// UsdExplainer widget
#ifndef USDEXPLAINER_H
#define USDEXPLAINER_H

#include <QSyntaxHighlighter>
#include <QTextDocument>
#include <QTextCharFormat>
#include <QRegularExpression>

class UsdExplainer : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    // UsdExplainer(QObject *parent = nullptr);
    UsdExplainer(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct SyntaxRule
    {
        QTextCharFormat format;
        QRegularExpression pattern;
        QRegularExpression patternEnd;
        bool isRange = false;
    };
    QList<SyntaxRule> syntaxRules;
    void initSyntaxRules();
};

#endif // USDEXPLAINER_H
