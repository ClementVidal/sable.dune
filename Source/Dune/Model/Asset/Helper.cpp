#include <Dune/Model/Asset/Helper.h>

using namespace Dune;

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CAssetHelper, CFBXDocInterface )

String CAssetHelper::PropPosition = "Position";

CAssetHelper::CAssetHelper( ) :
    CFBXDocInterface( )
{
    CPropertyInterface* p;

    p = new CPropertyVector3f( PropPosition );
	p->SetIsEnabled( FALSE );
    AddProperty( *p );

    SetName("Helper");
}

CAssetHelper::~CAssetHelper()
{
}

CPropertyVector3f& CAssetHelper::GetWorldPosition() const
{
	return (CPropertyVector3f&)*GetProperty( PropPosition );
}

Bool CAssetHelper::Serialize( Sb::CPersistentArchive& ar )
{
	return CFBXDocInterface::Serialize( ar );
}

Void CAssetHelper::OnSetFBXBinding()
{
	/*daeElement* node = GetColladaElement();
	if( node == NULL )
		return;

	CFBXGraphConverter converter( (domNode&) *node, TRUE );
	GetWorldPosition().SetValue( converter.GetWorldPosition() );
*/
	CFBXDocInterface::OnSetFBXBinding();
}

Void CAssetHelper::PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet  ) const
{
	Sb::CNodeSpatial* node = NEWOBJ( Sb::CNodeSpatial, () );
	objSet.AddObject( *node );
}

Void CAssetHelper::BakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet )
{
	Sb::CNodeSpatial* node = objSet.GetObj<Sb::CNodeSpatial>( );
	if( node )
	{
		// Make sure to reload collada file if necessary and to extract the right position
		ReloadFBXIfNecessary();

		Sb::CMatrix4x4f m;
		m.SetFromIdentity();
		m.SetTranslationComponent( GetWorldPosition().GetValue() );

		node->SetLocalMatrix( m );

		CAsset::BakeObjectSet( context, objSet );
	}

}