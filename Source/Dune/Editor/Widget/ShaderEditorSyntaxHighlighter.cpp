#include <Dune/Editor/Widget/ShaderEditorSyntaxHighlighter.h> 

using namespace Dune;

CShaderEditorSyntaxHighlighter::CShaderEditorSyntaxHighlighter( QTextDocument* parent ) : 
    QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    QTextCharFormat keywordFormat;
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bstruct\\b" << "\\buniform\\b" << "\\breturn\\b" << "\\bconst\\b"
                    << "\\bfloat\\b" << "\\bfloat2\\b" << "\\bfloat3\\b" << "\\bfloat4\\b" << "\\bbool\\b" << "\\bint\\b" << "\\bfloat4x4\\b" << "\\bfloat3x3\\b"
                    << "\\bTexture2D\\b" << "\\bTexture3D\\b" << "\\bTextureCube\\b" << "\\bSamplerState\\b";

    Q_FOREACH(const QString &pattern, keywordPatterns) 
    {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    QTextCharFormat semanticFormat;
    semanticFormat.setForeground(Qt::darkRed);
    semanticFormat.setFontWeight(QFont::Bold);
    QStringList semanticPatterns;
    semanticPatterns << "\\bPOSITION\\d*\\b" << "\\bBINORMAL\\d*\\b" << "\\bBLENDINDICES\\d*\\b" << "\\bBLENDWEIGHT\\d*\\b" << "\\bCOLOR\\d*\\b"  
                    << "\\bNORMAL\\d*\\b" << "\\bPOSITIONT\\d*\\b" << "\\bPSIZE\\d*\\b" << "\\bTANGENT\\d*\\b" << "\\bTEXCOORD\\d*\\b" << "\\bDEPTH\\d*\\b" 
                    << "\\bSV_POSITION\\b" << "\\bSV_INSTANCEID\\b" << "\\bSV_PRIMITIVEID\\b" << "\\bSV_VERTEXID\\b" ;

    Q_FOREACH(const QString &pattern, semanticPatterns) 
    {
        rule.pattern = QRegExp(pattern);
        rule.format = semanticFormat;
        highlightingRules.append(rule);
    }

    QTextCharFormat quotationFormat;
    quotationFormat.setForeground(Qt::darkRed);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    QTextCharFormat functionFormat;
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    QTextCharFormat singleLineCommentFormat;
    singleLineCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::darkGreen);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

void CShaderEditorSyntaxHighlighter::highlightBlock(const QString &text)
{
    Q_FOREACH(const HighlightingRule &rule, highlightingRules) 
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) 
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0) 
    {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) 
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else 
        {
            commentLength = endIndex - startIndex
                + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}