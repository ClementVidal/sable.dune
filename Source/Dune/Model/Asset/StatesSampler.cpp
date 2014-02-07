#include <Dune/Model/Asset/StatesSampler.h>

#include <Dune/Model/Project/Project.h>

#include <Sable/Graphics/Common/DataTypes.h>

using namespace Dune;

String CAssetStatesSampler::PropMipFilter = "MipFilter";
String CAssetStatesSampler::PropMinFilter = "MinFilter";
String CAssetStatesSampler::PropMagFilter = "MagFilter";
String CAssetStatesSampler::PropAddressU = "AddressU";
String CAssetStatesSampler::PropAddressV = "AddressV";
String CAssetStatesSampler::PropAddressW = "AddressW";

CPropertyEnum::SEntry CAssetStatesSampler::EnumFilterMode[] = { 
	{ "Point", nStateFilter_Point }, 
	{ "Linear", nStateFilter_Linear }, 
	{ NULL, NULL } };

CPropertyEnum::SEntry CAssetStatesSampler::EnumAddressMode[] = { 
	{ "Wrap", nStateAddress_Wrap }, 
	{ "Mirror", nStateAddress_Mirror }, 
	{ "Clamp", nStateAddress_Clamp }, 
	{ NULL, NULL } };

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CAssetStatesSampler, CAsset  )


CAssetStatesSampler::CAssetStatesSampler() :
    CAsset()
{
	m_MinFilter = NEWOBJ( CPropertyEnum, ( PropMinFilter, CPropertyEnum::CDef( EnumFilterMode ) ) );
	AddProperty( *m_MinFilter );
	m_MagFilter = NEWOBJ( CPropertyEnum, ( PropMagFilter, CPropertyEnum::CDef( EnumFilterMode ) ) );
	AddProperty( *m_MagFilter );
	m_MipFilter = NEWOBJ( CPropertyEnum, ( PropMipFilter, CPropertyEnum::CDef( EnumFilterMode ) ) );
	AddProperty( *m_MipFilter );

	m_AddressU = NEWOBJ( CPropertyEnum, ( PropAddressU, CPropertyEnum::CDef( EnumAddressMode ) ) );
	AddProperty( *m_AddressU );
	m_AddressV = NEWOBJ( CPropertyEnum, ( PropAddressV, CPropertyEnum::CDef( EnumAddressMode ) ) );
	AddProperty( *m_AddressV );
	m_AddressW = NEWOBJ( CPropertyEnum, ( PropAddressW, CPropertyEnum::CDef( EnumAddressMode ) ) );
	AddProperty( *m_AddressW );

    SetName("StatesSampler");
}

CAssetStatesSampler::~CAssetStatesSampler()
{
}

Void CAssetStatesSampler::PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet  ) const
{
	Sb::CStatesSampler* shader = NEWOBJ( Sb::CStatesSampler, ( ) );
	objSet.AddObject( *shader );
}

Void CAssetStatesSampler::BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
	Sb::CStatesSampler* state = objSet.GetObj<Sb::CStatesSampler>( );

	if( state )
	{
		Sb::CStatesSampler::SDesc sDesc;

		sDesc.AddressU = (EStateAddress) m_AddressU->GetValue();
		sDesc.AddressV = (EStateAddress) m_AddressV->GetValue();
		sDesc.AddressW = (EStateAddress) m_AddressW->GetValue();
		sDesc.MinFilter = (EStateFilter) m_MinFilter->GetValue();
		sDesc.MipFilter = (EStateFilter) m_MipFilter->GetValue();
		sDesc.MagFilter = (EStateFilter) m_MagFilter->GetValue();

		state->Acquire( sDesc );

		CAsset::BakeObjectSet( context, objSet );
	}
}

Void CAssetStatesSampler::InitializeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{

}