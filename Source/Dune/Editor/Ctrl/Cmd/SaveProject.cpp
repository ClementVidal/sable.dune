#include <Dune/Editor/Ctrl/Cmd/SaveProject.h>

#include <Dune/Editor/Ctrl/Project.h>

IMPLEMENT_MANAGED_CLASS1( Dune, CCmdSaveProject, CCmd );

using namespace Dune;

CCmdSaveProject::CCmdSaveProject( CCtrlProject& ctrl, const Sb::CFilePath& path )
{
	m_Ctrl = &ctrl;
	m_Path = path;
}

CCmdSaveProject::~CCmdSaveProject()
{
}

Bool CCmdSaveProject::GetIsUndoable() const
{
	return FALSE;
}

Bool CCmdSaveProject::Do()
{
	m_Ctrl->Save( m_Path );
	return TRUE;
}
