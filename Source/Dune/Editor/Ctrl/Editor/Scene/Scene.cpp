#include <Dune/Editor/Ctrl/Editor/Scene/Scene.h>

#include <Dune/Editor/Ctrl/Project.h>
#include <Dune/Model/Scene/NodeRender.h>
#include <Dune/Model/Scene/CameraPerspective.h>
#include <Dune/Editor/View/Editor/DisplayWindow.h>
#include <Sable/Input/Keyboard/Interface.h>
#include <Sable/Gui/Widget/Widget.h>
#include <Sable/Graphics/Camera/Camera.h>
#include <Dune/Editor/View/Editor/Scene/GizmoCamera.h>

using namespace Dune;

CCtrlEditorScene::CCtrlEditorScene( CViewEditorScene& view, const CHandle& h  ) :
    CCtrlEditor( view ),
	m_Manipulator( view.GetApp() )
{
	OnEditElement( h );
    Initialize();
}

CCtrlEditorScene::~CCtrlEditorScene()
{
	m_SableCamera.SetParent( NULL );
}

Void CCtrlEditorScene::Initialize()
{
    Sb::CApplicationWindow* window = &GetView().GetDisplayWindow()->GetSableEditorWindow();

	window->SetRenderer( &m_SableRenderer );
	m_SableRenderer.SetView( &m_SableView );

	m_SableView.SetCamera( &m_SableCamera );

	m_SableRenderer.Initialize( window->GetSwapChain().GetInfo().RenderTargetInfo );
	m_SableView.Initialize( );

    Sb::CVector3f position;

    position.Set(0,0,-10.0f);
    m_SableCamera.GetLocalMatrix().SetTranslationComponent(position);

    m_SableCamera.SetAspect( (Float32) window->GetSwapChain().GetInfo().RenderTargetInfo.TextureInfo.Height / (Float32) window->GetSwapChain().GetInfo().RenderTargetInfo.TextureInfo.Width );
    m_SableCamera.SetFOV( MathPi / 2.5f );
	m_SableCamera.SetNearPlane( 0.05f );

    if( m_SableRenderer.GetGuiRenderPass() )
        m_SableRenderer.GetGuiRenderPass()->ShowCursor( FALSE );

	AddCameraGizmo( *GetProject() );
}

Void CCtrlEditorScene::OnProjectDestroyed()
{
    if( GetApp().GetEditorContainerView().GetCurrentEditor() )
        GetApp().GetEditorContainerView().GetCurrentEditor()->setVisible( FALSE );

    CCtrl::OnProjectDestroyed();
}


Void CCtrlEditorScene::FitObject( const CNodeSpatial& obj )
{
	CBakeObjectSet* bakeObjSet = GetBakeContext().GetObjectSet( obj.GetHandle() );
	if( bakeObjSet == NULL )
		return;

	Sb::CNodeSpatial* node = bakeObjSet->GetObj<Sb::CNodeSpatial>();

	Sb::CSphere boundingSphere;

	node->GetWorldBoundingSphere( boundingSphere );

	m_CamCtrl.FitTo( boundingSphere );
}

Sb::CCameraController& CCtrlEditorScene::GetCameraController()
{
	return m_CamCtrl;
}

Void CCtrlEditorScene::FitScene()
{

}

Void CCtrlEditorScene::OnObjectSelected( const CHandle& current )
{
	CCtrlEditor::OnObjectSelected( current );
	CBankObject* bankObj = GetProject()->GetObj( current );

	if( bankObj->GetTypeInfo().IsKindOf( CNodeSpatial::GetStaticTypeInfo() ) )
	{
		m_Manipulator.SetAttachedNode( current );
	}
}

Void CCtrlEditorScene::OnRemoveObject( const CHandle& current )
{
	CCtrlEditor::OnRemoveObject( current );

	if( m_Manipulator.GetAttachedNode() == current )
		m_Manipulator.SetAttachedNode( CHandle() );

	if( m_AABoxTreeTable.HasItem( current ) ) 
	{
		m_AABoxTreeTable.RemoveItemAtKey( current );
	}

	if( m_GizmoTable.HasItem( current ) ) 
	{
		m_GizmoTable.RemoveItemAtKey( current );
	}	
}

Void CCtrlEditorScene::OnObjectDeSelected( const CHandle& current )
{
	CCtrlEditor::OnObjectDeSelected( current );

	if( m_Manipulator.GetAttachedNode() == current )
		m_Manipulator.SetAttachedNode( CHandle() );

}

CHandle CCtrlEditorScene::Pick( const Sb::CVector2f& pos, Sb::CAABoxTree::SIntersection& inter, Sb::CRay& r )
{
    AABoxTreeTable::Iterator it;

    m_PickedNode = CHandle();

    r = m_SableView.GetCamera()->GetRayFrom2DPos( pos );

    ForEachItem( it, GetAABoxTreeTable() )
    {
        Sb::CRay tmp = r;
		Dune::CNodeRender* node = GetProject()->GetObj<Dune::CNodeRender>( it.GetKey() );
		Sb::CMatrix4x4f m = node->GetTransformation().GetValue();
		m.Invert();
		tmp.Transform( m );

		AABoxTreeList::Iterator listIt;
		ForEachItem( listIt, *it )
		{
			inter = listIt->Intersect( tmp );
			if( inter.Dist > -1.0f )
			{
				m_PickedNode = it.GetKey();
				break;
			}
		}

		if( m_PickedNode.IsValid() )
			break;
    }

    CCmd* cmd = NULL;
    CCmdMeta* metaCmd = NEWOBJ( CCmdMeta, ( "PickObject" ) );

    if( !InputManager.GetKeyboard()->GetIsKeyDown( nKeyboardEventData_LeftControl ) )
    {
        Sb::CArray<CHandle> objList( GetApp().GetProjectCtrl().GetSelectedObjects() );
        cmd = NEWOBJ( CCmdSelectObject, ( GetApp().GetProjectCtrl(), objList.GetBuffer(), objList.GetItemCount(), CCmdSelectObject::nAction_DeSelect) );
        metaCmd->PushCommand( *cmd );
    }


    if( m_PickedNode.IsValid() )
    {
        m_Manipulator.SetAttachedNode( m_PickedNode );

        // Select picked object
        CCmdSelectObject::EAction action = CCmdSelectObject::nAction_Select;
        cmd = NEWOBJ( CCmdSelectObject, ( GetApp().GetProjectCtrl(), m_PickedNode, action ) );
        metaCmd->PushCommand( *cmd );
    }
    else
    {
        m_Manipulator.SetAttachedNode( CHandle() );
    }

    GetApp().GetCommandProcessor().Submit( *metaCmd );

    return CHandle();
}

Sb::CCameraPerspective& CCtrlEditorScene::GetSableCamera() 
{
    return m_SableCamera;
}

CManipulator& CCtrlEditorScene::GetManipulator()
{
    return m_Manipulator;
}

Void CCtrlEditorScene::MoveElement( const Sb::CVector2i& pos )
{

}

Void CCtrlEditorScene::Render( Sb::CRenderer& renderer )
{
	m_Manipulator.Render( renderer );

	GizmoTable::Iterator it;
	ForEachItem( it, m_GizmoTable )
	{
		(*it)->Render( renderer );
	}
}

Void CCtrlEditorScene::StartDraging( const Sb::CVector2i& pos )
{

}

Bool CCtrlEditorScene::IsDraging() const
{
	return FALSE;
}

Void CCtrlEditorScene::StopDraging( const Sb::CVector2i& pos )
{
}

const CCtrlEditorScene::AABoxTreeTable& CCtrlEditorScene::GetAABoxTreeTable() const
{
    return m_AABoxTreeTable;
}


const CHandle& CCtrlEditorScene::GetRootObject() const
{
    return m_RootElement;
}

Void CCtrlEditorScene::OnEditElement( const CHandle& h )
{
    m_RootElement = h;

	GetBakeContext().BakeObject( m_RootElement, TRUE, FALSE );

    CBakeObjectSet* set = GetBakeObjectSet( h );
    m_SableView.SetWorld( set->GetObj<Sb::CSceneWorld>() );

    // Build AABoxTree table used for hit test
    Sb::CList<CHandle>::Iterator it;
    Sb::CList<CHandle> nodeRenderList;
    GetProject()->IssueQuery( "IsKindOf( CNodeRender )", nodeRenderList );

    ForEachItem( it, nodeRenderList )
    {
		AddToAABoxTree( *it );
    }	
}

Void CCtrlEditorScene::OnAddObject( const CHandle& parent, const CHandle& h )
{
	CCtrlEditor::OnAddObject( parent, h );
	CBankObject* obj = GetProject()->GetObj( h );
	DebugAssert( obj );

	if( obj->GetTypeInfo().IsKindOf( CCameraPerspective::GetStaticTypeInfo() ) )
	{
		CCameraPerspective* cam = (CCameraPerspective*)obj;
		AddCameraGizmo( *cam );
	}
}

Void CCtrlEditorScene::AddCameraGizmo( CCameraPerspective& cam  )
{
	CGizmo* gizmo = NEWOBJ( CGizmoCamera, ( GetApp(), cam.GetHandle() ) );
	m_GizmoTable.AddItem( cam.GetHandle(), gizmo );
}

Void CCtrlEditorScene::AddCameraGizmo( CProject& p )
{
	CBankObject* rootObj = p.GetObj( GetRootObject() );	

	Sb::CList<CHandle> cameraList;
	Sb::CList<CHandle>::Iterator it;

	rootObj->FindChildByType( CCameraPerspective::GetStaticTypeInfo(), cameraList );

	ForEachItem( it, cameraList )
	{
		CCameraPerspective* cam = (CCameraPerspective*)p.GetObj( *it );
		DebugAssert( cam );
		AddCameraGizmo( *cam );
	}
}

Void CCtrlEditorScene::OnPropertyChanged( const CPropertyInterface& prop, const Sb::CString& oldValue )
{
	CCtrlEditor::OnPropertyChanged( prop, oldValue );
	if( prop.GetObj()->GetTypeInfo().IsKindOf( CNodeRender::GetStaticTypeInfo() ) )
	{
		if( !Sb::StringCompare( prop.GetName(), CNodeRender::PropModel ) )
		{
			AddToAABoxTree( prop.GetObj()->GetHandle() );
		}
	}
}

Void CCtrlEditorScene::AddToAABoxTree( const CHandle& h )
{
	CBakeObjectSet* objSet = GetBakeObjectSet( h );
	if( !objSet )
		return;

	if( m_AABoxTreeTable.HasItem( h ) )
		return;

	Sb::CNodeRender* nodeRender = objSet->GetObj<Sb::CNodeRender>();
	if( nodeRender && nodeRender->GetModel() )
	{
		AABoxTreeList& aaboxTreeList = m_AABoxTreeTable.AddItem( h );

		for( UInt32 i=0; i<nodeRender->GetModel()->GetSubsetCount(); i++ )
		{
			Sb::CAABoxTree& aaboxTree = aaboxTreeList.AddItemAtEnd();

			Sb::CGeometryMesh* mesh = nodeRender->GetModel()->GetSubset( i ).Mesh;
			Sb::CGeometryVertexBuffer* vtxStream = mesh->GetVertexBuffer();
			Sb::CGeometryIndexBuffer* idxBuffer = mesh->GetIndexBuffer();
			Sb::CGeometryVertexLayout* vlBuffer = mesh->GetVertexLayout();

			Sb::CBufferVisitor vbVisitor = vlBuffer->GetBufferVisitor( nVertexComponent_Position, vtxStream->GetVertexCount(), vtxStream->GetRawData() );
			Sb::CBufferVisitor ibVisitor = idxBuffer->GetBufferVisitor( idxBuffer->GetRawData() );

			aaboxTree.Build( vbVisitor, ibVisitor );
		}
	}
}