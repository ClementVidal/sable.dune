#ifndef _DUNE_EDITOR_CTRL_CMD_SAVEPROJECT_
#define _DUNE_EDITOR_CTRL_CMD_SAVEPROJECT_

#include <Dune/Editor/Ctrl/Cmd/Cmd.h>
#include <Sable/Core/File/FilePath.h>



namespace Dune
{

class CProject;
class CCtrlProject;

/** 
*/
class CCmdSaveProject : public CCmd
{

	DEFINE_MANAGED_CLASS( CCmdSaveProject );

public:

    /** @name Constructor/Destructor*/
    //@{
    CCmdSaveProject( CCtrlProject& ctrl, const Sb::CFilePath& path );
    virtual ~CCmdSaveProject();
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
	CCtrlProject*	m_Ctrl;
	Sb::CFilePath		m_Path;
    // Method

};

}

#endif
