#ifndef _DUNE_EDITOR_CTRL_CMD_CLOSEPROJECT_
#define _DUNE_EDITOR_CTRL_CMD_CLOSEPROJECT_

#include <Dune\Editor\Ctrl\Cmd\Cmd.h>



namespace Dune
{

class CProject;
class CCtrlProject;

/** 
*/
class CCmdCloseProject : public CCmd
{

	DEFINE_MANAGED_CLASS( CCmdCloseProject );

public:

    /** @name Constructor/Destructor*/
    //@{
    CCmdCloseProject( CCtrlProject& ctrl );
    virtual ~CCmdCloseProject();
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

    // Method

};

}

#endif
