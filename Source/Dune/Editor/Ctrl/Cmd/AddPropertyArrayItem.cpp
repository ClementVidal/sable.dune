#include <Dune/Editor/Ctrl/Cmd/AddPropertyArrayItem.h>

IMPLEMENT_MANAGED_CLASS1( Dune, CCmdAddPropertyArrayItem, CCmd );

using namespace Dune;

CCmdAddPropertyArrayItem::CCmdAddPropertyArrayItem( CCtrlProject& ctrl, CPropertyArray& propArray, CPropertyInterface& prop, Int32 index )
{
	m_Ctrl = &ctrl;
    m_PropertyArray = &propArray;
	m_Property = &prop;
	m_Index = index;
}

CCmdAddPropertyArrayItem::~CCmdAddPropertyArrayItem()
{
}

Bool CCmdAddPropertyArrayItem::Undo()
{
    m_PropertyArray->RemoveItem( *m_Property );

    return TRUE;
}

Bool CCmdAddPropertyArrayItem::Do()
{
	if( m_Index == -1 )
	{
		m_PropertyArray->AddItemAtEnd( *m_Property );
		return TRUE;
	}
	else
	{
		if( m_Index >= 0 && m_Index < (Int32)m_PropertyArray->GetItemCount() )
		{
			m_PropertyArray->AddItemAtIndex( *m_Property, m_Index );
			return TRUE;
		}
	}

	return FALSE;
}
