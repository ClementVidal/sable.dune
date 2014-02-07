#include <Dune/Editor/View/Editor/Scene/Scene.h>

#include <Sable/Input/Keyboard/Interface.h>
#include <Sable/Graphics/Debug/RenderPass.h>
#include <Dune/Editor/Sable/EditorRenderer.h>
#include <Dune/Editor/Ctrl/Editor/Scene/Scene.h>

#include <Dune/Resources/Icons/SceneEditor.xpm>

using namespace Dune;

CViewEditorScene::CViewEditorScene( CApplication& app, const CHandle& h ) :
	CViewEditor( app ),
	m_IconTable( (const char**)SceneEditor_xpm )
{
    m_LMBDown = FALSE;
    m_SelectableObjCount = 0;
    m_SelectedObj = -1;
    m_RootHandle = h;
    InitializeWidget();
}

CViewEditorScene::~CViewEditorScene()
{
	DebugAssert(m_Ctrl->GetRefCount() == 1 );
	m_Ctrl = NULL;
}

void CViewEditorScene::resizeEvent( QResizeEvent * event )
{
}

void CViewEditorScene::showEvent( QShowEvent * event )
{
	if( !m_Ctrl )
	{
		m_Ctrl = NEWOBJ( CCtrlEditorScene, ( *this, m_RootHandle ) );

		m_Ctrl->GetCameraController().Initialize( m_Ctrl->GetSableCamera(), m_SignalMouseEventCamCtrl, m_SignalKeyboardEventCamCtrl );
		m_Ctrl->GetManipulator().Initialize( m_SignalMouseEventManipulator, *m_Ctrl );
	}
}

CEditorDisplayWindow* CViewEditorScene::GetDisplayWindow() const 
{
    return m_DisplayWindow;
}

CCtrlEditor&	CViewEditorScene::GetCtrl() const
{
	return *m_Ctrl;
}

Void CViewEditorScene::InitializeWidget()
{
	QVBoxLayout* mainLayout = new QVBoxLayout( );
	mainLayout->setContentsMargins( 0,0,0,0 );

	//QScrollArea* scrollArea = new QScrollArea();

    Sb::CSwapChainInfo info( 1024, 768, nTextureFormat_R8G8B8A8, nMultiSamplingQuality_None, 60, FALSE );
    m_DisplayWindow = new CEditorDisplayWindow( *this, info );

    connect( m_DisplayWindow, SIGNAL( OnRender( Sb::CRenderer& ) ), this, SLOT( OnRender( Sb::CRenderer& ) ) );
    connect( m_DisplayWindow, SIGNAL( OnMouseLMBUp( const Sb::CVector2f& ) ), this, SLOT( OnMouseLMBUp( const Sb::CVector2f& ) ) );
    connect( m_DisplayWindow, SIGNAL( OnMouseLMBDown( const Sb::CVector2f& ) ), this, SLOT( OnMouseLMBDown( const Sb::CVector2f& ) ) );
    connect( m_DisplayWindow, SIGNAL( OnMouseRMBUp( const Sb::CVector2f& ) ), this, SLOT( OnMouseRMBUp( const Sb::CVector2f& ) ) );
    connect( m_DisplayWindow, SIGNAL( OnMouseRMBDown( const Sb::CVector2f& ) ), this, SLOT( OnMouseRMBDown( const Sb::CVector2f& ) ) );
    connect( m_DisplayWindow, SIGNAL( OnMouseMMBUp( const Sb::CVector2f& ) ), this, SLOT( OnMouseMMBUp( const Sb::CVector2f& ) ) );
    connect( m_DisplayWindow, SIGNAL( OnMouseMMBDown( const Sb::CVector2f& ) ), this, SLOT( OnMouseMMBDown( const Sb::CVector2f& ) ) );
    connect( m_DisplayWindow, SIGNAL( OnMouseMove( const Sb::CVector2f& ) ), this, SLOT( OnMouseMove( const Sb::CVector2f& ) ) );
    connect( m_DisplayWindow, SIGNAL( OnMouseLDClick( const Sb::CVector2f& ) ), this, SLOT( OnMouseLDClick( const Sb::CVector2f& ) ) );
    connect( m_DisplayWindow, SIGNAL( OnMouseRDClick( const Sb::CVector2f& ) ), this, SLOT( OnMouseRDClick( const Sb::CVector2f& ) ) );
    connect( m_DisplayWindow, SIGNAL( OnKeyboardKeyDown( EKeyboardEventData ) ), this, SLOT( OnKeyboardKeyDown( EKeyboardEventData ) ) );
    connect( m_DisplayWindow, SIGNAL( OnKeyboardKeyUp( EKeyboardEventData ) ), this, SLOT( OnKeyboardKeyUp( EKeyboardEventData ) ) );

	//scrollArea->setWidget( m_DisplayWindow );
	//scrollArea->setAlignment( Qt::AlignCenter );

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
	m_ToolBar->setIconSize( QSize( 16, 16 ) );

    QActionGroup* actionGroup = new QActionGroup( m_ToolBar );
    
    action = m_ToolBar->addAction( m_IconTable.GetIcon( 0 ), "Translate" );
    action->setCheckable( TRUE );
    action->setChecked( TRUE );
    actionGroup->addAction( action );
    connect( action, SIGNAL( triggered() ), this, SLOT( OnUIEnableTranslationMode() ) );

    action = m_ToolBar->addAction( m_IconTable.GetIcon( 1 ),"Rotate" );
    action->setCheckable( TRUE );
    actionGroup->addAction( action );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnUIEnableRotatationMode() ) );

	m_ToolBar->addSeparator();

	action = m_ToolBar->addAction( m_IconTable.GetIcon( 2 ),"Fit selected object" );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnUIFitSelectedObject() ) );
	action = m_ToolBar->addAction( m_IconTable.GetIcon( 3 ),"Fit scene" );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnUIFitScene() ) );

    actionGroup->setExclusive( TRUE );

	mainLayout->addWidget( m_ToolBar );
	mainLayout->addWidget( m_DisplayWindow );

	setLayout( mainLayout );

	layout()->setMenuBar( m_MenuBar );

}

Void CViewEditorScene::OnKeyboardKeyDown( EKeyboardEventData data )
{
    m_SignalKeyboardEventCamCtrl( nKeyboardEventType_KeyDown, data );
}

Void CViewEditorScene::OnKeyboardKeyUp( EKeyboardEventData data )
{
    m_SignalKeyboardEventCamCtrl( nKeyboardEventType_KeyUp, data );
}

void CViewEditorScene::OnUIEnableTranslationMode()
{
    m_Ctrl->GetManipulator().SetMode( CManipulator::nMode_Translation);
}

void CViewEditorScene::OnUIEnableRotatationMode()
{
    m_Ctrl->GetManipulator().SetMode( CManipulator::nMode_Rotation);
}

void CViewEditorScene::OnUIFitSelectedObject()
{
	if( !GetProject() )
		return;

	const Sb::CList<CHandle>& objList = GetApp().GetProjectCtrl().GetSelectedObjects();

	if( objList.GetItemCount() )
	{
		CBankObject* obj = GetProject()->GetObj( objList.GetFirstItem() );
		if( obj->GetTypeInfo().IsKindOf( CNodeSpatial::GetStaticTypeInfo() ) )
		{
			m_Ctrl->FitObject( (CNodeSpatial&) *obj );
		}
	}
}

void CViewEditorScene::OnUIFitScene()
{
	m_Ctrl->FitScene();
}

Void CViewEditorScene::OnMouseLMBUp( const Sb::CVector2f& pos )
{
    if( m_Ctrl->GetManipulator().GetCurrentHandle() )
    {
		m_SignalMouseEventManipulator( nMouseEventType_KeyUp, nMouseEventData_LeftBtn, pos );    
    }
	else
	{
		if( m_Ctrl->GetCameraController().GetIsDragging() == FALSE )
			m_Ctrl->Pick( pos, m_TestBBox, m_TestRay );
		
		m_SignalMouseEventCamCtrl( nMouseEventType_KeyUp, nMouseEventData_LeftBtn, pos );
    }
}

Void CViewEditorScene::OnMouseRMBUp( const Sb::CVector2f& pos )
{
    if( m_Ctrl->GetManipulator().GetCurrentHandle() )
    {
        m_SignalMouseEventManipulator( nMouseEventType_KeyUp, nMouseEventData_RightBtn, pos );    
    }
    else
    {
        m_SignalMouseEventCamCtrl( nMouseEventType_KeyUp, nMouseEventData_RightBtn, pos );
    }
}

Void CViewEditorScene::OnMouseMMBUp( const Sb::CVector2f& pos )
{
    if( m_Ctrl->GetManipulator().GetCurrentHandle() )
    {
        m_SignalMouseEventManipulator( nMouseEventType_KeyUp, nMouseEventData_MiddleBtn, pos );    
    }
    else
    {
        m_SignalMouseEventCamCtrl( nMouseEventType_KeyUp, nMouseEventData_MiddleBtn, pos );
    }
}

Void CViewEditorScene::OnMouseLDClick( const Sb::CVector2f& pos )
{

}

Void CViewEditorScene::OnMouseRDClick( const Sb::CVector2f& pos )
{

}

Void CViewEditorScene::OnMouseMMBDown( const Sb::CVector2f& pos )
{
    m_LMBDown = TRUE;
    Sb::CVector2f pixelPos = pos * m_RendertargetSize;

    if( m_Ctrl->GetManipulator().GetCurrentHandle() )
    {
        m_SignalMouseEventManipulator( nMouseEventType_KeyDown, nMouseEventData_MiddleBtn, pos );
    }
    else
    {
        m_SignalMouseEventCamCtrl( nMouseEventType_KeyDown, nMouseEventData_MiddleBtn, pos );
    }
}

Void CViewEditorScene::OnMouseRMBDown( const Sb::CVector2f& pos )
{
    m_LMBDown = TRUE;
    Sb::CVector2f pixelPos = pos * m_RendertargetSize;

    if( m_Ctrl->GetManipulator().GetCurrentHandle() )
    {
        m_SignalMouseEventManipulator( nMouseEventType_KeyDown, nMouseEventData_RightBtn, pos );
    }
    else
    {
        m_SignalMouseEventCamCtrl( nMouseEventType_KeyDown, nMouseEventData_RightBtn, pos );
    }
}

Void CViewEditorScene::OnMouseLMBDown( const Sb::CVector2f& pos )
{
    m_LMBDown = TRUE;
    Sb::CVector2f pixelPos = pos * m_RendertargetSize;

    if( m_Ctrl->GetManipulator().GetCurrentHandle() )
    {
		m_SignalMouseEventManipulator( nMouseEventType_KeyDown, nMouseEventData_LeftBtn, pos );
    }
	else
	{
		m_SignalMouseEventCamCtrl( nMouseEventType_KeyDown, nMouseEventData_LeftBtn, pos );
	}
}

Void CViewEditorScene::OnMouseMove( const Sb::CVector2f& pos )
{
	m_SignalMouseEventManipulator( nMouseEventType_Move, nMouseEventData_None, pos );

    if( m_Ctrl->GetManipulator().GetCurrentHandle() == NULL )
	{
		m_SignalMouseEventCamCtrl( nMouseEventType_Move, nMouseEventData_None, pos );
	}
}


Void CViewEditorScene::OnRender( Sb::CRenderer& renderer )
{
    Sb::CMatrix4x4f worldMtx, projMtx;
    Sb::CDebugRenderPass* debug = renderer.GetDebugRenderPass();

    worldMtx.SetFromIdentity();

    // Grid
    debug->PushViewProjMatrix();
    debug->PushWorldMatrix( worldMtx );
    debug->DrawAdaptativeGrid( Sb::CVector3f::Zero );
    debug->PopWorldMatrix();
    debug->PopViewProjMatrix();

    // Left side frame
    Sb::CCameraPerspective* cam = (Sb::CCameraPerspective*)renderer.GetView()->GetCamera();
    Float32 planeDist = 10.0f;
    Sb::CVector2f frustumSize = cam->GetFrustumSizeAtDist( planeDist );
    Sb::CVector3f framePos = cam->Get3DPointFrom2DPos( Sb::CVector2f( 0.05f,0.05f*cam->GetAspect() ), planeDist );
    worldMtx.SetFromTranslation( framePos );
    projMtx.SetFromOrthoProjectionValues(frustumSize.GetX(), frustumSize.GetY(),1,100);
    debug->PushViewMatrix();
    debug->PushProjMatrix( projMtx );
    debug->PushWorldMatrix( worldMtx );
    debug->DrawSpatialFrame( 1.0f );
    debug->PopWorldMatrix();
    debug->PopViewMatrix();
    debug->PopProjMatrix();

    // Render Manipulator
    m_Ctrl->Render( renderer );

}

