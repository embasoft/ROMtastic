#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpression>

class Highlighter : public QSyntaxHighlighter
{
public:
    Highlighter(QTextDocument *parent = 0);
    ~Highlighter();
protected:
    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;
private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;

    QTextCharFormat compilerFormat;
    QTextCharFormat routineFormat;
    QTextCharFormat commentFormat;
    QTextCharFormat literalFormat;
};

#endif // HIGHLIGHTER_H
