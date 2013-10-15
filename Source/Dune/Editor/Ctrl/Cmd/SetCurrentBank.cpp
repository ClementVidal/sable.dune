#include <Dune\Editor\Ctrl\Cmd\SetCurrentBank.h>

IMPLEMENT_MANAGED_CLASS1( Dune, CCmdSetCurrentBank, CCmd );

using namespace Dune;

CCmdSetCurrentBank::CCmdSetCurrentBank( CCtrlProject& ctrl, const CUuid& uuid )
{
	m_Ctrl = &ctrl;
	m_NewUuid = uuid;

    if( m_Ctrl->GetCurrentBank() )
        m_OldUuid = m_Ctrl->GetCurrentBank()->GetUuid();
}

CCmdSetCurrentBank::~CCmdSetCurrentBank()
{
}

Bool CCmdSetCurrentBank::Undo()
{
    return m_Ctrl->SetCurrentBank( m_OldUuid );
}

Bool CCmdSetCurrentBank::Do()
{
    return m_Ctrl->SetCurrentBank( m_NewUuid );
}

