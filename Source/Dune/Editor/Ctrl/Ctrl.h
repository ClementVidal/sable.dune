#ifndef _DUNE_EDITOR_CTRL_CTRL_
#define _DUNE_EDITOR_CTRL_CTRL_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/ManagedObject/ManagedObject.h>

#include <Dune/Editor/Ctrl/EventClient.h>



namespace Dune
{

class CApplication;
class CPropertyInterface;
class CBankObject;
class CCtrlProject;
class CCtrlEditorUI;
class CCtrlAsset;
class CProject;
class CBank;
class CHandle;

/** 
Base controller class
*/
class CCtrl : public Sb::CManagedObject, public CEventClient
{

public:

    /** @name Constructor/Destructor*/
    //@{
    CCtrl( CApplication& app );
    virtual ~CCtrl();
    //@}

    /** @name Accessor*/
    //@{
	//@}

    /** @name Manipulator*/
    //@{
    //@}

private:

    // Methods

    // Attributes

};

}

#endif
