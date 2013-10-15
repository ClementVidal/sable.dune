#ifndef _DUNE_COMMON_LOG_
#define _DUNE_COMMON_LOG_

#include <Sable\Core\Common\DataTypes.h>
#include <Sable\Core\Log\Handler.h>



namespace Dune
{

/** 
*/
class CDuneLogHandler : public Sb::CLogHandler
{

public:

	/** @name Constructor/Destructor*/
    //@{
	CDuneLogHandler( Sb::CLogger& logger  );
	~CDuneLogHandler();
    //@}

	/** @name Accessor*/
    //@{
    //@}

	/** @name Manipulator*/
	//@{
    Void Log( UInt32 lvl, String header, String msg );
	//@}

	/** @name Manipulator*/
	//@{
	Sb::CEventSignal< UInt32, String, String >	SignalLog;
	//@}

private:

    //Types

    //Methods

    //Attributes

};


}

#endif