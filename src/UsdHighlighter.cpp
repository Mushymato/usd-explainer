// UsdHighlighter
#include "UsdHighlighter.h"
#include <iostream>

UsdHighlighter::UsdHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    initSyntaxRules();
}

UsdHighlighter::UsdTextBlockData *UsdHighlighter::processBlock(const QString &text)
{
    qsizetype offset = 0;
    qsizetype length = text.length();

    QRegularExpressionMatch result;
    QRegularExpressionMatch minMatch = QRegularExpressionMatch();
    const SyntaxRule *minRule;
    UsdTextBlockData *blockData = new UsdTextBlockData();
    SubBlock subBlock = SubBlock();

    setCurrentBlockState(MULTILINE_BLOCK); // possibly in a multiline block

    if (previousBlockState() == 0) // prev in a multiline block
    {
        result = multiRule->patternEnd.match(text, offset = offset);
        if (result.hasMatch()) // found end, no longer multiline
        {
            // setFormat(0, result.capturedEnd(), multiRule->format);
            subBlock = (SubBlock){.rule = multiRule,
                                  .content = text.sliced(0, result.capturedEnd() - result.capturedLength()),
                                  .start = 0,
                                  .length = result.capturedEnd()};
            blockData->subBlocks.append(subBlock);
            offset = result.capturedEnd();
        }
        else
        { // no end, continue
            // setFormat(0, length, multiRule->format);
            subBlock = (SubBlock){.rule = multiRule,
                                  .content = text.sliced(
                                      result.capturedEnd(),
                                      length - result.capturedEnd()),
                                  .start = 0,
                                  .length = length};
            blockData->subBlocks.append(subBlock);
            return blockData;
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
                // setFormat(minMatch.capturedStart(), result.capturedEnd() - minMatch.capturedStart(), minRule->format);
                subBlock = (SubBlock){.rule = minRule,
                                      .content = text.sliced(
                                          minMatch.capturedEnd(),
                                          result.capturedStart() - minMatch.capturedEnd()),
                                      .start = minMatch.capturedStart(),
                                      .length = result.capturedEnd() - minMatch.capturedStart()};
                blockData->subBlocks.append(subBlock);
                offset = result.capturedEnd();
            }
            else if (minRule->isMultiline)
            {
                // no end found, but rule is multiline so continue to next text block
                multiRule = minRule;
                // setFormat(minMatch.capturedStart(), length, multiRule->format);
                subBlock = (SubBlock){.rule = multiRule,
                                      .content = text.sliced(
                                          minMatch.capturedEnd(),
                                          length - minMatch.capturedEnd()),
                                      .start = minMatch.capturedStart(),
                                      .length = length - minMatch.capturedStart()};
                blockData->subBlocks.append(subBlock);
                return blockData;
            }
        }
        else
        {
            // not range, update format & offset with this match
            // setFormat(minMatch.capturedStart(), minMatch.capturedLength(), minRule->format);
            subBlock = (SubBlock){.rule = minRule,
                                  .content = minMatch.captured(1),
                                  .start = minMatch.capturedStart(),
                                  .length = minMatch.capturedLength()};
            blockData->subBlocks.append(subBlock);
        }
    }

    setCurrentBlockState(NORMAL_BLOCK); // not in a multiline block
    return blockData;
}

void UsdHighlighter::highlightBlock(const QString &text)
{
    std::cout << "BLOCK: '" << text.toStdString() << "'\n";
    UsdTextBlockData *blockData = processBlock(text);
    for (const SubBlock &subBlock : std::as_const(blockData->subBlocks))
    {
        std::cout << "subBlock " << subBlock.start << "+" << subBlock.length << "\n"
                  << "\trule: " << subBlock.rule->name.toStdString() << "\n"
                  << "\tcontent '" << subBlock.content.toStdString() << "'\n";
        setFormat(subBlock.start, subBlock.length, subBlock.rule->format);
    }

    setCurrentBlockUserData(blockData);
}
