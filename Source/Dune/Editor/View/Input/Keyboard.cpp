#include <Dune\Editor\View\Input\Keyboard.h>

using namespace Dune;

IMPLEMENT_MANAGED_CLASS1( Dune, CViewInputKeyboard, Sb::CKeyboardInterface );

CViewInputKeyboard::CViewInputKeyboard( Sb::CApplicationWindow& window ) :
	Sb::CKeyboardInterface( window )
{
	ReleaseAllKey();
}

CViewInputKeyboard::~CViewInputKeyboard()
{

}

Void CViewInputKeyboard::Update()
{

}

Void CViewInputKeyboard::ReleaseAllKey()
{
	Sb::MemoryFill( m_KeyStates, 0, sizeof( UInt8 ) * nKeyboardEventData_Count );
}

Void CViewInputKeyboard::OnKeyDown( const EKeyboardEventData key )
{
	m_KeyStates[key] = TRUE;
}

Void CViewInputKeyboard::OnKeyUp( const EKeyboardEventData key )
{
	m_KeyStates[key] = FALSE;
}

Bool CViewInputKeyboard::GetIsKeyDown( const EKeyboardEventData key ) const 
{
	return m_KeyStates[key];
}
