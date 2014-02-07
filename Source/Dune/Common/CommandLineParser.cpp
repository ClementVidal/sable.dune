#include <Dune/Common/CommandLineParser.h>

#include <QtCore/QRegExp>

using namespace Dune;

/**
*/
CCommandLineParser::CCommandLineParser( )
{

}

CCommandLineParser::~CCommandLineParser()
{
}

Void CCommandLineParser::Parse( const QStringList& cmdLine )
{
    Int32 i;
    // parse flags
    for(i=0;i<cmdLine.size();i++)
    {
        QRegExp flagRegExp( "-(\\w)" );
        QString arg = cmdLine[i];
        if( flagRegExp.exactMatch(arg) )
        {
            QString flag = flagRegExp.cap( 1 );
            m_FlagList.push_back( flag );
        }
    }

    // Parse options
    for(i=0;i<cmdLine.size();i++)
    {
        QRegExp flagRegExp( "-(\\w+)\\=(.+)" );
        QString arg = cmdLine[i];
        if( flagRegExp.exactMatch(arg) )
        {
            QRegExp brackectRegExp( "(.+)" );
            SOption param;
            param.Name = flagRegExp.cap( 1 );
            param.Value = flagRegExp.cap( 2 );

            // Remove brackets if necessary
            if( brackectRegExp.exactMatch(param.Value) )
            {
                param.Value = brackectRegExp.cap( 1 );
            }

            m_OptionList.push_back( param );
        }
    }
}

Bool CCommandLineParser::HasFlag( const QString& flag )
{
    return m_FlagList.indexOf( flag ) != -1;
}

Bool CCommandLineParser::HasOption( const QString& name )
{
    SOption param;
    Q_FOREACH( param, m_OptionList )
    {
        if( param.Name == name )
        {
            return TRUE;
        }
    }

    return FALSE;
}

QString CCommandLineParser::GetOptionValue( const QString& name )
{
    SOption param;
    Q_FOREACH( param, m_OptionList )
    {
        if( param.Name == name )
        {
            return param.Value;
        }
    }

    return "";
}

Bool CCommandLineParser::HasParameter( UInt32 pos )
{
    return FALSE;
}

QString CCommandLineParser::GetParameter( UInt32 pos )
{
    return "";
}
