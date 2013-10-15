#include <Dune\Model\Game\Object.h>

#include <Dune\Model\Property\Property.h>
#include <Dune\Model\Property\PropertyArray.h>

using namespace Dune;

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CGameObject, CBankObject )

CGameObject::CGameObject() :
    CBankObject( )
{
    m_IsEdited = FALSE;
}

CGameObject::~CGameObject()
{

}

Bool CGameObject::GetIsEdited() const
{
    return m_IsEdited;
}

Void CGameObject::SetIsEdited( Bool onOff )
{
    m_IsEdited = onOff;
}