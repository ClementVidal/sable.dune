#include <Dune/Model/Asset/StatesDepthStencil.h>

#include <Dune/Model/Project/Project.h>

#include <Sable/Graphics/Common/DataTypes.h>
#include <Sable/Graphics/States/DepthStencil.h>

using namespace Dune;

String CAssetStatesDepthStencil::PropTestEnable = "TestEnable";
String CAssetStatesDepthStencil::PropWriteEnable = "WriteEnable";
String CAssetStatesDepthStencil::PropDepthFunc = "DepthFunc";

CPropertyEnum::SEntry CAssetStatesDepthStencil::EnumDepthFunc[] = { 
	{ "Never", nStateDepthFunc_Never }, 
	{ "Less", nStateDepthFunc_Less }, 
	{ "Equal", nStateDepthFunc_Equal }, 
	{ "LessEqual", nStateDepthFunc_LessEqual }, 
	{ "Greater", nStateDepthFunc_Greater }, 
	{ NULL, NULL } };

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CAssetStatesDepthStencil, CAsset )


CAssetStatesDepthStencil::CAssetStatesDepthStencil( ) :
    CAsset()
{
	m_TestEnable = NEWOBJ( CPropertyBool, ( PropTestEnable ) );
	AddProperty( *m_TestEnable );
	m_WriteEnable = NEWOBJ( CPropertyBool, ( PropWriteEnable ) );
	AddProperty( *m_WriteEnable );
	m_DepthFunc = NEWOBJ( CPropertyEnum, ( PropDepthFunc, CPropertyEnum::CDef( EnumDepthFunc ) ) );
	AddProperty( *m_DepthFunc );

    SetName("StatesDepthStencil");
}

CAssetStatesDepthStencil::~CAssetStatesDepthStencil()
{
}

Void CAssetStatesDepthStencil::PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet  ) const
{
	Sb::CStatesDepthStencil* shader = NEWOBJ( Sb::CStatesDepthStencil, ( ) );
	objSet.AddObject( *shader );
}

Void CAssetStatesDepthStencil::BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
	Sb::CStatesDepthStencil* state = objSet.GetObj<Sb::CStatesDepthStencil>( );

	if( state )
	{
		Sb::CStatesDepthStencil::SDesc dsDesc;
		dsDesc.DepthTestEnable =  m_TestEnable->GetValue();
		dsDesc.DepthWriteEnable = m_WriteEnable->GetValue();
		dsDesc.DepthFunc = (EStateDepthFunc) m_DepthFunc->GetValue();

		state->Acquire( dsDesc );

		CAsset::BakeObjectSet( context, objSet );
	}
}

