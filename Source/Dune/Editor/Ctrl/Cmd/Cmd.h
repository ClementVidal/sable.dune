#ifndef _DUNE_EDITOR_CTRL_CMD_CMD_
#define _DUNE_EDITOR_CTRL_CMD_CMD_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/ManagedObject/ManagedObject.h>
#include <Sable/Core/String/String.h>



namespace Dune
{

/** 
*/
class CCmd : public Sb::CManagedObject
{

	DEFINE_MANAGED_CLASS( CCmd );

public:

    /** @name Constructor/Destructor*/
    //@{
    CCmd( );
    virtual ~CCmd();
    //@}

    /** @name Accessor*/
    //@{
	virtual Bool	GetIsUndoable() const ;
	virtual Bool	GetIsRedoable() const ;
	virtual Sb::CString	GetTextualDesc() const;
	//@}

    /** @name Manipulator*/
    //@{    
	virtual Bool	Do() = 0;
	virtual Bool	Undo();
	//@}

private:

	// Methods

    // Attributes

    // Method

};

}

#endif
