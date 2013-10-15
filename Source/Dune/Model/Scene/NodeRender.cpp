#include <Dune\Model\Scene\NodeRender.h>

using namespace Dune;

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CNodeRender, CNodeSpatial )

String CNodeRender::PropModel = "Model";

CNodeRender::CNodeRender() :
    CNodeSpatial( )
{
    SetName("RenderNode");

    m_Model = NEWOBJ( CPropertyHandle, ( PropModel ) );
	m_Model->SetGroup( "NodeRender" );
    AddProperty( *m_Model );
    m_Model->AddFilter( CAssetModel::GetStaticTypeInfo() );

}

CNodeRender::~CNodeRender()
{
}

CPropertyHandle& CNodeRender::GetModel() const
{
    return *m_Model;
}

Void CNodeRender::PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet ) const
{
    Sb::CNodeRender* node = NEWOBJ( Sb::CNodeRender, ( ) );
    objSet.AddObject( *node );

    CBankObject::PrepareBakeObjectSet( context, objSet );
}

Void CNodeRender::BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
    Sb::CNodeRender* node = objSet.GetObj<Sb::CNodeRender>();
    DebugAssert( node );

    CBakeObjectSet* set = context.GetObjectSet( GetModel().GetValue() );
	Sb::CGeometryModel* geometryModel = NULL;
	if( set )
	{
		geometryModel = set->GetObj<Sb::CGeometryModel>();
	}
    node->SetModel( geometryModel );

    CNodeSpatial::BakeObjectSet( context, objSet );
}
