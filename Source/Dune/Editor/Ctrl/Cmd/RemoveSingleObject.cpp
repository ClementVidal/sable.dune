#include <Dune/Editor/Ctrl/Cmd/RemoveSingleObject.h>

IMPLEMENT_MANAGED_CLASS1( Dune, CCmdRemoveSingleObject, CCmd );

using namespace Dune;

CCmdRemoveSingleObject::CCmdRemoveSingleObject( CCtrlProject& ctrl, const CHandle& obj )
{
	m_Ctrl = &ctrl;
	m_ObjectHandle = obj;
	m_Parent = m_Ctrl->GetProject()->GetObj( obj )->GetParent();

	m_Type = &m_Ctrl->GetProject()->GetObj( m_ObjectHandle )->GetTypeInfo();
}

CCmdRemoveSingleObject::~CCmdRemoveSingleObject()
{
}

Bool CCmdRemoveSingleObject::Undo()
{
	if( m_Parent.IsValid() )
	{
		m_Ctrl->AddObject( m_Parent, m_ObjectHandle, *m_Object );
		return m_ObjectHandle.IsValid();
	}

	return TRUE;
}

Bool CCmdRemoveSingleObject::Do()
{
	if( m_ObjectHandle.IsValid() )
	{
		m_Object = m_Ctrl->GetProject()->GetObj( m_ObjectHandle );
		m_Ctrl->RemoveObject( m_ObjectHandle, FALSE );
		return TRUE;
	}
	
	return FALSE;
}
