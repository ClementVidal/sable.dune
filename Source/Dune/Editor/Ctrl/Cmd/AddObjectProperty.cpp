#include <Dune\Editor\Ctrl\Cmd\AddObjectProperty.h>

IMPLEMENT_MANAGED_CLASS1( Dune, CCmdAddObjectProperty, CCmd );

using namespace Dune;

CCmdAddObjectProperty::CCmdAddObjectProperty( CCtrlProject& ctrl, const CHandle& object, const CPropertyInterface& prop )
{
	m_Ctrl = &ctrl;
	m_ObjectHandle = object;
	m_Property = &prop;
}

CCmdAddObjectProperty::~CCmdAddObjectProperty()
{
}

Bool CCmdAddObjectProperty::Undo()
{
	if( m_ObjectHandle.IsValid() )
	{
		m_Ctrl->GetProject()->GetObj( m_ObjectHandle )->RemoveProperty( *m_Property );
		return TRUE;
	}
	return FALSE;
}

Bool CCmdAddObjectProperty::Do()
{
	if( m_ObjectHandle.IsValid() )
	{
		m_Ctrl->GetProject()->GetObj( m_ObjectHandle )->AddProperty( *m_Property );
		return TRUE;
	}
	return FALSE;
}
