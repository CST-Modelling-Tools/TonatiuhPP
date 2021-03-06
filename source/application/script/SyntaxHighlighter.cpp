#include "SyntaxHighlighter.h"

// https://regex101.com/

SyntaxHighlighter::SyntaxHighlighter(QTextDocument* parent):
    QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    const QString keywordPatterns[] = {
        "var", "new",
        "function", "return",
        "if", "else",
        "for", "continue", "break"
    };
    for (const QString& pattern : keywordPatterns) {
        rule.pattern = QRegularExpression("\\b" + pattern + "\\b"); // word boundary \\b
        rule.format = keywordFormat;
        highlightingRules << rule;
    }

    // . any single character
    // zero or one ?, zero or more *, one or more +
    // ? if added after another quantifier switches the matching mode from greedy to lazy
    QTextCharFormat classFormat;
    classFormat.setForeground(Qt::darkMagenta);
    classFormat.setFontWeight(QFont::Bold);
    rule.pattern = QRegularExpression("\\bQ[A-Za-z]+\\b"); // starts with Q
    rule.format = classFormat;
    highlightingRules << rule;

    QTextCharFormat functionFormat;
    functionFormat.setForeground(QColor("#3465A4"));
    functionFormat.setFontItalic(true);
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()"); // positive lookahead (?=.) for (
    rule.format = functionFormat;
    highlightingRules << rule;

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("\".*?\""); // in quotes, .*?,  ? for lazy match
    rule.format = quotationFormat;
    highlightingRules << rule;

    singleLineCommentFormat.setForeground(Qt::gray);
    singleCommentExpression = QRegularExpression("[\".*?\"]*//[^\n]*");

    multiLineCommentFormat.setForeground(Qt::gray);
    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}

void SyntaxHighlighter::highlightBlock(const QString& text)
{
    for (const HighlightingRule& rule : highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    // single comments
    QRegularExpressionMatchIterator matchIterator = singleCommentExpression.globalMatch(text);
    while (matchIterator.hasNext()) {
        QRegularExpressionMatch match = matchIterator.next();
        if (format(match.capturedStart()) == quotationFormat) continue; // for "//
        setFormat(match.capturedStart(), match.capturedLength(), singleLineCommentFormat);
    }

    // multiline comments
    setCurrentBlockState(0); // default state

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1); // comment state
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
