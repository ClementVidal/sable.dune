#ifndef _DUNE_EDITOR_CTRL_CMD_META_
#define _DUNE_EDITOR_CTRL_CMD_META_

#include <Dune/Editor/Ctrl/Cmd/Cmd.h>



namespace Dune
{

class CProject;

/** 
*/
class CCmdMeta : public CCmd
{

	DEFINE_MANAGED_CLASS( CCmdMeta );

public:

    /** @name Constructor/Destructor*/
    //@{
    CCmdMeta( String name );
    virtual ~CCmdMeta();
    //@}

    /** @name Accessor*/
    //@{
	Sb::CString	GetTextualDesc() const;
	UInt32	GetCommandCount() const;
	//@}

    /** @name Manipulator*/
    //@{    
	Void	PushCommand( CCmd& cmd );
	Bool	Do();
	Bool	Undo();
	//@}

private:

	// DataTypes
	typedef Sb::CList< Sb::CRef<CCmd> > CmdList;

    // Method

	// Attributes
	CmdList		m_CmdTable;
	Sb::CString		m_Name;
	
};

}

#endif
