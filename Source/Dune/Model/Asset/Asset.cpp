#include <Dune\Model\Asset\Asset.h>

#include <Dune\Model\Property\Property.h>
#include <Dune\Model\Property\PropertyArray.h>

using namespace Dune;

IMPLEMENT_MANAGED_CLASS1( Dune, CAsset, CBankObject )

CAsset::CAsset() :
    CBankObject( )
{
    m_IsEdited = FALSE;
}

CAsset::~CAsset()
{

}

Bool CAsset::GetIsEdited() const
{
    return m_IsEdited;
}

Void CAsset::SetIsEdited( Bool onOff )
{
    m_IsEdited = onOff;
}