#ifndef EDITOR_H
#define EDITOR_H

#include "highlighter.h"
#include <QTextEdit>
#include <QFont>
#include <QFontDatabase>

class Editor : public QTextEdit
{
public:
    Editor(QWidget *parent = 0);
    ~Editor();

    Highlighter *highlighter() const;
    void setHighlighter(Highlighter *highlighter);

private:
    Highlighter *_highlighter;
};

#endif // EDITOR_H
