// UsdExplainer
#include "UsdExplainer.h"

UsdExplainer::UsdExplainer(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    initSyntaxRules();
}

void UsdExplainer::highlightBlock(const QString &text)
{
    qsizetype offset = 0;
    qsizetype length = text.length();

    QRegularExpressionMatch result;
    QRegularExpressionMatch minMatch = QRegularExpressionMatch();
    const SyntaxRule *minRule;

    while (offset < length)
    {
        minMatch = QRegularExpressionMatch();
        for (const SyntaxRule &rule : std::as_const(syntaxRules))
        {
            result = rule.pattern.match(text, offset = offset);
            if (result.hasMatch())
            {
                if (!minMatch.hasMatch() || minMatch.capturedStart() > result.capturedStart())
                {
                    minMatch = result;
                    minRule = &rule;
                }
            }
        }
        // nothing matched, stop iteration
        if (!minMatch.hasMatch())
            break;
        offset = minMatch.capturedEnd();
        if (minRule->isRange)
        {
            // is range, search for end
            result = minRule->patternEnd.match(text, offset = offset);
            if (result.hasMatch())
            {
                setFormat(minMatch.capturedStart(), result.capturedEnd() - minMatch.capturedStart(), minRule->format);
                offset = result.capturedEnd();
            }
        }
        else
        {
            // not range, update format & offset with this match
            setFormat(minMatch.capturedStart(), minMatch.capturedLength(), minRule->format);
        }
    }
}
