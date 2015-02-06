#include "editor.h"
#include <QDebug>

Editor::Editor(QWidget *parent) : QTextEdit(parent)
{
    // Set the font to monospace
    QFont newFont = QFontDatabase::systemFont(QFontDatabase::GeneralFont);
    newFont.setStyleHint(QFont::Monospace);
    setFont(newFont);
}

Editor::~Editor()
{

}
Highlighter *Editor::highlighter() const
{
    return _highlighter;
}

void Editor::setHighlighter(Highlighter *highlighter)
{
    _highlighter = highlighter;
}


