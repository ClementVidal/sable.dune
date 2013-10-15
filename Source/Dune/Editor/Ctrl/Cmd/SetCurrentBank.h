#ifndef _DUNE_EDITOR_CTRL_CMD_SETCURRENTBANK_
#define _DUNE_EDITOR_CTRL_CMD_SETCURRENTBANK_

#include <Dune\Editor\Ctrl\Cmd\Cmd.h>
#include <Sable\Core\File\FilePath.h>

namespace Dune
{

class CUuid;
class CCtrlProject;

/** 
*/
class CCmdSetCurrentBank : public CCmd
{

	DEFINE_MANAGED_CLASS( CCmdSetCurrentBank );

public:

    /** @name Constructor/Destructor*/
    //@{
    CCmdSetCurrentBank( CCtrlProject& ctrl, const CUuid& uuid );
    ~CCmdSetCurrentBank();
    //@}

    /** @name Accessor*/
    //@{
	//@}

    /** @name Manipulator*/
    //@{    
	Bool	Do();
    Bool	Undo();
	//@}

private:

	// Methods

    // Attributes
    CUuid               m_OldUuid;
	CUuid		        m_NewUuid;
	CCtrlProject*	    m_Ctrl;

    // Method

};

}

#endif
