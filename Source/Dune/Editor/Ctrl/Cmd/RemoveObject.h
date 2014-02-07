#ifndef _DUNE_EDITOR_CTRL_CMD_REMOVEOBJECT_
#define _DUNE_EDITOR_CTRL_CMD_REMOVEOBJECT_

#include <Dune/Editor/Ctrl/Cmd/Meta.h>



namespace Dune
{

class CBankObject;
class CAsset;
class CProject;
class CCtrlProject;

/** 
*/
class CCmdRemoveObject : public CCmdMeta
{

	DEFINE_MANAGED_CLASS( CCmdRemoveObject );

public:

    /** @name Constructor/Destructor*/
    //@{
    CCmdRemoveObject( CCtrlProject& ctrl, const CHandle& obj );
    virtual ~CCmdRemoveObject();
    //@}

    /** @name Accessor*/
    //@{
	//@}

    /** @name Manipulator*/
    //@{    
	//@}

private:

	// DataTypes

    // Method
	Void PushCommands( CBankObject& elem );

	// Attributes
	CCtrlProject*		m_Ctrl;
};

}

#endif
