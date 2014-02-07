#include <Dune/Editor/Ctrl/Cmd/Meta.h>

IMPLEMENT_MANAGED_CLASS1( Dune, CCmdMeta, CCmd );

using namespace Dune;

CCmdMeta::CCmdMeta( String name )
{
	m_Name = name;
}

CCmdMeta::~CCmdMeta()
{
}

UInt32	CCmdMeta::GetCommandCount() const
{
	return m_CmdTable.GetItemCount();
}

Sb::CString	CCmdMeta::GetTextualDesc() const
{
	return m_Name;
}

Bool CCmdMeta::Undo()
{
	Int32 i = 0;
	for(i=m_CmdTable.GetItemCount()-1;i>=0;i--)
	{
		m_CmdTable.GetItemAtIndex( i )->Undo();
	}
	return TRUE;
}

Void CCmdMeta::PushCommand( CCmd& cmd )
{
	m_CmdTable.PushItem( &cmd );
}

Bool CCmdMeta::Do()
{
	CmdList::Iterator it;

	ForEachItem( it, m_CmdTable )
	{
		(*it)->Do();
	}

	return TRUE;
}
