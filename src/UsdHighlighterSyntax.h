// singleton class describing rules for USD language
#ifndef USDHIGHLIGHTERSYNTAX_H
#define USDHIGHLIGHTERSYNTAX_H

#include <QTextCharFormat>
#include <QRegularExpression>

struct SyntaxRule
{
    QString name;
    QList<QTextCharFormat> styles;
    QRegularExpression pattern;
    QRegularExpression patternEnd;
    bool isRange = false;
    bool isMultiline = false;
    QString label;
};

class UsdHighlighterSyntax
{
public:
    UsdHighlighterSyntax();
    static UsdHighlighterSyntax &getInstance()
    {
        static UsdHighlighterSyntax instance; // Guaranteed to be destroyed.
                                              // Instantiated on first use.
        return instance;
    }
    QList<SyntaxRule> syntaxRules;
};

#endif // USDHIGHLIGHTERSYNTAX_H
