#ifndef _DUNE_EDITOR_CTRL_COMMANDPROCESSOR_
#define _DUNE_EDITOR_CTRL_COMMANDPROCESSOR_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/ManagedObject/ManagedObject.h>
#include <Sable/Core/Event/Header.h>
#include <Sable/Core/ManagedObject/Ref.h>
#include <Sable/Core/String/String.h>

namespace Dune
{

class CHandle;
class CCmd;
class CPropertyInterface;
class CApplication;
class CProject;

/** 
Controller manager class
*/
class CCommandProcessor : public Sb::CManagedObject
{

public:

	/** @name Constructor/Destructor*/
	//@{
	typedef Sb::CList< Sb::CRef<CCmd> > CmdStack;
	//@}

    /** @name Constructor/Destructor*/
    //@{
    CCommandProcessor( CApplication& app );
    ~CCommandProcessor();
    //@}

    /** @name Accessor*/
    //@{
	const CmdStack& GetUndoStack() const;
	const CmdStack& GetRedoStack() const;
	//@}

    /** @name Manipulator*/
    //@{
	Void	Reset();
	Bool	Submit( CCmd& command, Bool store = TRUE );
	Bool	Undo();
	Bool	Redo();
	//@}

	/** @name Signals*/
	//@{
	Sb::CEventSignal< >			SignalCmdStackChanged;
	//@}

private:

	// DataTypes

	// Methods
	Void OnProjectCreated( CProject& h );

    // Attributes
	CApplication*	m_App;
	CmdStack		    m_UndoStack;
	CmdStack		    m_RedoStack;
	Bool			    m_CanSubmit;

};

}

#endif
