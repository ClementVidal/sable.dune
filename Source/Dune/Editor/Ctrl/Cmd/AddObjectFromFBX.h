#ifndef _DUNE_EDITOR_CTRL_CMD_ADDELEMENTFROMDAE_
#define _DUNE_EDITOR_CTRL_CMD_ADDELEMENTFROMDAE_

#include <Dune\Editor\Ctrl\Cmd\Cmd.h>

#include <Dune\Model\Asset\FBX\Object.h>

namespace Dune
{

class CFBXDoc;
class CCtrlProject;

/** 
*/
class CCmdAddObjectFromFBX : public CCmd
{

	DEFINE_MANAGED_CLASS( CCmdAddObjectFromFBX );

public:

    /** @name Constructor/Destructor*/
    //@{
    CCmdAddObjectFromFBX( CCtrlProject& ctrl, const CHandle& parent, const Sb::CTypeInfo& type, CFBXObject& obj );
    ~CCmdAddObjectFromFBX();
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
	CCtrlProject*			m_Ctrl;
	CHandle					m_Parent;
	CHandle					m_ObjectHandle;
	Sb::CRef<CBankObject>	m_Object;
	const Sb::CTypeInfo*	m_Type;
	Sb::CRef<CFBXObject>	m_FBXObject;

    // Method

};

}

#endif
