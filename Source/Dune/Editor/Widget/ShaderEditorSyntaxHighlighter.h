#ifndef _DUNE_EDITOR_WIDGET_SHADEREDITORSYNTAXHIGHLIGHTER_
#define _DUNE_EDITOR_WIDGET_SHADEREDITORSYNTAXHIGHLIGHTER_

#include <Sable\Core\Common\DataTypes.h>
#include <QtGui\QSyntaxHighlighter>

namespace Dune
{

class CShaderEditorSyntaxHighlighter : public QSyntaxHighlighter
{

    Q_OBJECT

public:

    CShaderEditorSyntaxHighlighter(QTextDocument *parent = 0);

protected:

    void highlightBlock(const QString &text);

private:

    struct HighlightingRule
    {
        QRegExp pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;

    QRegExp commentStartExpression;
    QRegExp commentEndExpression;
    QTextCharFormat multiLineCommentFormat;

};

};

#endif