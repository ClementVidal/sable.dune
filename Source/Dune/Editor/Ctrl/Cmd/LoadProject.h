#ifndef _DUNE_EDITOR_CTRL_CMD_LOADPROJECT_
#define _DUNE_EDITOR_CTRL_CMD_LOADPROJECT_

#include <Dune\Editor\Ctrl\Cmd\Cmd.h>

#include <Sable\Core\File\FilePath.h>



namespace Dune
{

class CProject;
class CCtrlProject;

/** 
*/
class CCmdLoadProject : public CCmd
{

	DEFINE_MANAGED_CLASS( CCmdLoadProject );

public:

    /** @name Constructor/Destructor*/
    //@{
    CCmdLoadProject( CCtrlProject& ctrl, const Sb::CFilePath& path );
    virtual ~CCmdLoadProject();
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
