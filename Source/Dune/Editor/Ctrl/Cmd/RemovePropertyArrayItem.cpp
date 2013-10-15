#include <Dune\Editor\Ctrl\Cmd\RemovePropertyArrayItem.h>

IMPLEMENT_MANAGED_CLASS1( Dune, CCmdRemovePropertyArrayItem, CCmd );

using namespace Dune;

CCmdRemovePropertyArrayItem::CCmdRemovePropertyArrayItem( CCtrlProject& ctrl, CPropertyArray& propArray, CPropertyInterface& prop )
{
    m_Ctrl = &ctrl;
    m_PropertyArray = &propArray;
    m_Property = &prop;
}

CCmdRemovePropertyArrayItem::~CCmdRemovePropertyArrayItem()
{
}

Bool CCmdRemovePropertyArrayItem::Undo()
{
    m_PropertyArray->AddItemAtEnd( *m_Property );
    return TRUE;
}

Bool CCmdRemovePropertyArrayItem::Do()
{
    m_PropertyArray->RemoveItem( *m_Property );
    return TRUE;
}
