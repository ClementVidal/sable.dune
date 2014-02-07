#include <Dune/Editor/Ctrl/Cmd/CloseProject.h>

#include <Dune/Editor/Ctrl/Project.h>

IMPLEMENT_MANAGED_CLASS1( Dune, CCmdCloseProject, CCmd );

using namespace Dune;

CCmdCloseProject::CCmdCloseProject( CCtrlProject& ctrl )
{
	m_Ctrl = &ctrl;
}

CCmdCloseProject::~CCmdCloseProject()
{
}

Bool CCmdCloseProject::GetIsUndoable() const
{
	return FALSE;
}

Bool CCmdCloseProject::Do()
{
	m_Ctrl->Destroy();
	return TRUE;
}
