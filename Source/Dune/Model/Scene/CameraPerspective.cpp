#include <Dune/Model/Scene/CameraPerspective.h>

#include <Sable/Graphics/Camera/Perspective.h>

using namespace Dune;

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CCameraPerspective, CNodeSpatial )


CCameraPerspective::CCameraPerspective() :
    CNodeSpatial( )
{
    SetName("CameraPerspective");

	m_FieldOfView = NEWOBJ( CPropertyFloat32, ( "FOV" ) );
	m_FieldOfView->SetValue( MathPi / 2.0f );
	m_FieldOfView->SetGroup( "CameraPerspective" );
	AddProperty( *m_FieldOfView );

	m_Aspect = NEWOBJ( CPropertyFloat32, ( "Aspect" ) );
	m_Aspect->SetValue( 3.0f / 4.0f );
	m_Aspect->SetGroup( "CameraPerspective" );
	AddProperty( *m_Aspect );

	m_NearPlane = NEWOBJ( CPropertyFloat32, ( "NearPlane" ) );
	m_NearPlane->SetValue( 1.0f );
	m_NearPlane->SetGroup( "CameraPerspective" );
	AddProperty( *m_NearPlane );

	m_FarPlane = NEWOBJ( CPropertyFloat32, ( "FarPlane" ) );
	m_FarPlane->SetValue( 100.0f );
	m_FarPlane->SetGroup( "CameraPerspective" );
	AddProperty( *m_FarPlane );
}

CCameraPerspective::~CCameraPerspective()
{
}


CPropertyFloat32& CCameraPerspective::GetFieldOfView()
{
	return *m_FieldOfView;
}

CPropertyFloat32& CCameraPerspective::GetAspect()
{
	return *m_Aspect;
}

CPropertyFloat32& CCameraPerspective::GetNearPlane()
{
	return *m_NearPlane;
}

CPropertyFloat32& CCameraPerspective::GetFarPlane()
{
	return *m_FarPlane;
}

Void CCameraPerspective::PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet ) const
{
    Sb::CCameraPerspective* camera = NEWOBJ( Sb::CCameraPerspective, ( ) );
    objSet.AddObject( *camera );

    CBankObject::PrepareBakeObjectSet( context, objSet );
}

Void CCameraPerspective::BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
    Sb::CCameraPerspective* camera = objSet.GetObj<Sb::CCameraPerspective>();
    DebugAssert( camera );

	camera->SetFOV( m_FieldOfView->GetValue() );
	camera->SetAspect( m_Aspect->GetValue() );
	camera->SetNearPlane( m_NearPlane->GetValue() );
	camera->SetFarPlane( m_FarPlane->GetValue() );

    CNodeSpatial::BakeObjectSet( context, objSet );
}
