#include <Dune/Editor/View/Editor/Scene/ManipulatorHandleRotation.h>

using namespace Dune;

#define AXIS_UNIT_LENGTH (1.0f/4.0f)
#define MANIPULATORHANDLEROTATION_RESOLUTION 32.0f

CManipulatorHandleRotation::CManipulatorHandleRotation( const Sb::CColor& color, EAxis supportAxis ) :
    CManipulatorHandle( supportAxis )
{
	m_Color = color;
    m_CanDisplayDirectionVector = FALSE;
}

CManipulatorHandleRotation::~CManipulatorHandleRotation( )
{
}

Void CManipulatorHandleRotation::InternalDraw( const Sb::CRenderer& renderer, const Sb::CMatrix4x4f& m )
{
	Float32 distFromCamera = Sb::CVector3f( renderer.GetView()->GetCamera()->GetWorldPosition() - m.GetTranslationComponent() ).GetLength();
	Float32 axisLength = AXIS_UNIT_LENGTH * distFromCamera;

    Sb::CMatrix4x4f identity;
    identity.SetFromTranslation( m.GetTranslationComponent() );

    Sb::CDebugRenderPass* debug = renderer.GetDebugRenderPass();

    debug->PushViewProjMatrix();
    debug->PushWorldMatrix( m );

    if( GetIsHighlighted() )
        debug->PushColor( Sb::CColor::White );
    else
        debug->PushColor( m_Color );

    debug->BeginLineStrip();

    UInt32 i;
    if( GetSupportAxis() == nAxis_X )
    {
        for(i=0;i<=MANIPULATORHANDLEROTATION_RESOLUTION;i++)
        {
            Float32 t = (i/MANIPULATORHANDLEROTATION_RESOLUTION)*MathPi*2.0f;
            debug->DrawLine( Sb::CVector3f( 0.0f, Sb::MathSin( t ) * axisLength, Sb::MathCos( t ) * axisLength ) );
        }
    }
    else if( GetSupportAxis() == nAxis_Y )
    {
        for(i=0;i<=MANIPULATORHANDLEROTATION_RESOLUTION;i++)
        {
            Float32 t = (i/MANIPULATORHANDLEROTATION_RESOLUTION)*MathPi*2.0f;
            debug->DrawLine( Sb::CVector3f( Sb::MathSin( t ) * axisLength, 0.0f, Sb::MathCos( t ) * axisLength ) );
        }
    }
    else if( GetSupportAxis() == nAxis_Z )
    {
        for(i=0;i<=MANIPULATORHANDLEROTATION_RESOLUTION;i++)
        {
            Float32 t = (i/MANIPULATORHANDLEROTATION_RESOLUTION)*MathPi*2.0f;
            debug->DrawLine( Sb::CVector3f( Sb::MathSin( t ) * axisLength, Sb::MathCos( t ) * axisLength, 0.0f ) );
        }
    }

    debug->EndLineStrip();

    debug->PopWorldMatrix();

    if( m_CanDisplayDirectionVector )
    {
        debug->PushWorldMatrix( identity );

        Sb::CVector3f centerToDrag = m_StartDragPos - m.GetTranslationComponent();
        centerToDrag.Normalize();
        centerToDrag = centerToDrag * axisLength;

        debug->BeginLineList();
        debug->DrawLine( centerToDrag, centerToDrag + m_DragTangent * axisLength );
        debug->EndLineList();

        debug->PopWorldMatrix();
    }

    debug->PopColor();

    debug->PopViewProjMatrix();
}

Float32 CManipulatorHandleRotation::Intersect( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m )
{
	Float32 distFromCamera = Sb::CVector3f( cam.GetWorldPosition() - m.GetTranslationComponent() ).GetLength();
	Float32 axisLength = AXIS_UNIT_LENGTH * distFromCamera;

    Sb::CRay r = cam.GetRayFrom2DPos( p );
    Sb::CDisk disk( Sb::CVector3f::Axis[GetSupportAxis()], Sb::CVector3f::Zero, axisLength );
    disk.Transform( m );

    return disk.Intersect( r );
}

Void CManipulatorHandleRotation::StartDrag( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m  )
{
    Sb::CRay r = cam.GetRayFrom2DPos( p );
    Sb::CVector3f worldAxis = Sb::CVector3f::Axis[GetSupportAxis()];
    worldAxis.Transform( m );
    Sb::CPlane plane( m.GetTranslationComponent(), worldAxis );

    Float32 dist = plane.Intersect( r );

    m_StartDragPos = r.GetPointAt( dist );

    m_CanDisplayDirectionVector = FALSE;
    m_StartDragTangent = Sb::CVector3f::Zero;
    m_DragTangent = Sb::CVector3f::Zero;

}

Void CManipulatorHandleRotation::Drag( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m, Sb::CVector3f& deltaTranslation, Sb::CEulerAngle& deltaRotation )
{
	Float32 distFromCamera = Sb::CVector3f( cam.GetWorldPosition() - m.GetTranslationComponent() ).GetLength();
	Float32 axisLength = AXIS_UNIT_LENGTH * distFromCamera;

    Sb::CRay r = cam.GetRayFrom2DPos( p );
    Sb::CVector3f worldAxis = Sb::CVector3f::Axis[GetSupportAxis()];
    worldAxis.Transform( m );
    Sb::CPlane plane( m.GetTranslationComponent(), worldAxis );

    Float32 dist = plane.Intersect( r );

    Sb::CVector3f currentPos = r.GetPointAt( dist );
    Sb::CVector3f dragDelta = currentPos - m_StartDragPos;

    Float32 deltaLength = dragDelta.GetLength();


    // Calculons la tangent lors du premier drag
    if( !m_CanDisplayDirectionVector )
    {
        Sb::CVector3f centerToDrag = m_StartDragPos - m.GetTranslationComponent();
        centerToDrag.Normalize();
        centerToDrag = centerToDrag * axisLength;
        m_StartDragTangent = worldAxis.CrossProduct( centerToDrag );
        m_StartDragTangent.Normalize();

        m_CanDisplayDirectionVector = TRUE;
    }

    // Reajuste la tangente
    dragDelta.Normalize();
    Float32 coef = m_StartDragTangent.DotProduct( dragDelta );
    if( coef < 0.0f )
        m_DragTangent = -m_StartDragTangent;
    else
        m_DragTangent = m_StartDragTangent;

    // Calcule l'angle en fonction du vecteur de drag et du vecteur tangent
    Float32 angle = - deltaLength*coef / 4.0f;
    deltaRotation = Sb::CEulerAngle( Sb::CVector3f::Axis[GetSupportAxis()] * angle );

}

Void CManipulatorHandleRotation::EndDrag( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m  )
{
    m_CanDisplayDirectionVector = FALSE;
}

Void CManipulatorHandleRotation::Highlight( )
{

}

Void CManipulatorHandleRotation::CancelDrag( )
{

}