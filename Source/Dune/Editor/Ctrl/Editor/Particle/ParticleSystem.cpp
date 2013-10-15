#include <Dune\Editor\Ctrl\Editor\Particle\ParticleSystem.h>

#include <Dune\Editor\Ctrl\Project.h>
#include <Dune\Editor\Sable\EditorRenderer.h>
#include <Dune\Editor\View\Editor\DisplayWindow.h>

#include <Sable\Graphics\Debug\Header.h>
#include <Sable\Graphics\Particle\Operator\Header.h>
#include <Sable\Graphics\Particle\Group.h>

using namespace Dune;

CCtrlEditorParticleSystem::CCtrlEditorParticleSystem( CViewParticleSystem& view, const CHandle& ps ) :
    CCtrlEditor( (CViewEditor&)view )
{
	Initialize();
	m_ParticleSystem = NULL;

	m_Handle = ps;

	GetBakeContext().BakeObject( GetRootObject(), TRUE, FALSE );

	CBakeObjectSet* set = GetBakeObjectSet( m_Handle );
	if( set )
	{
		m_ParticleSystem = set->GetObj<Sb::CParticleSystem>();
		if( m_ParticleSystem )
		{
            DebugAssert( m_SableView.GetWorld() == NULL );
            m_World = NEWOBJ( Sb::CSceneWorld, () );
			Sb::CParticleGroup* sim = &m_ParticleSystem->GetGroup();
            m_SableView.SetWorld( m_World );
			m_ParticleSystem->SetParent( &m_World->GetRootNode() );
			sim->Start();
//			sim->Pause();
		}
	}
//	m_SableRenderer.GetGuiRenderPass()->GetRoot().RemoveAllChild();
}

CCtrlEditorParticleSystem::~CCtrlEditorParticleSystem()
{
	if( m_Handle.IsValid() )
	{
		CAssetParticleSystem& set = *GetProject()->GetObj<CAssetParticleSystem >( m_Handle );
		if( set.GetIsEdited() )
			set.SetIsEdited( FALSE );
	}
}

Sb::CCameraPerspective& CCtrlEditorParticleSystem::GetSableCamera()
{
    return m_SableCamera;
}

const CHandle& CCtrlEditorParticleSystem::GetRootObject() const
{
    return m_Handle;
}

Sb::CParticleSystem* CCtrlEditorParticleSystem::GetParticleSystem() const
{
	return m_ParticleSystem;
}

Void CCtrlEditorParticleSystem::Initialize()
{
	Sb::CApplicationWindow* window = &GetView().GetDisplayWindow()->GetSableEditorWindow();

	window->SetRenderer( &m_SableRenderer );
	m_SableRenderer.SetView( &m_SableView );

	m_SableView.SetCamera( &m_SableCamera );

	m_SableRenderer.Initialize( window->GetSwapChain().GetInfo().RenderTargetInfo );
	m_SableView.Initialize( );
}

Void CCtrlEditorParticleSystem::OnProjectDestroyed()
{
//    if( GetApp().GetEditorContainerView().GetCurrentEditor() )
  //      GetApp().GetEditorContainerView().GetCurrentEditor()->Hide();

    CCtrl::OnProjectDestroyed();
}

Void CCtrlEditorParticleSystem::RemoveOperator( Sb::CParticleOperator& op )
{
	m_ParticleSystem->GetGroup().RemoveOperator( op );
}

Sb::CParticleOperator* CCtrlEditorParticleSystem::AddOperator( const Sb::CTypeInfo& type )
{
	DebugAssert( type.IsKindOf( Sb::CParticleOperator::GetStaticTypeInfo() ) );

	Sb::CParticleOperator* op = (Sb::CParticleOperator*)type.CreateObject( NULL );
	m_ParticleSystem->GetGroup().AddOperator( *op );

	return op;
}
//REFACTORDX10
/*
Void CCtrlEditorParticleSystem::OnRender( CSableEditorRenderer& renderer )
{
	Sb::CDebugRenderPass& drp = *renderer.GetDebugRenderPass();
	
	drp.PushViewProjMatrix();

	drp.BeginLineList();

	drp.PushColor( Sb::CColor::Red );
	drp.DrawLine( Sb::CVector3f( 0.0f, 0.0f, 0.0f ), Sb::CVector3f( 1.0f, 0.0f, 0.0f ) );
	drp.PopColor();

	drp.PushColor( Sb::CColor::Green );
	drp.DrawLine( Sb::CVector3f( 0.0f, 0.0f, 0.0f ), Sb::CVector3f( 0.0f, 1.0f, 0.0f ) );
	drp.PopColor();

	drp.PushColor( Sb::CColor::Blue );
	drp.DrawLine( Sb::CVector3f( 0.0f, 0.0f, 0.0f ), Sb::CVector3f( 0.0f, 0.0f, 1.0f ) );
	drp.PopColor();

	drp.EndLineList();


	drp.PopViewProjMatrix();
}
*/

Bool CCtrlEditorParticleSystem::SerializeParticleFile( const Sb::CFilePath& path, EAccesMode mode ) const
{
	CAssetParticleSystem* ps = GetProject()->GetObj<CAssetParticleSystem>( m_Handle );
	
	if( ps && path.IsValid() )
	{
		ps->GetFilePath().SetValue( path );
		return ps->SerializeParticleFile( m_ParticleSystem->GetGroup(), mode );
	}

	return FALSE;
}
