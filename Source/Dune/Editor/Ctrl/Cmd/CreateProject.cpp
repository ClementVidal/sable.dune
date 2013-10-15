#include <Dune\Editor\Ctrl\Cmd\CreateProject.h>

#include <Dune\Editor\Ctrl\Project.h>

IMPLEMENT_MANAGED_CLASS1( Dune, CCmdCreateProject, CCmd );

using namespace Dune;

CCmdCreateProject::CCmdCreateProject( CCtrlProject& ctrl, const Sb::CFilePath& path )
{
	m_Ctrl = &ctrl;
	m_Path = path;
}

CCmdCreateProject::~CCmdCreateProject()
{
}

Bool CCmdCreateProject::GetIsUndoable() const
{
	return FALSE;
}

Bool CCmdCreateProject::Do()
{
	CProject* prj = m_Ctrl->Create( m_Path );
	if( prj )
		return TRUE;

	return FALSE;
}
