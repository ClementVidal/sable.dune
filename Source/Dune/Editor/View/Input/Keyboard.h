#ifndef _DUNE_EDITOR_VIEW_INPUT_KEYBOARD_
#define _DUNE_EDITOR_VIEW_INPUT_KEYBOARD_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Input/Keyboard/Interface.h>
#include <Sable/Application/Window.h>


namespace Dune
{

class CKeyboardObserver;

/** 
\ingroup Input
*/
class CViewInputKeyboard : public Sb::CKeyboardInterface
{

	DEFINE_MANAGED_CLASS( CViewInputKeyboard );

public:

	/** @name Constructor/Destructor*/
	//@{
	CViewInputKeyboard( Sb::CApplicationWindow& window );
	~CViewInputKeyboard();
	//@}

	/** @name Operator*/
	//@{
	//@}

	/** @name Accessors*/
	//@{
	Bool	GetIsKeyDown( const EKeyboardEventData key ) const ;
	//@}

	/** @name Manipulator*/
	//@{
	Void	OnKeyDown( const EKeyboardEventData key ) ;
	Void	OnKeyUp( const EKeyboardEventData key ) ;
	Void	Update();

	Void	ReleaseAllKey();
	//@}

private:

	// Attributes
	UInt8 m_KeyStates[nKeyboardEventData_Count];
};


}

#endif


