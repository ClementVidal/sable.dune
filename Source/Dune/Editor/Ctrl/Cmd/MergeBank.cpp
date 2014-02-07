#include <Dune/Editor/Ctrl/Cmd/MergeBank.h>

IMPLEMENT_MANAGED_CLASS1( Dune, CCmdMergeBank, CCmd );

using namespace Dune;

CCmdMergeBank::CCmdMergeBank( CCtrlProject& ctrl, CBank& srcBank, CBank& dstBank, const CHandle& newRoot )
{
	m_Ctrl = &ctrl;
	m_SrcBank = &srcBank;
	m_DstBank = &dstBank;
	m_NewRoot = newRoot;
}

CCmdMergeBank::~CCmdMergeBank()
{
}

Bool CCmdMergeBank::Undo()
{
	Sb::CMap<CHandle,CHandle>::Iterator it;

	ForEachItem( it, m_MappingTable )
	{
		if(! m_DstBank->RemoveObject( *it ) )
			return FALSE;
	}

	return TRUE;
}

Bool CCmdMergeBank::Do()
{
	if( !m_DstBank->Merge( *m_SrcBank, m_NewRoot, m_MappingTable ) )
		return FALSE;

	return TRUE;
}
