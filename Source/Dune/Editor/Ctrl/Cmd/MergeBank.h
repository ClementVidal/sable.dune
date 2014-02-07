#ifndef _DUNE_EDITOR_CTRL_CMD_MERGEBANK_
#define _DUNE_EDITOR_CTRL_CMD_MERGEBANK_

#include <Dune/Editor/Ctrl/Cmd/Cmd.h>
#include <Dune/Model/Bank/Bank.h>

namespace Dune
{

class CCtrlProject;
class CBank;

/** 
*/
class CCmdMergeBank : public CCmd
{

	DEFINE_MANAGED_CLASS( CCmdMergeBank );

public:

    /** @name Constructor/Destructor*/
    //@{
    CCmdMergeBank( CCtrlProject& ctrl, CBank& srcBank, CBank& dstBank, const CHandle& newRoot );
    ~CCmdMergeBank();
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
	CHandle						m_NewRoot;
	Sb::CRef<CBank>				m_SrcBank;
	Sb::CRef<CBank>				m_DstBank;
	Sb::CMap<CHandle,CHandle>	m_MappingTable;

    // Method

};

}

#endif
