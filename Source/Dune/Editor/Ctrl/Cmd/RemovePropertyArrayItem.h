#ifndef _DUNE_EDITOR_CTRL_CMD_REMOVEPROPERTYARRAYITEM_
#define _DUNE_EDITOR_CTRL_CMD_REMOVEPROPERTYARRAYITEM_

#include <Dune/Editor/Ctrl/Cmd/Cmd.h>

namespace Dune
{

class CCtrlProject;
class CPropertyInterface;
class CPropertyArray;

/** 
*/
class CCmdRemovePropertyArrayItem : public CCmd
{

	DEFINE_MANAGED_CLASS( CCmdRemovePropertyArrayItem );

public:

    /** @name Constructor/Destructor*/
    //@{
    CCmdRemovePropertyArrayItem( CCtrlProject& ctrl, CPropertyArray& propArray, CPropertyInterface& prop );
    ~CCmdRemovePropertyArrayItem();
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
	Sb::CRef<CPropertyInterface>	m_Property;
	Sb::CRef<CPropertyArray>	    m_PropertyArray;

    // Method

};

}

#endif
