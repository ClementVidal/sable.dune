#include <Dune/Model/Asset/StatesBlend.h>

#include <Dune/Model/Project/Project.h>

#include <Sable/Graphics/Common/DataTypes.h>

using namespace Dune;

String CAssetStatesBlend::PropBlendEnable = "BlendEnable";
String CAssetStatesBlend::PropSrcColorBlendMode = "SrcColorBlendMode";
String CAssetStatesBlend::PropDstColorBlendMode = "DstColorBlendMode";
String CAssetStatesBlend::PropSrcAlphaBlendMode = "SrcAlphaBlendMode";
String CAssetStatesBlend::PropDstAlphaBlendMode = "DstAlphaBlendMode";
String CAssetStatesBlend::PropColorBlendOp = "ColorBlendOp";
String CAssetStatesBlend::PropAlphaBlendOp = "AlphaBlendOp";

CPropertyEnum::SEntry CAssetStatesBlend::EnumBlendMode[] = { 
	{ "Zero", nStateBlend_Zero }, 
	{ "One", nStateBlend_One }, 
	{ "SrcColor", nStateBlend_SrcColor }, 
	{ "InvSrcColor", nStateBlend_InvSrcColor }, 
	{ "SrcAlpha", nStateBlend_SrcAlpha }, 
	{ "InvSrcAlpha", nStateBlend_InvSrcAlpha }, 
	{ "DstAlpha", nStateBlend_DestAlpha }, 
	{ "InvDstAlpha", nStateBlend_InvDestAlpha }, 
	{ "DstColor", nStateBlend_DestColor }, 
	{ "InvDstColor", nStateBlend_InvDestColor }, 
	{ NULL, NULL } };

CPropertyEnum::SEntry CAssetStatesBlend::EnumBlendOp[] = { 
	{ "Add", nStateBlendOp_Add }, 
	{ "Substract", nStateBlendOp_Substract }, 
	{ "RevSubstract", nStateBlendOp_RevSubstract }, 
	{ "Min", nStateBlendOp_Min }, 
	{ "Max", nStateBlendOp_Max }, 
	{ NULL, NULL } };

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CAssetStatesBlend, CAsset )


CAssetStatesBlend::CAssetStatesBlend() :
    CAsset()
{
	m_BlendEnable = NEWOBJ( CPropertyBool, ( PropBlendEnable ) );
    AddProperty( *m_BlendEnable );
	m_SrcColorBlendMode = NEWOBJ( CPropertyEnum, ( PropSrcColorBlendMode, CPropertyEnum::CDef( EnumBlendMode ) ) );
    AddProperty( *m_SrcColorBlendMode );
	m_DstColorBlendMode = NEWOBJ( CPropertyEnum, ( PropDstColorBlendMode, CPropertyEnum::CDef( EnumBlendMode ) ) );
    AddProperty( *m_DstColorBlendMode );
	m_SrcAlphaBlendMode = NEWOBJ( CPropertyEnum, ( PropSrcAlphaBlendMode, CPropertyEnum::CDef( EnumBlendMode ) ) );
	AddProperty( *m_SrcAlphaBlendMode );
	m_DstAlphaBlendMode = NEWOBJ( CPropertyEnum, ( PropDstAlphaBlendMode, CPropertyEnum::CDef( EnumBlendMode ) ) );
	AddProperty( *m_DstAlphaBlendMode );
	m_ColorBlendOp = NEWOBJ( CPropertyEnum, ( PropColorBlendOp, CPropertyEnum::CDef( EnumBlendOp ) ) );
	AddProperty( *m_ColorBlendOp );
	m_AlphaBlendOp = NEWOBJ( CPropertyEnum, ( PropAlphaBlendOp, CPropertyEnum::CDef( EnumBlendOp ) ) );
	AddProperty( *m_AlphaBlendOp );

    SetName("StatesBlend");
}

CAssetStatesBlend::~CAssetStatesBlend()
{
}

Void CAssetStatesBlend::PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet  ) const
{
	Sb::CStatesBlend* state = NEWOBJ( Sb::CStatesBlend, ( ) );
	objSet.AddObject( *state );
}

Void CAssetStatesBlend::BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
	Sb::CStatesBlend* state = objSet.GetObj<Sb::CStatesBlend>( );

	if( state )
	{
		Sb::CStatesBlend::SDesc desc;

		desc.BlendEnable[0] = m_BlendEnable->GetValue();
		desc.SrcColorBlend = (EStateBlend) m_SrcColorBlendMode->GetValue();
		desc.DstColorBlend = (EStateBlend) m_DstColorBlendMode->GetValue();
		desc.SrcAlphaBlend = (EStateBlend) m_SrcAlphaBlendMode->GetValue();
		desc.DstAlphaBlend = (EStateBlend) m_DstAlphaBlendMode->GetValue();
		desc.AlphaBlendOp = (EStateBlendOp) m_AlphaBlendOp->GetValue();
		desc.ColorBlendOp = (EStateBlendOp) m_ColorBlendOp->GetValue();

		state->Acquire( desc );

		CAsset::BakeObjectSet( context, objSet );
	}
}

Void CAssetStatesBlend::InitializeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{

}
