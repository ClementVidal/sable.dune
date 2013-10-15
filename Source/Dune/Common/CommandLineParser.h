#ifndef _DUNE_COMMON_COMMANDLINEPARSER_
#define _DUNE_COMMON_COMMANDLINEPARSER_

#include <Sable\Core\Common\DataTypes.h>

#include <QtCore\QString>
#include <QtCore\QStringList>
#include <QtCore\QList>

namespace Dune
{

/** 
*/
class CCommandLineParser
{

public:

	/** @name Constructor/Destructor*/
    //@{
	CCommandLineParser( );
	~CCommandLineParser();
    //@}

	/** @name Accessor*/
    //@{
    Bool    HasFlag( const QString& flag );
    Bool    HasOption( const QString& flag );
    QString GetOptionValue( const QString& flag );
    Bool    HasParameter( UInt32 pos );
    QString GetParameter( UInt32 pos );
    //@}

	/** @name Manipulator*/
	//@{
    Void    Parse( const QStringList& cmdLine );
	//@}

private:

    //Types
    struct SOption
    {
        QString Name;
        QString Value;
    };

    //Methods

    //Attributes
    QStringList         m_FlagList;
    QList<SOption>      m_OptionList;
    QList<QString>      m_ParamList;

};


}

#endif