#include <Dune\Editor\Ctrl\Cmd\SetObjectProperty.h>

#include <Dune\Model\Property\Interface.h>
#include <Dune\Model\Bank\Object.h>

IMPLEMENT_MANAGED_CLASS1( Dune, CCmdSetObjectProperty, CCmd );

using namespace Dune;


CCmdSetObjectProperty::~CCmdSetObjectProperty()
{
}

Void CCmdSetObjectProperty::SetNewValue( String val )
{
    m_NewValue = val;
}

Void CCmdSetObjectProperty::SetOldValue( String val )
{
    m_OldValue = val;
}

Bool CCmdSetObjectProperty::Undo()
{
	if( m_Object.IsValid() )
	{
        CPropertyInterface* p = m_Property;
        if( m_Property->GetTypeInfo().IsKindOf( CPropertyArray::GetStaticTypeInfo() ) )
        {
            p = & ((CPropertyArray*)m_Property)->GetItemAtIndex( m_ArrayIndex );
        }

        if( p->SetValueFromString( m_OldValue ) )
            return TRUE;
	}

	return FALSE;
}

Bool CCmdSetObjectProperty::Do()
{
	if( m_Object.IsValid() )
	{
        CPropertyInterface* p = m_Property;
        if( m_Property->GetTypeInfo().IsKindOf( CPropertyArray::GetStaticTypeInfo() ) )
        {
            p = & ((CPropertyArray*)m_Property)->GetItemAtIndex( m_ArrayIndex );
        }

        if( p->SetValueFromString( m_NewValue ) )
            return TRUE;
	}

	return FALSE;
}


Sb::CString CCmdSetObjectProperty::GetTextualDesc() const
{
	CBankObject* obj = m_Ctrl->GetProject()->GetObj( m_Object );
	if( obj )
	{
		return Sb::CString( "SetObjectProperty: ") + obj->GetName() + " " + m_Property->GetName();
	}
	else
	{
		return Sb::CString( "SetObjectProperty");
	}
	
}
