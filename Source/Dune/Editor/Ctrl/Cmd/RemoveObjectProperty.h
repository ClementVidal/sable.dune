#ifndef _DUNE_EDITOR_CTRL_CMD_REMOVEOBJECTPROPERTY_
#define _DUNE_EDITOR_CTRL_CMD_REMOVEOBJECTPROPERTY_

#include <Dune/Editor/Ctrl/Cmd/Cmd.h>

namespace Dune
{

class CCtrlProject;
class CPropertyInterface;

/** 
*/
class CCmdRemoveObjectProperty : public CCmd
{

	DEFINE_MANAGED_CLASS( CCmdRemoveObjectProperty );

public:

    /** @name Constructor/Destructor*/
    //@{
    CCmdRemoveObjectProperty( CCtrlProject& ctrl, const CHandle& object, const CPropertyInterface& prop );
    virtual ~CCmdRemoveObjectProperty();
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
	CCtrlProject*				    m_Ctrl;
	CHandle						    m_ObjectHandle;
	Sb::CRef<CPropertyInterface>	m_Property;

    // Method

};

}

#endif
