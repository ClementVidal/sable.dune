#include <Dune/Editor/Ctrl/Cmd/Cmd.h>

IMPLEMENT_MANAGED_CLASS1( Dune, CCmd, Sb::CManagedObject );

using namespace Dune;

CCmd::CCmd( )
{
}

CCmd::~CCmd()
{
}

Sb::CString CCmd::GetTextualDesc() const
{
	return GetTypeInfo().GetTypeName();
}

Bool CCmd::GetIsUndoable() const
{
	return TRUE;
}

Bool CCmd::GetIsRedoable() const
{
	return TRUE;
}

Bool CCmd::Undo()
{
	return FALSE;
}