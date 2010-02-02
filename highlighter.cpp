#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
     : QSyntaxHighlighter(parent)
 {
     HighlightingRule rule;

     /* we define what we want */
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    quotationFormat1.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("/.*");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    Comment.setForeground(Qt::red);
    rule.pattern = QRegExp("^#.*$");
    rule.format = Comment;
    highlightingRules.append(rule);

    UseFlag.setFontWeight(QFont::Bold);
    rule.pattern = QRegExp("^[A-Z0-9_]+");
    rule.format = UseFlag;
    highlightingRules.append(rule);

 }

 void Highlighter::highlightBlock(const QString &text)
 {
     foreach (const HighlightingRule &rule, highlightingRules) {
         QRegExp expression(rule.pattern);
         int index = expression.indexIn(text);
         while (index >= 0) {
             int length = expression.matchedLength();
             setFormat(index, length, rule.format);
             index = expression.indexIn(text, index + length);
         }
     }
     setCurrentBlockState(0);
 }
