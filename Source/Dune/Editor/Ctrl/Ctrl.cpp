#include <Dune/Editor/Ctrl/Ctrl.h>

using namespace Dune;


CCtrl::CCtrl( CApplication& app ) :
	CEventClient( app )
{
}

CCtrl::~CCtrl()
{
}
