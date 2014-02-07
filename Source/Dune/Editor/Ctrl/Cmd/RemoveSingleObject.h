#ifndef _DUNE_EDITOR_CTRL_CMD_REMOVESINGLEOBJECT_
#define _DUNE_EDITOR_CTRL_CMD_REMOVESINGLEOBJECT_

#include <Dune/Editor/Ctrl/Cmd/Cmd.h>



namespace Dune
{

class CCtrlProject;

/** 
*/
class CCmdRemoveSingleObject : public CCmd
{

	DEFINE_MANAGED_CLASS( CCmdRemoveSingleObject );

public:

    /** @name Constructor/Destructor*/
    //@{
    CCmdRemoveSingleObject( CCtrlProject& ctrl, const CHandle& obj );
    virtual ~CCmdRemoveSingleObject();
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
	CCtrlProject*		m_Ctrl;
	CHandle				m_ObjectHandle;
	Sb::CRef<CBankObject>	m_Object;
	CHandle				m_Parent;
	const Sb::CTypeInfo*	m_Type;

    // Method

};

}

#endif
