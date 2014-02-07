#ifndef _DUNE_EDITOR_CTRL_CMD_ADDOBJECTPROPERTY_
#define _DUNE_EDITOR_CTRL_CMD_ADDOBJECTPROPERTY_

#include <Dune/Editor/Ctrl/Cmd/Cmd.h>

namespace Dune
{

class CCtrlProject;
class CPropertyInterface;

/** 
*/
class CCmdAddObjectProperty : public CCmd
{

	DEFINE_MANAGED_CLASS( CCmdAddObjectProperty );

public:

    /** @name Constructor/Destructor*/
    //@{
    CCmdAddObjectProperty( CCtrlProject& ctrl, const CHandle& object, const CPropertyInterface& prop );
    ~CCmdAddObjectProperty();
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
	Sb::CRef<CPropertyInterface>	m_Property;

    // Method

};

}

#endif
