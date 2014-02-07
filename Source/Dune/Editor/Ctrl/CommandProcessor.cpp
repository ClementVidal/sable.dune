#include <Dune/Editor/Ctrl/CommandProcessor.h>

#include <Dune/Model/Project/Project.h>
#include <Dune/Editor/Ctrl/Cmd/Cmd.h>

#include <Dune/Common/LexicalCast.h>

using namespace Dune;

CCommandProcessor::CCommandProcessor( CApplication& app )
{
	m_App = &app;

	m_CanSubmit = TRUE;	
}


CCommandProcessor::~CCommandProcessor()
{

}

const CCommandProcessor::CmdStack& CCommandProcessor::GetUndoStack() const
{
	return m_UndoStack;
}

const CCommandProcessor::CmdStack& CCommandProcessor::GetRedoStack() const
{
	return m_RedoStack;
}

Void CCommandProcessor::Reset()
{
	m_UndoStack.ClearAllItems();
	m_RedoStack.ClearAllItems();
	m_CanSubmit = TRUE;
}

Bool CCommandProcessor::Submit( CCmd& command, Bool store )
{
	Sb::CRef<CCmd> cmd = &command;
	DebugAssert( m_CanSubmit );
	Bool result = cmd->Do();
	if( result == FALSE )
	{
		Sb::DebugLogWarning( "\tCommand %s (%s), failed", cmd->GetTextualDesc().GetBuffer(), cmd->GetTypeInfo().GetTypeName() );
	}

	if( result && store && cmd->GetIsUndoable() )
	{
		m_UndoStack.PushItem( cmd );
		m_RedoStack.ClearAllItems();
		SignalCmdStackChanged();
	}

	return result;
}

Bool CCommandProcessor::Undo()
{
	if( m_UndoStack.GetItemCount() == 0 )
		return FALSE;

	Sb::CRef<CCmd> cmd = m_UndoStack.GetLastItem();

	m_CanSubmit = FALSE;

	Bool result = cmd->Undo();

	m_UndoStack.PopItem();
	if( cmd->GetIsRedoable() )
		m_RedoStack.PushItem( cmd );
	
	SignalCmdStackChanged();
	m_CanSubmit = TRUE;
	return result;
}

Bool CCommandProcessor::Redo()
{
	if( m_RedoStack.GetItemCount() == 0 )
		return FALSE;

	Sb::CRef<CCmd> cmd = m_RedoStack.GetLastItem();
	m_CanSubmit = FALSE;
	Bool result = cmd->Do();

	m_RedoStack.PopItem();
	m_UndoStack.PushItem( cmd );
	SignalCmdStackChanged();
	m_CanSubmit = TRUE;

	return result;
}