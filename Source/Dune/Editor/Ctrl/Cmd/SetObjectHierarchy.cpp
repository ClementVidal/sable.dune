#include <Dune\Editor\Ctrl\Cmd\SetObjectHierarchy.h>

IMPLEMENT_MANAGED_CLASS1( Dune, CCmdSetObjectHierarchy, CCmd );

using namespace Dune;

CCmdSetObjectHierarchy::CCmdSetObjectHierarchy( CCtrlProject& ctrl, const CHandle& bankObject, const CHandle& parent, Int32 childIndex )
{
	m_Ctrl = &ctrl;
	m_ObjectHandle = bankObject;
	m_ParentHandle = parent;
	m_Index = childIndex;

	m_OldParent = m_Ctrl->GetProject()->GetObj( m_ObjectHandle )->GetParent();
	m_OldIndex = -1;
	if( m_OldParent.IsValid() )
	{
		m_OldIndex = m_Ctrl->GetProject()->GetObj( m_OldParent )->GetChildIndex( m_ObjectHandle );
		DebugAssert( m_OldIndex != -1 );
	}
}

CCmdSetObjectHierarchy::~CCmdSetObjectHierarchy()
{
}

Bool CCmdSetObjectHierarchy::Undo()
{
	if( m_ObjectHandle.IsValid() )
	{
		m_Ctrl->GetProject()->GetObj( m_ObjectHandle )->SetParent( m_OldParent );
		if( m_OldIndex != -1 && m_OldParent.IsValid() )
		{
			Int32 previousIndex = m_Ctrl->GetProject()->GetObj( m_OldParent )->GetChildIndex( m_ObjectHandle );
			DebugAssert( previousIndex != -1 );
			m_Ctrl->GetProject()->GetObj( m_OldParent )->SetChildIndex( previousIndex, m_OldIndex );
		}
		return TRUE;
	}
	return FALSE;
}

Bool CCmdSetObjectHierarchy::Do()
{
	if( m_ObjectHandle.IsValid() )
	{
		m_Ctrl->GetProject()->GetObj( m_ObjectHandle )->SetParent( m_ParentHandle );
		if( m_Index != -1 && m_ParentHandle.IsValid() )
		{
			Int32 previousIndex = m_Ctrl->GetProject()->GetObj( m_ParentHandle )->GetChildIndex( m_ObjectHandle );
			DebugAssert( previousIndex != -1 );
			m_Ctrl->GetProject()->GetObj( m_ParentHandle )->SetChildIndex( previousIndex, m_Index );
		}
		return TRUE;
	}
	return FALSE;
}
