#include <Dune\Editor\View\Editor\UI\UI.h>

#include <Sable\Input\Keyboard\Interface.h>
#include <Sable\Graphics\Debug\RenderPass.h>
#include <Dune\Editor\Sable\EditorRenderer.h>
#include <Dune\Editor\Ctrl\Editor\UI\UI.h>

using namespace Dune;

CViewEditorUI::CViewEditorUI( CApplication& app, const CHandle& h ) :
    CViewEditor( app )
{
	m_LMBDown = FALSE;
    m_RootHandle = h;
	m_SelectableObjCount = 0;
	m_SelectedObj = -1;
	m_Ctrl = NULL;
    InitializeWidget();
}

CViewEditorUI::~CViewEditorUI()
{
	DebugAssert(m_Ctrl->GetRefCount() == 1 );
    m_DisplayWindow->GetSableEditorWindow().SetRenderer( NULL );
	m_Ctrl = NULL;
}

CCtrlEditor& CViewEditorUI::GetCtrl() const
{
	return *m_Ctrl;
}

CEditorDisplayWindow* CViewEditorUI::GetDisplayWindow() const 
{
    return m_DisplayWindow;
}

Void CViewEditorUI::InitializeWidget()
{
	QVBoxLayout* mainLayout = new QVBoxLayout( );
	mainLayout->setContentsMargins( 0,0,0,0 );
    Sb::CSwapChainInfo info( 800, 600, nTextureFormat_R8G8B8A8, nMultiSamplingQuality_None, 60, FALSE );
    m_DisplayWindow = new CEditorDisplayWindow( *this, info );

	m_MenuBar = new QMenuBar( this );
	QAction* action;
	QMenu* menuFile = new QMenu( "&View", this );
	action = menuFile->addAction( "&Show Grid" );
	action = menuFile->addAction( "&Grid Settings..." );
	m_MenuBar->addMenu( menuFile );
	QMenu* editFile = new QMenu( "&Edit", this );
	action = editFile->addAction( "&Enable Snapping" );
	m_MenuBar->addMenu( editFile );

	m_ToolBar = new QToolBar( this );

	mainLayout->addWidget( m_ToolBar );
	mainLayout->addWidget( m_DisplayWindow );

	setLayout( mainLayout );

	layout()->setMenuBar( m_MenuBar );

	connect( m_DisplayWindow, SIGNAL( OnMouseLMBUp( const Sb::CVector2f& ) ), this, SLOT( OnMouseLMBUp( const Sb::CVector2f& ) ) );
	connect( m_DisplayWindow, SIGNAL( OnMouseLMBDown( const Sb::CVector2f& ) ), this, SLOT( OnMouseLMBDown( const Sb::CVector2f& ) ) );
	connect( m_DisplayWindow, SIGNAL( OnMouseMove( const Sb::CVector2f& ) ), this, SLOT( OnMouseMove( const Sb::CVector2f& ) ) );
	connect( m_DisplayWindow, SIGNAL( OnMouseLDClick( const Sb::CVector2f& ) ), this, SLOT( OnMouseLDClick( const Sb::CVector2f& ) ) );
	connect( m_DisplayWindow, SIGNAL( OnMouseRDClick( const Sb::CVector2f& ) ), this, SLOT( OnMouseRDClick( const Sb::CVector2f& ) ) );
	connect( m_DisplayWindow, SIGNAL( OnRender( Sb::CRenderer& ) ), this, SLOT( OnRender( Sb::CRenderer& ) ) );

}

void CViewEditorUI::showEvent( QShowEvent * event )
{
    if( !m_Ctrl )
    {
        m_Ctrl = NEWOBJ( CCtrlEditorUI, ( *this, m_RootHandle ) );
    }
}

void CViewEditorUI::resizeEvent(  QResizeEvent * event  )
{

}

Void CViewEditorUI::OnMouseLMBUp( const Sb::CVector2f& pos )
{
    Sb::CVector2f pixelPos = pos * m_RendertargetSize;

	m_LMBDown = FALSE;
	if( m_Ctrl->IsDraging() )
	{
		m_Ctrl->StopDraging( pixelPos );
	}
    else
    {
        CCmd* cmd;

        if( !InputManager.GetKeyboard()->GetIsKeyDown( nKeyboardEventData_LeftControl ) )
        {
            //COMMAND: SelectObject
            Sb::CArray<CHandle> objList( GetApp().GetProjectCtrl().GetSelectedObjects() );
            cmd = NEWOBJ( CCmdSelectObject, ( GetApp().GetProjectCtrl(), objList.GetBuffer(), objList.GetItemCount(), CCmdSelectObject::nAction_DeSelect) );
            GetApp().GetCommandProcessor().Submit( *cmd );
        }

        if( m_SelectedObj != -1 )
        {
            m_SelectedObj++;
            if( m_SelectableObjCount > 1)
                m_SelectedObj = m_SelectedObj % (m_SelectableObjCount);
            else
                m_SelectedObj = 0;

            CCmdSelectObject::EAction action = CCmdSelectObject::nAction_Select;
            if( InputManager.GetKeyboard()->GetIsKeyDown( nKeyboardEventData_LeftControl ) )
            {
                action = CCmdSelectObject::nAction_Toggle;
            }

            //COMMAND: SelectObject
            cmd = NEWOBJ( CCmdSelectObject, ( GetApp().GetProjectCtrl(), m_SelectableObj[m_SelectedObj], action ) );
            GetApp().GetCommandProcessor().Submit( *cmd );
        }
    }
}

void CViewEditorUI::OnMouseLDClick( const Sb::CVector2f& pos )
{

}

void CViewEditorUI::OnMouseRDClick( const Sb::CVector2f& pos )
{

}

void CViewEditorUI::OnMouseLMBDown( const Sb::CVector2f& pos )
{
    m_LMBDown = TRUE;
    Sb::CVector2f pixelPos = pos * m_RendertargetSize;

    m_SelectableObjCount = m_Ctrl->PickElement( m_RootHandle, pixelPos, 32, m_SelectableObj );
    if( m_SelectableObjCount )
    {
        if( m_SelectedObj == -1 )
        {	
            m_SelectedObj = 0;
        }
    }
    else
    {
        m_SelectedObj = -1;
    }   
}

void CViewEditorUI::OnMouseMove( const Sb::CVector2f& pos )
{
	Sb::CVector2i pixelPos = Sb::CVector2i( pos * m_RendertargetSize);

	if( m_LMBDown && !m_Ctrl->IsDraging() )
	{
		m_Ctrl->StartDraging( pixelPos );
	}
	m_Ctrl->MoveElement( pixelPos );
	
}

void CViewEditorUI::OnRender( Sb::CRenderer& renderer )
{
	CCtrlProject::ObjectList::Iterator it;

	ForEachItem( it, GetApp().GetProjectCtrl().GetSelectedObjects() )
	{
		CHandle h = (*it);

		if( h.IsValid() && m_Ctrl->GetBakeObjectSet( h ) )
		{
			CBakeObjectSet* set = m_Ctrl->GetBakeObjectSet( h );
			Sb::CWidget* w = (Sb::CWidget*)set->GetObj<Sb::CWidget>();
			if( w )
			{
				DebugAssert( w );
				OutlineWidget( renderer, *w, Sb::CColor( 1.0f, 0.0f, 0.0f ), 1 );
			}
		}
	}
}

/**
Outline a widget in the current UI editor
*/
Void CViewEditorUI::OutlineWidget( Sb::CRenderer& renderer, const Sb::CWidget& w, const Sb::CColor& col, UInt32 width )
{
	if( !renderer.GetCurrentViewport() )
		return;

	m_RendertargetSize = renderer.GetCurrentViewport()->GetSize();

    Sb::CDebugRenderPass* debugRP = renderer.GetDebugRenderPass();

    Sb::CRect2Di moveZone = w.GetRect();
	Sb::CRect2Di scaleZone;
	scaleZone.SetSize( Sb::CVector2i( 16, 16 ) );
	scaleZone.SetPosition( moveZone.GetPosition() + moveZone.GetSize() / 2 );

    Sb::CMatrix4x4f m;

    m.SetFromIdentity();

    debugRP->PushWorldMatrix( m );
    debugRP->PushViewMatrix( m );

    m.SetFromOrthoProjectionValues( 0.0f, m_RendertargetSize.GetX(), 0.0f, m_RendertargetSize.GetY(), 0.0, 1.0 );
    debugRP->PushProjMatrix( m );

	debugRP->PushColor( Sb::CColor::Red );
	DrawRect( Sb::CRect2Df( moveZone ), *debugRP, *renderer.GetCurrentViewport() );
	debugRP->PopColor( );
	debugRP->PushColor( Sb::CColor::Blue );
	DrawRect( Sb::CRect2Df( scaleZone ), *debugRP, *renderer.GetCurrentViewport() );
	debugRP->PopColor(  );

    debugRP->PopViewMatrix();
    debugRP->PopWorldMatrix();
    debugRP->PopProjMatrix();
}

Void CViewEditorUI::DrawRect( const Sb::CRect2Df& r, Sb::CDebugRenderPass& debugRP, Sb::CViewport& vp )
{
	Sb::CVector3f t3;
	Sb::CVector2f t2;
    Sb::CVector2f divider( vp.GetWidth(), vp.GetHeight() );

	debugRP.BeginLineStrip();

	t2 = r.GetUpperLeftCorner() ;
	t3 = Sb::CVector3f( t2.GetX(), t2.GetY(), 0.0f );
	debugRP.DrawLine( t3 );

	t2 = r.GetUpperRightCorner() ;
	t3 = Sb::CVector3f( t2.GetX(), t2.GetY(), 0.0f );
	debugRP.DrawLine( t3 );

	t2 = r.GetLowerRightCorner() ;
	t3 = Sb::CVector3f( t2.GetX(), t2.GetY(), 0.0f );
	debugRP.DrawLine( t3 );

	t2 = r.GetLowerLeftCorner() ;
	t3 = Sb::CVector3f( t2.GetX(), t2.GetY(), 0.0f );
	debugRP.DrawLine( t3 );

	t2 = r.GetUpperLeftCorner() ;
	t3 = Sb::CVector3f( t2.GetX(), t2.GetY(), 0.0f );
	debugRP.DrawLine( t3 );

	debugRP.EndLineStrip();

}
