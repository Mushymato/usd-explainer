// singleton class describing rules for USD language
#ifndef USDHIGHLIGHTERSYNTAX_H
#define USDHIGHLIGHTERSYNTAX_H

#include <QTextCharFormat>
#include <QRegularExpression>

//! struct describing a specific syntax element
struct SyntaxRule
{
    //! readable rule name
    QString name;
    //! list of text styles to apply; the 0th item is the overall style, and each item after corresponds to capture group
    QList<QTextCharFormat> styles;
    //! regex describing the syntax
    QRegularExpression pattern;
    //! terminating pattern, if the rule refers to a range (e.g. quoted strings)
    QRegularExpression patternEnd;
    //! true if this rule describes a range
    bool isRange = false;
    //! true if this rule should be matched across text blocks (lines)
    bool isMultiline = false;
    //! text to show in tooltip
    QString label;
};

class UsdHighlighterSyntax
{
public:
    //! constructor
    UsdHighlighterSyntax();
    //! return singleton instance of UsdHighlighterSyntax
    static UsdHighlighterSyntax &getInstance()
    {
        static UsdHighlighterSyntax instance;
        return instance;
    }
    //! list of all syntax rules
    QList<SyntaxRule> syntaxRules;
};

#endif // USDHIGHLIGHTERSYNTAX_H
