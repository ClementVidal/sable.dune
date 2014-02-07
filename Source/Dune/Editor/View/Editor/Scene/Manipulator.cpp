#include <Dune/Editor/View/Editor/Scene/Manipulator.h>

#include <Dune/Editor/View/Editor/Scene/ManipulatorHandleRotation.h>
#include <Dune/Editor/View/Editor/Scene/ManipulatorHandleTranslation1D.h>
#include <Dune/Editor/View/Editor/Scene/ManipulatorHandleTranslation2D.h>
#include <Dune/Editor/Ctrl/Cmd/SetObjectProperty.h>

#include <Dune/Editor/Application/Application.h>

using namespace Dune;

CManipulator::CManipulator( CApplication& app )
{
	m_App = &app;
    Float32 halfPi = MathDegreeToRadian( 90.0f );
    Sb::CMatrix3x3f localTransfo;
    m_CurrentHandle = NULL;
    m_Camera = NULL;
	m_LastScreenPos = Sb::CVector2f::Zero;
	m_DragState = 0;

    localTransfo.SetFromEulerAngle( Sb::CEulerAngle( 0.0f, 0.0f, 0.0f ) );
    m_HandleTable[nType_Rotation][nAxis_X] = NEWOBJ( CManipulatorHandleRotation, (Sb::CColor::Red, nAxis_X) );
    localTransfo.SetFromEulerAngle( Sb::CEulerAngle( 0.0f, 0.0f, halfPi ) );
    m_HandleTable[nType_Rotation][nAxis_Y] = NEWOBJ( CManipulatorHandleRotation, (Sb::CColor::Green, nAxis_Y) );
    localTransfo.SetFromEulerAngle( Sb::CEulerAngle( 0.0f, halfPi, 0.0f ) );
    m_HandleTable[nType_Rotation][nAxis_Z] = NEWOBJ( CManipulatorHandleRotation, (Sb::CColor::Blue, nAxis_Z) );

    localTransfo.SetFromEulerAngle( Sb::CEulerAngle( 0.0f, 0.0f, 0.0f ) );
	m_HandleTable[nType_Translation1D][nAxis_X] = NEWOBJ( CManipulatorHandleTranslation1D, (Sb::CColor::Red, nAxis_X) );
    localTransfo.SetFromEulerAngle( Sb::CEulerAngle( 0.0f, 0.0f, halfPi ) );
	m_HandleTable[nType_Translation1D][nAxis_Y] = NEWOBJ( CManipulatorHandleTranslation1D, (Sb::CColor::Green, nAxis_Y) );
    localTransfo.SetFromEulerAngle( Sb::CEulerAngle( 0.0f, halfPi, 0.0f ) );
    m_HandleTable[nType_Translation1D][nAxis_Z] = NEWOBJ( CManipulatorHandleTranslation1D, (Sb::CColor::Blue, nAxis_Z) );

    localTransfo.SetFromEulerAngle( Sb::CEulerAngle( 0.0f, 0.0f, 0.0f ) );
    m_HandleTable[nType_Translation2D][nAxis_X] = NEWOBJ( CManipulatorHandleTranslation2D, (Sb::CColor::Green, Sb::CColor::Blue, nAxis_X) );
    localTransfo.SetFromEulerAngle( Sb::CEulerAngle( 0.0f, 0.0f, -halfPi ) );
	m_HandleTable[nType_Translation2D][nAxis_Y] = NEWOBJ( CManipulatorHandleTranslation2D, (Sb::CColor::Red, Sb::CColor::Blue, nAxis_Y) );
    localTransfo.SetFromEulerAngle( Sb::CEulerAngle( 0.0f, -halfPi, 0.0f ) );
	m_HandleTable[nType_Translation2D][nAxis_Z] = NEWOBJ( CManipulatorHandleTranslation2D, (Sb::CColor::Green, Sb::CColor::Red,nAxis_Z) );

    m_Transformation.SetFromIdentity();
    m_StartDragTransformation.SetFromIdentity();

	SetMode( nMode_Translation );
    Show( FALSE );
}

CManipulator::~CManipulator()
{
}

CApplication& CManipulator::GetApp() const
{
	return *m_App;
}

Void CManipulator::Initialize( Sb::CMouseInterface::Signal& mouseSignal, CCtrlEditorScene& ctrl )
{
    m_Ctrl = &ctrl;
	EVENT_CONNECT_SLOT( mouseSignal, m_SlotMouseEvent, &CManipulator::OnMouseEvent );
}

Void CManipulator::Show( Bool onOff )
{
    for( UInt32 i=0;i<nType_Count;i++ )
    {
        for( UInt32 j=0;j<nAxis_Count;j++ )
        {
            CManipulatorHandle* handle = m_HandleTable[i][j];
            handle->SetIsVisible( FALSE );
        }
    }

    if( onOff == FALSE )
        return;

    if( m_Mode == nMode_Rotation )
    {
        m_HandleTable[nType_Rotation][nAxis_X]->SetIsVisible( TRUE );
        m_HandleTable[nType_Rotation][nAxis_Y]->SetIsVisible( TRUE );
        m_HandleTable[nType_Rotation][nAxis_Z]->SetIsVisible( TRUE );
    }
    else if( m_Mode == nMode_Translation )
    {
        m_HandleTable[nType_Translation1D][nAxis_X]->SetIsVisible( TRUE );
        m_HandleTable[nType_Translation1D][nAxis_Y]->SetIsVisible( TRUE );
        m_HandleTable[nType_Translation1D][nAxis_Z]->SetIsVisible( TRUE );

        m_HandleTable[nType_Translation2D][nAxis_X]->SetIsVisible( TRUE );
        m_HandleTable[nType_Translation2D][nAxis_Y]->SetIsVisible( TRUE );
        m_HandleTable[nType_Translation2D][nAxis_Z]->SetIsVisible( TRUE );
    }

}

Void CManipulator::SetMode( CManipulator::EMode mode )
{
	m_Mode = mode;
    Show(  GetAttachedNode().IsValid() );
}

Void CManipulator::Render( const Sb::CRenderer& renderer )
{
    m_Camera = renderer.GetView()->GetCamera();

    if( GetAttachedNode().IsValid() == FALSE || m_Mode == nMode_None )
        return;

	renderer.GetDebugRenderPass()->PushDepthTestMode( FALSE );
    for( UInt32 i=0;i<nType_Count;i++ )
    {
        for( UInt32 j=0;j<nAxis_Count;j++ )
        {
            CManipulatorHandle* handle = m_HandleTable[i][j];
            handle->Render( renderer, GetTransformMatrix() );
        }
    }
	renderer.GetDebugRenderPass()->PopDepthTestMode( );
}

CHandle CManipulator::GetAttachedNode() const
{
    return m_AttachedNode;
}

Void CManipulator::SetAttachedNode( CHandle h )
{
	m_AttachedNode = h;

	Show( GetAttachedNode().IsValid() );


	CNodeSpatial* node = GetProject()->GetObj<CNodeSpatial>( h );
	if( node )
	{
		CNodeSpatial* node = GetProject()->GetObj<CNodeSpatial>( h );

        m_Transformation = node->GetTransformation().GetValue();

		EVENT_CONNECT_SLOT( node->GetTransformation().SignalPropertyChanged, m_SlotPropertyChanged, &CManipulator::OnPropertyChanged );
	}
	else
	{
		m_SlotPropertyChanged.DisconnectAll();
	}
}

Void CManipulator::OnPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue )
{
	CNodeSpatial* node = GetProject()->GetObj<CNodeSpatial>( m_AttachedNode );

	DebugAssert( node );

	if( &p == &node->GetTransformation() )
	{
		m_Transformation = node->GetTransformation().GetValue();
	}
}

Void CManipulator::OnMouseEvent( EMouseEventType type, EMouseEventData data, const Sb::CVector2f& pos )
{
	Sb::CVector2f newPos = InputManager.GetMouse()->GetPosition();
	Sb::CVector2f delta;

	delta = m_LastScreenPos - newPos;
	m_LastScreenPos = newPos;

	// Prepare for a new potential drag sequence
	if( type == nMouseEventType_KeyDown )
	{
		m_DragState = 1;
		m_DragBtn = data;
	}
	// End drag sequence
	else if( type == nMouseEventType_KeyUp )
	{
		if( m_DragState == 2 )
		{
			EndDrag( newPos );
		}
		m_DragState = 0;
	}
	else if( type == nMouseEventType_Move )
	{
		// Start a drag Sequence
		if( m_DragState == 1 )
		{
			m_LastScreenPos = InputManager.GetMouse()->GetPosition();
			delta = m_LastScreenPos - newPos;
			m_DragState = 2;
			StartDrag( newPos );
		}
		// Drag
		else if( m_DragState == 2 )
		{
			Drag( newPos );
		}
		// Move without dragging
		else
		{
			Move( newPos );
		}
	}
}

Void CManipulator::StartDrag( const Sb::CVector2f& p )
{
    DebugAssert( m_Camera ); 
    DebugAssert( m_CurrentHandle );

    if( m_Mode == nMode_None )
        return;

    m_StartDragTransformation = m_Transformation;

    m_CurrentHandle->StartDrag( p, *m_Camera, GetTransformMatrix() );
}

Void CManipulator::CancelDrag()
{
    DebugAssert( m_CurrentHandle );

    if( m_Mode == nMode_None )
        return;

    m_Transformation = m_StartDragTransformation;

    m_CurrentHandle->CancelDrag( );

    m_StartDragTransformation.SetFromIdentity();
}

Sb::CMatrix4x4f  CManipulator::GetTransformMatrix() const
{
    return m_Transformation;
}

Void CManipulator::Drag( const Sb::CVector2f& p )
{
    DebugAssert( m_Camera ); 
    DebugAssert( m_CurrentHandle );

    if( m_Mode == nMode_None )
        return;

    Sb::CVector3f localDeltaTranslation = Sb::CVector3f::Zero;
    Sb::CEulerAngle localDeltaRotation = Sb::CEulerAngle::Zero;

    m_CurrentHandle->Drag( p, *m_Camera, GetTransformMatrix(), localDeltaTranslation, localDeltaRotation );

    Sb::CMatrix4x4f deltaRotMtx, deltaTrsMtx, final;
    deltaRotMtx.SetFromEulerAngle( localDeltaRotation );
    deltaTrsMtx.SetFromTranslation( localDeltaTranslation );
    final = deltaTrsMtx * deltaRotMtx ;

    m_Transformation = final * m_StartDragTransformation;

    CNodeSpatial* node = GetProject()->GetObj<CNodeSpatial>( m_AttachedNode );
	if( node )
	{
		node->GetTransformation().SetValue( m_Transformation );
	}
}

Void CManipulator::EndDrag( const Sb::CVector2f& p )
{
    DebugAssert( m_Camera ); 
    DebugAssert( m_CurrentHandle );

    if( m_Mode == nMode_None )
        return;

    m_CurrentHandle->EndDrag( p, *m_Camera, GetTransformMatrix() );

    CNodeSpatial* node = GetProject()->GetObj<CNodeSpatial>( GetAttachedNode() );
	if( node )
	{
		CCmdSetObjectProperty* command = NULL;

        command = NEWOBJ( CCmdSetObjectProperty, ( m_Ctrl->GetApp().GetProjectCtrl(), node->GetTransformation(), m_StartDragTransformation, m_Transformation ) );
        m_Ctrl->GetApp().GetCommandProcessor().Submit( *command );
	}

    m_StartDragTransformation.SetFromIdentity();
}

CProject* CManipulator::GetProject() const
{
	return m_App->GetProject();
}

Void CManipulator::Move( const Sb::CVector2f& pos )
{
    if( !m_Camera )
        return; 

    if( m_Mode == nMode_None )
        return;

    if( m_DragState != 2 )
    {
        for( UInt32 i=0;i<nType_Count;i++ )
        {
            for( UInt32 j=0;j<nAxis_Count;j++ )
            {
                CManipulatorHandle* handle = m_HandleTable[i][j];
                handle->SetIsHighlighted( FALSE );
            }
        }

        m_CurrentHandle = NULL;

        Float32 minDist = FLOAT32_MAX_VAL;
        CManipulatorHandle* closestHandle = NULL;

        for( UInt32 i=0;i<nType_Count;i++ )
        {
            for( UInt32 j=0;j<nAxis_Count;j++ )
            {
                CManipulatorHandle* handle = m_HandleTable[i][j];
                if( handle->GetIsVisible()  )
                {
                    Float32 d = handle->Intersect( pos, *m_Camera, GetTransformMatrix() );
                    if( d > -1.0f && d < minDist )
                    {
                        minDist = d;
                        closestHandle = handle;
                    }
                }
            }
        }

        if( closestHandle )
        {
            m_CurrentHandle = closestHandle;
            m_CurrentHandle->SetIsHighlighted( TRUE );
        }
    }
}

CManipulatorHandle* CManipulator::GetCurrentHandle() const
{
    return m_CurrentHandle;
}

Bool CManipulator::GetIsDragging() const
{
    return m_DragState == 2;
}