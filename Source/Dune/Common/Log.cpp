#include <Dune/Common/Log.h>

#include <Sable/Core/Common/Manager.h>

using namespace Dune;

/**
Dune Log Handler:
Pass log niformations to wxWidgets log system
*/
CDuneLogHandler::CDuneLogHandler( Sb::CLogger& logger ) :
	CLogHandler( logger )
{

}

CDuneLogHandler::~CDuneLogHandler()
{
}

Void CDuneLogHandler::Log( UInt32 lvl, String header, String msg )
{
	SignalLog( lvl, header, msg );
}
