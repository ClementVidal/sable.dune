#ifndef _DUNE_EDITOR_CTRL_CMD_CREATEPROJECT_
#define _DUNE_EDITOR_CTRL_CMD_CREATEPROJECT_

#include <Dune/Editor/Ctrl/Cmd/Cmd.h>
#include <Sable/Core/File/FilePath.h>



namespace Dune
{

class CCtrlProject;

/** 
*/
class CCmdCreateProject : public CCmd
{

	DEFINE_MANAGED_CLASS( CCmdCreateProject );

public:

    /** @name Constructor/Destructor*/
    //@{
    CCmdCreateProject( CCtrlProject& ctrl, const Sb::CFilePath& path );
    virtual ~CCmdCreateProject();
    //@}

    /** @name Accessor*/
    //@{
	Bool	GetIsUndoable() const ;
	//@}

    /** @name Manipulator*/
    //@{    
	Bool	Do();
	//@}

private:

	// Methods

    // Attributes
	Sb::CFilePath		m_Path;
	CCtrlProject*	m_Ctrl;
    // Method

};

}

#endif
