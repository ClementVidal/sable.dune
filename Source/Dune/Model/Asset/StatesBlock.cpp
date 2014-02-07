#include <Dune/Model/Asset/StatesBlock.h>

#include <Dune/Model/Project/Project.h>
#include <Dune/Model/Asset/StatesBlend.h>
#include <Dune/Model/Asset/StatesRasterizer.h>
#include <Dune/Model/Asset/StatesDepthStencil.h>
#include <Dune/Model/Asset/StatesSampler.h>

#include <Sable/Graphics/Common/DataTypes.h>

using namespace Dune;

String CAssetStatesBlock::PropSamplerStates = "SamplerStates";
String CAssetStatesBlock::PropRasterizerStates = "RasterizerStates";
String CAssetStatesBlock::PropBlendStates = "BlendStates";
String CAssetStatesBlock::PropDepthStencilStates = "DepthStencilStates";

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CAssetStatesBlock, CAsset )


CAssetStatesBlock::CAssetStatesBlock() :
    CAsset()
{
	m_Sampler = NEWOBJ( CPropertyHandle, ( PropSamplerStates ) );
	AddProperty( *m_Sampler );
	m_Sampler->AddFilter( CAssetStatesSampler::GetStaticTypeInfo() );

	m_Rasterizer = NEWOBJ( CPropertyHandle, ( PropRasterizerStates ) );
	AddProperty( *m_Rasterizer );
	m_Rasterizer->AddFilter( CAssetStatesRasterizer::GetStaticTypeInfo() );

	m_Blend = NEWOBJ( CPropertyHandle, ( PropBlendStates ) );
	AddProperty( *m_Blend );
	m_Blend->AddFilter( CAssetStatesBlend::GetStaticTypeInfo() );

	m_DepthStencil = NEWOBJ( CPropertyHandle, ( PropDepthStencilStates ) );
	AddProperty( *m_DepthStencil );
	m_DepthStencil->AddFilter( CAssetStatesDepthStencil::GetStaticTypeInfo() );

    SetName("StatesBlock");
}

CAssetStatesBlock::~CAssetStatesBlock()
{
}

Void CAssetStatesBlock::PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& out  ) const
{

}

Void CAssetStatesBlock::BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{

}

Void CAssetStatesBlock::InitializeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{

}