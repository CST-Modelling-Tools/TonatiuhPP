#pragma once

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class QTextDocument;


class SyntaxHighlighter: public QSyntaxHighlighter
{
    Q_OBJECT

public:
    SyntaxHighlighter(QTextDocument* parent = 0);

protected:
    void highlightBlock(const QString& text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat quotationFormat;

    QRegularExpression singleCommentExpression;
    QTextCharFormat singleLineCommentFormat;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;
    QTextCharFormat multiLineCommentFormat;
};
