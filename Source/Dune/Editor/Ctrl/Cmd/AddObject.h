#ifndef _DUNE_EDITOR_CTRL_CMD_ADDELEMENT_
#define _DUNE_EDITOR_CTRL_CMD_ADDELEMENT_

#include <Dune/Editor/Ctrl/Cmd/Cmd.h>
#include <Dune/Model/Bank/Bank.h>



namespace Dune
{

class CCtrlProject;

/** 
*/
class CCmdAddObject : public CCmd
{

	DEFINE_MANAGED_CLASS( CCmdAddObject );

public:

    /** @name Constructor/Destructor*/
    //@{
    CCmdAddObject( CCtrlProject& ctrl, const CHandle& parent, const Sb::CTypeInfo& type );
    virtual ~CCmdAddObject();
    //@}

    /** @name Accessor*/
    //@{
	CHandle	GetAddedObject() const;
	//@}

    /** @name Manipulator*/
    //@{    
	Bool	Do();
	Bool	Undo();
	//@}

private:

	// Methods

    // Attributes
	CCtrlProject*		m_Ctrl;
	CHandle				m_Parent;
	CHandle				m_ObjectHandle;
	Sb::CRef<CBankObject>	m_Object;
	const Sb::CTypeInfo*	m_Type;
    // Method

};

}

#endif
