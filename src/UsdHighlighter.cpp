// UsdHighlighter
#include "UsdHighlighter.h"

UsdHighlighter::UsdHighlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
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
            subBlock = (SubBlock){.rule = multiRule,
                                  .range = RANGE(0, result.capturedEnd()),
                                  .captures = QList<RANGE>()};
            blockData->subBlocks.append(subBlock);
            offset = result.capturedEnd();
        }
        else
        { // no end, continue
            subBlock = (SubBlock){.rule = multiRule,
                                  .range = RANGE(0, length),
                                  .captures = QList<RANGE>()};
            blockData->subBlocks.append(subBlock);
            return blockData;
        }
    }

    while (offset < length)
    {
        minRule = nullptr;
        minMatch = QRegularExpressionMatch();
        for (const SyntaxRule &rule : std::as_const(UsdHighlighterSyntax::getInstance().syntaxRules))
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
                subBlock = (SubBlock){.rule = minRule,
                                      .range = RANGE(minMatch.capturedStart(), result.capturedEnd() - minMatch.capturedStart()),
                                      .captures = QList<RANGE>()};
                blockData->subBlocks.append(subBlock);
                offset = result.capturedEnd();
            }
            else if (minRule->isMultiline)
            {
                // no end found, but rule is multiline so continue to next text block
                multiRule = minRule;
                subBlock = (SubBlock){.rule = multiRule,
                                      .range = RANGE(minMatch.capturedStart(), length - minMatch.capturedStart()),
                                      .captures = QList<RANGE>()};
                blockData->subBlocks.append(subBlock);
                return blockData;
            }
        }
        else
        {
            // not range, update format & offset with this match
            subBlock = (SubBlock){.rule = minRule,
                                  .range = RANGE(minMatch.capturedStart(), minMatch.capturedLength()),
                                  .captures = QList<RANGE>()};
            for (int i = 1; i <= minRule->pattern.captureCount(); i++)
            {
                subBlock.captures.append(std::pair(minMatch.capturedStart(i), minMatch.capturedLength(i)));
            }
            blockData->subBlocks.append(subBlock);
        }
    }

    setCurrentBlockState(NORMAL_BLOCK); // not in a multiline block
    return blockData;
}

void UsdHighlighter::highlightBlock(const QString &text)
{
    UsdTextBlockData *blockData = processBlock(text);
    // std::cout << "BLOCK: '" << text.toStdString() << "'\n";
    for (const SubBlock &subBlock : std::as_const(blockData->subBlocks))
    {
        // UsdHighlighter::SubBlock::debugPrint(subBlock);
        setFormat(subBlock.range.first, subBlock.range.second, subBlock.rule->styles[0]);
        for (int i = 0; i < subBlock.captures.length(); i++)
        {
            RANGE range = subBlock.captures[i];
            if (range.first == -1)
            {
                continue;
            }
            if (i + 1 < subBlock.rule->styles.length())
            {
                setFormat(range.first, range.second, subBlock.rule->styles[i + 1]);
            }
            else
            {
                setFormat(range.first, range.second, subBlock.rule->styles[0]);
            }
        }
    }
    setCurrentBlockUserData(blockData);
}
