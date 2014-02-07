#include <Dune/Editor/Ctrl/Cmd/LoadProject.h>

#include <Dune/Editor/Ctrl/Project.h>

IMPLEMENT_MANAGED_CLASS1( Dune, CCmdLoadProject, CCmd );

using namespace Dune;

CCmdLoadProject::CCmdLoadProject( CCtrlProject& ctrl, const Sb::CFilePath& path )
{
	m_Ctrl = &ctrl;
	m_Path = path;
}

CCmdLoadProject::~CCmdLoadProject()
{
}

Bool CCmdLoadProject::GetIsUndoable() const
{
	return FALSE;
}

Bool CCmdLoadProject::Do()
{
	if( ! m_Ctrl->Load( m_Path ) )
		return FALSE;

	return TRUE;
}
