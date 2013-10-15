#include <Dune\Model\Scene\NodeSpatial.h>

using namespace Dune;

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CNodeSpatial, CBankObject )


CNodeSpatial::CNodeSpatial() :
    CBankObject( )
{
    m_Position = NEWOBJ( CPropertyVector3f, ( "Position" ) );
	m_Position->SetGroup( "NodeSpatial" );
    AddProperty( *m_Position );

    m_Rotation = NEWOBJ( CPropertyVector3f, ( "Rotation" ) );
	m_Rotation->SetGroup( "NodeSpatial" );
    AddProperty( *m_Rotation );

    m_Transformation = NEWOBJ( CPropertyMatrix4x4, ( "Transformation" ) );
	m_Transformation->SetGroup( "NodeSpatial" );
    AddProperty( *m_Transformation );
    m_Transformation->SetIsEnabled( FALSE );
}

CNodeSpatial::~CNodeSpatial()
{
}

Void CNodeSpatial::OnPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue )
{
    if( &p == m_Transformation )
    {
        m_Position->SetValue( m_Transformation->GetValue().GetTranslationComponent() );
        Sb::CEulerAngle angles = Sb::CEulerAngle( m_Transformation->GetValue().GetRotationComponent() );
        m_Rotation->SetValue( MathRadianToDegree( angles ) );
    }
    else if( &p == m_Position )
    {
        Sb::CMatrix4x4f m = m_Transformation->GetValue();
        m.SetTranslationComponent( m_Position->GetValue() );
        m_Transformation->SetValue( m );
    }
    else if( &p == m_Rotation )
    {
        Sb::CMatrix4x4f m = m_Transformation->GetValue(); 
        m.SetRotationComponent( Sb::CMatrix3x3f( MathDegreeToRadian( m_Rotation->GetValue() ) ) );
        m_Transformation->SetValue( m );
    }
    CBankObject::OnPropertyChanged( p, oldValue );
}

Void CNodeSpatial::BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
	Sb::CNodeSpatial* node = objSet.GetObj<Sb::CNodeSpatial>();
	DebugAssert( node );

	node->SetLocalMatrix( GetTransformation().GetValue() );

	// Reparent it
	Sb::CSceneWorld* parentWorld = NULL;
	CBakeObjectSet* parentObjSet = context.GetObjectSet( GetParent() );

	if( parentObjSet )
		parentWorld = parentObjSet->GetObj<Sb::CSceneWorld>();

	if( parentWorld )
		node->SetParent( &parentWorld->GetRootNode() );
	else
		node->SetParent( NULL );

	CBankObject::BakeObjectSet( context, objSet );
}

CPropertyMatrix4x4& CNodeSpatial::GetTransformation() const
{
	return *m_Transformation;
}

CPropertyVector3f& CNodeSpatial::GetPosition() const
{
	return *m_Position;
}

CPropertyVector3f& CNodeSpatial::GetRotation() const
{
	return *m_Rotation;
}