#include <Dune/Editor/Ctrl/Cmd/AddObjectFromFBX.h>

IMPLEMENT_MANAGED_CLASS1( Dune, CCmdAddObjectFromFBX, CCmd );

using namespace Dune;

CCmdAddObjectFromFBX::CCmdAddObjectFromFBX( CCtrlProject& ctrl, const CHandle& parent, const Sb::CTypeInfo&  type, CFBXObject& obj )
{
	m_Ctrl = &ctrl;
	m_Parent = parent;
	m_Type = &type;
	m_FBXObject = &obj;
}

CCmdAddObjectFromFBX::~CCmdAddObjectFromFBX()
{
}

Bool CCmdAddObjectFromFBX::Undo()
{
	if( m_ObjectHandle.IsValid() )
	{
		m_Ctrl->RemoveObject( m_ObjectHandle, FALSE );
	}

	return TRUE;
}

Bool CCmdAddObjectFromFBX::Do()
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
			m_ObjectHandle = m_Ctrl->AddObjectFromFBX( m_Parent, *m_Type, *m_FBXObject );
			if( m_ObjectHandle.IsValid() )
			{
				m_Object = m_Ctrl->GetProject()->GetObj( m_ObjectHandle );
				return TRUE;
			}
		}
	}
	return FALSE;
}
