#ifndef _DUNE_EDITOR_CTRL_CMD_SETBANKOBJECTHIERARCHY_
#define _DUNE_EDITOR_CTRL_CMD_SETBANKOBJECTHIERARCHY_

#include <Dune/Editor/Ctrl/Cmd/Cmd.h>

namespace Dune
{

class CCtrlProject;
class CPropertyInterface;

/** 
*/
class CCmdSetObjectHierarchy : public CCmd
{

	DEFINE_MANAGED_CLASS( CCmdSetObjectHierarchy );

public:

    /** @name Constructor/Destructor*/
    //@{
    CCmdSetObjectHierarchy( CCtrlProject& ctrl, const CHandle& bankObject, const CHandle& parent, Int32 childIndex );
    ~CCmdSetObjectHierarchy();
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
	CCtrlProject*				m_Ctrl;
	CHandle						m_ObjectHandle;
	CHandle						m_ParentHandle;
	Int32						m_Index;

	CHandle						m_OldParent;
	Int32						m_OldIndex;

    // Method

};

}

#endif
