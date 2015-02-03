#include "highlighter.h"
#include <QDebug>

Highlighter::Highlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    // compiler related

    compilerFormat.setForeground(Qt::darkBlue);
    compilerFormat.setFontWeight(QFont::Bold);

    QString compilerPattern= "\\.[a-z]*\\b";
    rule.pattern = QRegularExpression(compilerPattern);
    rule.format = compilerFormat;
    highlightingRules.append(rule);

    // routines

    routineFormat.setFontWeight(QFont::Bold);

    QString routinePattern = "^\\w*:";
    rule.pattern = QRegularExpression(routinePattern);
    rule.format = routineFormat;
    highlightingRules.append(rule);

    // comments

    commentFormat.setForeground(Qt::darkGreen);

    QString commentPattern = "\\@.*$";
    rule.pattern = QRegularExpression(commentPattern);
    rule.format = commentFormat;
    highlightingRules.append(rule);

    // hex numbers

    literalFormat.setForeground(Qt::darkYellow);

    QString literalPattern = "0x[0-9a-fA-F]+";
    rule.pattern = QRegularExpression(literalPattern);
    rule.format = literalFormat;
    highlightingRules.append(rule);
}

Highlighter::~Highlighter()
{

}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegularExpression expression(rule.pattern);
        expression.setPatternOptions(QRegularExpression::MultilineOption);
        QRegularExpressionMatchIterator iterator = expression.globalMatch(text);
        while (iterator.hasNext())
        {
            QRegularExpressionMatch match = iterator.next();
            int length = match.captured().length();
            int index = match.capturedStart();
            setFormat(index, length, rule.format);
        }
    }
}
