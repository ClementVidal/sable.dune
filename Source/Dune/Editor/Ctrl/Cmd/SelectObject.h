#ifndef _DUNE_EDITOR_CTRL_CMD_SELECTOBJECT_
#define _DUNE_EDITOR_CTRL_CMD_SELECTOBJECT_

#include <Dune/Editor/Ctrl/Cmd/Cmd.h>



namespace Dune
{

class CCtrlProject;
class CBankObject;

/** 
*/
class CCmdSelectObject : public CCmd
{

	DEFINE_MANAGED_CLASS( CCmdSelectObject );

public:

	/** @name DataTypes*/
	//@{
	enum EAction 
	{
		nAction_Select = 0,
		nAction_DeSelect,
		nAction_Toggle,
		nAction_None
	};
	//@}

    /** @name Constructor/Destructor*/
    //@{
    CCmdSelectObject( CCtrlProject& ctrl,  EAction action );
    CCmdSelectObject( CCtrlProject& ctrl, const CHandle& newObj, EAction action );
	CCmdSelectObject( CCtrlProject& ctrl, const CHandle* newObjs, UInt32 count, EAction action );
    virtual ~CCmdSelectObject();
    //@}

    /** @name Manipulator*/
    //@{    
	Bool	Do();
	Bool	Undo();
	//@}

private:

    // Attributes
	EAction			m_Action;
	Sb::CArray<CHandle>	m_SelectionSet;
	CCtrlProject*	m_Ctrl;

    // Method

};

}

#endif
