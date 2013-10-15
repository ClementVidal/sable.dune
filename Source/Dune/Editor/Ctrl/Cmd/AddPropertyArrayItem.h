#ifndef _DUNE_EDITOR_CTRL_CMD_ADDPROPERTYARRAYITEM_
#define _DUNE_EDITOR_CTRL_CMD_ADDPROPERTYARRAYITEM_

#include <Dune\Editor\Ctrl\Cmd\Cmd.h>

namespace Dune
{

class CCtrlProject;
class CPropertyInterface;
class CPropertyArray;

/** 
*/
class CCmdAddPropertyArrayItem : public CCmd
{

	DEFINE_MANAGED_CLASS( CCmdAddObjectProperty );

public:

    /** @name Constructor/Destructor*/
    //@{
    CCmdAddPropertyArrayItem( CCtrlProject& ctrl, CPropertyArray& propArray, CPropertyInterface& prop, Int32 index = -1 );
    ~CCmdAddPropertyArrayItem();
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
	Sb::CRef<CPropertyArray>	    m_PropertyArray;
	Sb::CRef<CPropertyInterface>	m_Property;
	Int32							m_Index;

    // Method

};

}

#endif
