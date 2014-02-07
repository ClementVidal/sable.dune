#include <Dune/Editor/Ctrl/Cmd/AddObject.h>

IMPLEMENT_MANAGED_CLASS1( Dune, CCmdAddObject, CCmd );

using namespace Dune;

CCmdAddObject::CCmdAddObject( CCtrlProject& ctrl, const CHandle& parent, const Sb::CTypeInfo& type )
{
	m_Ctrl = &ctrl;
	m_Parent = parent;
	m_Type = &type;
}

CCmdAddObject::~CCmdAddObject()
{
}

Bool CCmdAddObject::Undo()
{
	if( m_ObjectHandle.IsValid() )
	{
		m_Ctrl->RemoveObject( m_ObjectHandle, FALSE );
	}

	return TRUE;
}

CHandle	CCmdAddObject::GetAddedObject() const
{
	return m_ObjectHandle;
}

Bool CCmdAddObject::Do()
{
	if( m_Parent.IsValid() )
	{
		CBank* bank = m_Ctrl->GetProject()->GetBank( m_ObjectHandle.GetBankUuid() );

		if( bank && bank->IsReserved( m_ObjectHandle ) )	
		{
			m_Ctrl->AddObject( m_Parent, m_ObjectHandle, *m_Object );
			return TRUE;
		}
		else
		{
			m_ObjectHandle = m_Ctrl->AddObject( m_Parent, *m_Type );
			if( m_ObjectHandle.IsValid() )
			{
				m_Object = m_Ctrl->GetProject()->GetObj( m_ObjectHandle );
				return TRUE;
			}
		}
	}
	return FALSE;
}
