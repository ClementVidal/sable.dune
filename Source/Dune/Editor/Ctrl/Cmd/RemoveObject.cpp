#include <Dune\Editor\Ctrl\Cmd\RemoveObject.h>

#include <Dune\Editor\Ctrl\Cmd\RemoveSingleObject.h>
#include <Dune\Model\Bank\Object.h>
#include <Dune\Model\Bank\Bank.h>
#include <Dune\Model\Asset\Asset.h>

IMPLEMENT_MANAGED_CLASS1( Dune, CCmdRemoveObject, CCmd );

using namespace Dune;

CCmdRemoveObject::CCmdRemoveObject( CCtrlProject& ctrl, const CHandle& obj ) :
	CCmdMeta( Sb::CString( "RemoveObject: " ) + ctrl.GetProject()->GetObj( obj )->GetName() )
{
	m_Ctrl = &ctrl;
	PushCommands( * ctrl.GetProject()->GetObj<CBankObject>( obj ) );
}

CCmdRemoveObject::~CCmdRemoveObject()
{
}

Void CCmdRemoveObject::PushCommands( CBankObject& asset )
{
	UInt32 i;
	for(i=0;i<asset.GetChildCount();i++)
	{
		PushCommands( * m_Ctrl->GetProject()->GetObj<CBankObject>( asset.GetChildAtIndex( i ) ) );
	}
	PushCommand( * NEWOBJ( CCmdRemoveSingleObject, ( *m_Ctrl, asset.GetHandle() ) ) );
}
