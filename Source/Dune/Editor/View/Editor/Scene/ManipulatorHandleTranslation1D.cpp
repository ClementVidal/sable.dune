#include <Dune/Editor/View/Editor/Scene/ManipulatorHandleTranslation1D.h>

#include <Sable/Core/Geometry/Cylinder.h>

#define AXIS_UNIT_LENGTH (1.0f/4.0f)
#define ARROW_UNIT_LENGTH (1.0f/8.0f)

using namespace Dune;

CManipulatorHandleTranslation1D::CManipulatorHandleTranslation1D( const Sb::CColor& color, EAxis supportAxis ):
    CManipulatorHandle( supportAxis )
{
	m_Color = color;

	CObjLoader loader;
	loader.Load( 
#include <Dune/Resources/Mesh/Arrow.inc>
		);

	const CObjLoader::SFaceTable& faceTable = loader.GetFaceTable();
	const CObjLoader::SVertexTable& vertexTable = loader.GetVertexTable();

	m_ArrowMesh.SetItemCount( faceTable.GetItemCount() );
	for( UInt32 i=0;i<faceTable.GetItemCount(); i++ )
	{
		m_ArrowMesh[i] = Sb::CTriangle( vertexTable[faceTable[i].Id[0]].Pos, vertexTable[faceTable[i].Id[1]].Pos, vertexTable[faceTable[i].Id[2]].Pos );
	}
}

CManipulatorHandleTranslation1D::~CManipulatorHandleTranslation1D( )
{
}

Void CManipulatorHandleTranslation1D::InternalDraw( const Sb::CRenderer& renderer, const Sb::CMatrix4x4f& m )
{
	Float32 distFromCamera = Sb::CVector3f( renderer.GetView()->GetCamera()->GetWorldPosition() - m.GetTranslationComponent() ).GetLength();
	Float32 axisLength = AXIS_UNIT_LENGTH * distFromCamera;
    Sb::CRay r;
    r.Origin = Sb::CVector3f::Zero;
    r.Direction = Sb::CVector3f::Axis[GetSupportAxis()];

    Sb::CDebugRenderPass* debug = renderer.GetDebugRenderPass();

    debug->PushViewProjMatrix();
    debug->PushWorldMatrix( m );

    if( GetIsHighlighted() )
        debug->PushColor( Sb::CColor::White );
    else
        debug->PushColor( m_Color );

    debug->DrawRay( r, axisLength );

	// Draw Arrow
	{
		Sb::CMatrix4x4f offset;
		Sb::CMatrix4x4f scale;
		Sb::CEulerAngle rotationAngles[nAxis_Count];
		rotationAngles[nAxis_X] = Sb::CEulerAngle( 0.0f, 0.0f, - MathPi / 2.0f );
		rotationAngles[nAxis_Y] = Sb::CEulerAngle( 0.0f, 0.0f, 0.0f );
		rotationAngles[nAxis_Z] = Sb::CEulerAngle( -MathPi / 2.0f , 0.0f, 0.0f );

		scale.SetFromScale( ARROW_UNIT_LENGTH * axisLength );
		
		offset.SetFromEulerAngle( rotationAngles[ GetSupportAxis() ] );
		offset.SetTranslationComponent( Sb::CVector3f::Axis[ GetSupportAxis() ] * axisLength );
	
		debug->PushColor( m_Color );
		debug->PushWorldMatrix( scale * offset * m );
		debug->PushFillMode( nStateFillMode_Solid );
		debug->DrawTriangleList( m_ArrowMesh );
		debug->PopFillMode();
		debug->PopWorldMatrix();
		debug->PopColor();
	}

	debug->PopColor();
		
    debug->PopWorldMatrix();
    debug->PopViewProjMatrix();
}

Float32 CManipulatorHandleTranslation1D::Intersect( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m )
{
	Float32 distFromCamera = Sb::CVector3f( cam.GetWorldPosition() - m.GetTranslationComponent() ).GetLength();
	Float32 axisLength = AXIS_UNIT_LENGTH * distFromCamera;

    Sb::CCylinder cyl;
    Sb::CRay r = cam.GetRayFrom2DPos( p );

    cyl.PointA = Sb::CVector3f::Zero;
    cyl.PointB = Sb::CVector3f::Axis[GetSupportAxis()] * axisLength;
    cyl.Radius = 0.02f * distFromCamera;

    cyl.Transform( m );

    return cyl.Intersect( r );
}

Void CManipulatorHandleTranslation1D::StartDrag( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m  )
{
    Sb::CRay r = cam.GetRayFrom2DPos( p );
    Float32 dist = Sb::CVector3f( r.Origin - m.GetTranslationComponent() ).GetLength();
	m_StartDragPos = r.GetPointAt( dist );
}

Void CManipulatorHandleTranslation1D::Drag( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m, Sb::CVector3f& deltaTranslation, Sb::CEulerAngle& deltaRotation )
{
    Sb::CRay r = cam.GetRayFrom2DPos( p );
    Float32 dist = Sb::CVector3f( r.Origin - m.GetTranslationComponent() ).GetLength();
    Sb::CVector3f dragPos = r.GetPointAt( dist );

    Sb::CVector3f dragDelta = m_StartDragPos - dragPos;
    Sb::CVector3f dragDir = dragDelta;
    dragDir.Normalize();

    Sb::CVector3f worldAxis = Sb::CVector3f::Axis[GetSupportAxis()];
    worldAxis.Transform( m );

    Float32 coef = dragDir.DotProduct( worldAxis );
    Float32 deltaDist = - dragDelta.GetLength() * coef;

    // Return offset in local space
    deltaTranslation = Sb::CVector3f::Axis[GetSupportAxis()] * deltaDist;
}

Void CManipulatorHandleTranslation1D::EndDrag( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m  )
{

}

Void CManipulatorHandleTranslation1D::Highlight( )
{

}

Void CManipulatorHandleTranslation1D::CancelDrag( )
{

}


#undef AXIS_UNIT_LENGTH
#undef ARROW_UNIT_LENGTH