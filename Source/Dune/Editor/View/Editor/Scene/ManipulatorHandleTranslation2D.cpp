#include <Dune\Editor\View\Editor\Scene\ManipulatorHandleTranslation2D.h>

#define AXIS_UNIT_LENGTH (1.0f/8.0f)

using namespace Dune;

CManipulatorHandleTranslation2D::CManipulatorHandleTranslation2D( const Sb::CColor& color1, const Sb::CColor& color2, EAxis supportAxis ):
    CManipulatorHandle( supportAxis )
{
	m_Color1 = color1;
	m_Color2 = color2;
}

CManipulatorHandleTranslation2D::~CManipulatorHandleTranslation2D( )
{
}

Void CManipulatorHandleTranslation2D::InternalDraw( const Sb::CRenderer& renderer, const Sb::CMatrix4x4f& m )
{
	Float32 distFromCamera = Sb::CVector3f( renderer.GetView()->GetCamera()->GetWorldPosition() - m.GetTranslationComponent() ).GetLength();
	Float32 axisLength = AXIS_UNIT_LENGTH * distFromCamera;

	Sb::CDebugRenderPass* debug = renderer.GetDebugRenderPass();

	debug->PushViewProjMatrix();
	debug->PushWorldMatrix( m );

	debug->BeginLineList();

	// First line
	if( GetIsHighlighted() )
		debug->PushColor( Sb::CColor::White );
	else
		debug->PushColor( m_Color2 );

    if( GetSupportAxis() == nAxis_X )
        debug->DrawLine( Sb::CVector3f::ZAxis * axisLength, ( Sb::CVector3f::ZAxis + Sb::CVector3f::YAxis ) * axisLength );
    else if( GetSupportAxis() == nAxis_Y )
        debug->DrawLine( Sb::CVector3f::ZAxis* axisLength, ( Sb::CVector3f::ZAxis + Sb::CVector3f::XAxis ) * axisLength );
    else if( GetSupportAxis() == nAxis_Z )
        debug->DrawLine( Sb::CVector3f::XAxis* axisLength, ( Sb::CVector3f::XAxis + Sb::CVector3f::YAxis ) * axisLength );


	debug->PopColor();

	// Second Line
	if( GetIsHighlighted() )
		debug->PushColor( Sb::CColor::White );
	else
		debug->PushColor( m_Color1 );

    if( GetSupportAxis() == nAxis_X )
	    debug->DrawLine( Sb::CVector3f::YAxis * axisLength, ( Sb::CVector3f::YAxis + Sb::CVector3f::ZAxis ) * axisLength );
    else if( GetSupportAxis() == nAxis_Y )
        debug->DrawLine( Sb::CVector3f::XAxis * axisLength, ( Sb::CVector3f::XAxis + Sb::CVector3f::ZAxis ) * axisLength );
    else if( GetSupportAxis() == nAxis_Z )
        debug->DrawLine( Sb::CVector3f::YAxis * axisLength, ( Sb::CVector3f::YAxis + Sb::CVector3f::XAxis ) * axisLength );


	debug->PopColor();

	debug->EndLineList();

	debug->PopWorldMatrix();
	debug->PopViewProjMatrix();

}

Float32 CManipulatorHandleTranslation2D::Intersect( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m )
{
	Float32 distFromCamera = Sb::CVector3f( cam.GetWorldPosition() - m.GetTranslationComponent() ).GetLength();
	Float32 axisLength = AXIS_UNIT_LENGTH * distFromCamera;

	Float32 dist = -1.0f;
	Sb::CVector3f v1;
	Sb::CVector3f v2;
	Sb::CVector3f ori = Sb::CVector3f::Zero;
    Sb::CRay r = cam.GetRayFrom2DPos( p );

    if( GetSupportAxis() == nAxis_X )
    {
        v1 = Sb::CVector3f::YAxis * axisLength;
        v2 = Sb::CVector3f::ZAxis * axisLength;
    }
    else if( GetSupportAxis() == nAxis_Y )
    {
        v1 = Sb::CVector3f::XAxis * axisLength;
        v2 = Sb::CVector3f::ZAxis * axisLength;
    }
    else if( GetSupportAxis() == nAxis_Z )
    {
        v1 = Sb::CVector3f::XAxis * axisLength;
        v2 = Sb::CVector3f::YAxis * axisLength;
    }

	v1.Transform( m );
	v2.Transform( m );
	ori.TransformPosition( m );

	Sb::CTriangle t1( ori, ori + v1, ori + v2);
	Sb::CTriangle t2( ori + v1 + v2, ori + v2, ori + v1);

    Float32 d = -1.0f;
    
    d = t1.IntersectWithoutCulling( r );
	if( d > -1.0f )
		return d;
    d = t2.IntersectWithoutCulling( r );
    if( d > -1.0f )
        return d;

	return -1.0f;
}

Void CManipulatorHandleTranslation2D::StartDrag( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m  )
{
    Sb::CRay r = cam.GetRayFrom2DPos( p );
    Sb::CVector3f worldAxis = Sb::CVector3f::Axis[GetSupportAxis()];
    worldAxis.Transform( m );
    Sb::CPlane plane( m.GetTranslationComponent(), worldAxis );

    Float32 dist = plane.Intersect( r );
    DebugAssert( dist > -1.0f );
    m_StartDragPos = r.GetPointAt( dist );
}

Void CManipulatorHandleTranslation2D::Drag( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m, Sb::CVector3f& deltaTranslation, Sb::CEulerAngle& deltaRotation )
{
    Sb::CRay r = cam.GetRayFrom2DPos( p );

    Sb::CVector3f worldAxis = Sb::CVector3f::Axis[GetSupportAxis()];
    worldAxis.Transform( m );
    Sb::CPlane plane( m.GetTranslationComponent(), worldAxis );

    Float32 dist = plane.Intersect( r );

    Sb::CVector3f currentPos = r.GetPointAt( dist );
    deltaTranslation = currentPos - m_StartDragPos;

    // Transform offset back to local space
    Sb::CMatrix4x4f im = m;
    im.Invert();
    deltaTranslation.Transform( im );
}

Void CManipulatorHandleTranslation2D::EndDrag( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m  )
{

}

Void CManipulatorHandleTranslation2D::Highlight( )
{

}

Void CManipulatorHandleTranslation2D::CancelDrag( )
{

}

#undef AXIS_UNIT_LENGTH