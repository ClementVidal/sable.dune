#include <Dune/Model/Asset/StatesRasterizer.h>

#include <Dune/Model/Project/Project.h>

#include <Sable/Graphics/Common/DataTypes.h>

using namespace Dune;

String CAssetStatesRasterizer::PropCullMode = "CullMode";
String CAssetStatesRasterizer::PropFillMode = "FillMode";

CPropertyEnum::SEntry CAssetStatesRasterizer::EnumFillMode[] = { 
	{ "Point", nStateFillMode_Point }, 
	{ "Wireframe", nStateFillMode_WireFrame }, 
	{ "Solid", nStateFillMode_Solid },
	{ NULL, NULL } };


CPropertyEnum::SEntry CAssetStatesRasterizer::EnumCullMode[] = { 
	{ "None", nStateCullMode_Disabled }, 
	{ "CW", nStateCullMode_CW }, 
	{ "CCW", nStateCullMode_CCW },
	{ NULL, NULL } };

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CAssetStatesRasterizer, CAsset )

CAssetStatesRasterizer::CAssetStatesRasterizer( ) :
    CAsset()
{
	m_CullMode = NEWOBJ( CPropertyEnum, ( PropCullMode, CPropertyEnum::CDef( EnumCullMode ) ) );
	AddProperty( *m_CullMode );
	m_FillMode = NEWOBJ( CPropertyEnum, ( PropFillMode, CPropertyEnum::CDef( EnumFillMode ) ) );
	AddProperty( *m_FillMode );

    SetName("StatesRasterizer");
}

CAssetStatesRasterizer::~CAssetStatesRasterizer()
{
}


Void CAssetStatesRasterizer::PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet  ) const
{
	Sb::CStatesRasterizer* shader = NEWOBJ( Sb::CStatesRasterizer, ( ) );
	objSet.AddObject( *shader );
}

Void CAssetStatesRasterizer::BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
	Sb::CStatesRasterizer* state = objSet.GetObj<Sb::CStatesRasterizer>( );

	if( state )
	{
		Sb::CStatesRasterizer::SDesc srDesc;
		srDesc.CullMode = (EStateCullMode) m_CullMode->GetValue();
		srDesc.FillMode = (EStateFillMode) m_FillMode->GetValue();
		state->Acquire( srDesc );
		CAsset::BakeObjectSet( context, objSet );
	}
}

Void CAssetStatesRasterizer::InitializeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{

}