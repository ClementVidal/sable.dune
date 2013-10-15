#include <Dune\Editor\Ctrl\Cmd\RemoveObjectProperty.h>

IMPLEMENT_MANAGED_CLASS1( Dune, CCmdRemoveObjectProperty, CCmd );

using namespace Dune;

CCmdRemoveObjectProperty::CCmdRemoveObjectProperty( CCtrlProject& ctrl, const CHandle& object, const CPropertyInterface& prop )
{
	m_Ctrl = &ctrl;
	m_ObjectHandle = object;
	m_Property = &prop;
}

CCmdRemoveObjectProperty::~CCmdRemoveObjectProperty()
{
}

Bool CCmdRemoveObjectProperty::Undo()
{
	if( m_ObjectHandle.IsValid() )
	{
		m_Ctrl->GetProject()->GetObj( m_ObjectHandle )->AddProperty( *m_Property );
		return TRUE;
	}
	return FALSE;
}

Bool CCmdRemoveObjectProperty::Do()
{
	if( m_ObjectHandle.IsValid() )
	{
		m_Ctrl->GetProject()->GetObj( m_ObjectHandle )->RemoveProperty( *m_Property );
		return TRUE;
	}
	return FALSE;
}
