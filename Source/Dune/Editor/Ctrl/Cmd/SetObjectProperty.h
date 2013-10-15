#ifndef _DUNE_EDITOR_CTRL_CMD_SETOBJECTPROPERTY_
#define _DUNE_EDITOR_CTRL_CMD_SETOBJECTPROPERTY_

#include <Dune\Editor\Ctrl\Cmd\Cmd.h>
#include <Dune\Model\Bank\Bank.h>



namespace Dune
{

class CPropertyInterface;
class CCtrlProject;

/** 
*/
class CCmdSetObjectProperty : public CCmd
{

	DEFINE_MANAGED_CLASS( CCmdSetObjectProperty );

public:

    /** @name Constructor/Destructor*/
    //@{
	template <class T>
	CCmdSetObjectProperty( CCtrlProject& ctrl, CPropertyInterface& p, const T& oldValue, const T& newValue, UInt32 index = 0 );
	template <class T>
	CCmdSetObjectProperty( CCtrlProject& ctrl, CPropertyInterface& p, const T& newValue, UInt32 index = 0 );

    ~CCmdSetObjectProperty();
    //@}

    /** @name Accessor*/
    //@{
	Sb::CString GetTextualDesc() const;
    Void    SetNewValue( String val );
    Void    SetOldValue( String val );
	//@}

    /** @name Manipulator*/
    //@{    
	Bool	Do();
	Bool	Undo();
	//@}

private:

	// Methods

    // Attributes
	CCtrlProject*			m_Ctrl;
	CHandle					m_Object;
	CPropertyInterface*		m_Property;
	Sb::CString				m_OldValue;
	Sb::CString				m_NewValue;
	UInt32					m_ArrayIndex;

    // Method

};

template <class T>
CCmdSetObjectProperty::CCmdSetObjectProperty( CCtrlProject& ctrl, CPropertyInterface& p, const T& newValue, UInt32 index )
{
	m_Ctrl = &ctrl;
	m_Property = &p;
	m_Object = p.GetObj()->GetHandle();
	m_OldValue = p.GetValueAsString();
	m_NewValue = CLexicalCast::ValueToString( newValue );
	m_ArrayIndex = index;
}

template <class T>
CCmdSetObjectProperty::CCmdSetObjectProperty( CCtrlProject& ctrl, CPropertyInterface& p, const T& oldValue, const T& newValue, UInt32 index )
{
	m_Ctrl = &ctrl;
	m_Property = &p;
	m_Object = p.GetObj()->GetHandle();
	m_OldValue = CLexicalCast::ValueToString( oldValue );
	m_NewValue = CLexicalCast::ValueToString( newValue );
	m_ArrayIndex = index;
}

}

#endif
