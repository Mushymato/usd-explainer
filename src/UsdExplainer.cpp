// UsdExplainer
#include "UsdExplainer.h"
#include <string>
#include <iostream>

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

    // std::cout << "highlightBlock: '" << text.toStdString() << "'\n";

    setCurrentBlockState(0); // possibly in a multiline block

    if (previousBlockState() == 0) // prev in a multiline block
    {
        if (multiRule != nullptr)
        {
            std::cout << "multiRule: " << multiRule->name.toStdString() << "\n";
        }
        else
        {
            std::cout << "multiRule: NULL\n";
        }
        std::cout << "in multiline block"
                  << "\n";
        result = multiRule->patternEnd.match(text, offset = offset);
        if (result.hasMatch()) // found end, no longer multiline
        {
            setFormat(0, result.capturedEnd(), multiRule->format);
            offset = result.capturedEnd();
        }
        else
        { // no end, continue
            setFormat(0, length, multiRule->format);
            return;
        }
    }

    while (offset < length)
    {
        minRule = nullptr;
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
        {
            break;
        }
        offset = minMatch.capturedEnd();
        if (minRule->isRange)
        {
            // is range, search for end
            result = minRule->patternEnd.match(text, offset = offset);
            if (result.hasMatch())
            {
                // found end match on this line
                setFormat(minMatch.capturedStart(), result.capturedEnd() - minMatch.capturedStart(), minRule->format);
                offset = result.capturedEnd();
            }
            else if (minRule->isMultiline)
            {
                // no end found, but rule is multiline so continue to next text block
                multiRule = minRule;
                setFormat(minMatch.capturedStart(), length, multiRule->format);
                return;
            }
        }
        else
        {
            // not range, update format & offset with this match
            setFormat(minMatch.capturedStart(), minMatch.capturedLength(), minRule->format);
        }
    }

    setCurrentBlockState(1); // not in a multiline block
}
