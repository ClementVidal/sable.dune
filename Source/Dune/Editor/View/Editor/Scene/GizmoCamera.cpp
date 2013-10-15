#include <Dune\Editor\View\Editor\Scene\GizmoCamera.h>

#include <Dune\Editor\Application\Application.h>

using namespace Dune;

CGizmoCamera::CGizmoCamera( CApplication& app, const CHandle& objHandle ) :
	CGizmo( app, objHandle )
{
	LoadMesh();
}

CGizmoCamera::~CGizmoCamera()
{
}

Void CGizmoCamera::OnPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue  )
{

}

Void CGizmoCamera::LoadMesh()
{
	CObjLoader loader;
	loader.Load( 
#include <Dune\Resources\Mesh\Camera.inc>
		);

	const CObjLoader::SFaceTable& faceTable = loader.GetFaceTable();
	const CObjLoader::SVertexTable& vertexTable = loader.GetVertexTable();

	m_CameraGizmoMesh.SetItemCount( faceTable.GetItemCount() );
	for( UInt32 i=0;i<faceTable.GetItemCount(); i++ )
	{
		m_CameraGizmoMesh[i] = Sb::CTriangle( vertexTable[faceTable[i].Id[0]].Pos, vertexTable[faceTable[i].Id[1]].Pos, vertexTable[faceTable[i].Id[2]].Pos );
	}
}

Void CGizmoCamera::Render( const Sb::CRenderer& renderer )
{
	CCameraPerspective* cam = GetProject()->GetObj<CCameraPerspective>( GetObjectHandle() );

	DebugAssert( cam );

	Sb::CDebugRenderPass* debug = renderer.GetDebugRenderPass();

	debug->PushViewProjMatrix();
	debug->PushWorldMatrix( cam->GetTransformation().GetValue() );

	debug->PushColor( Sb::CColor::Blue );
	debug->PushFillMode( nStateFillMode_WireFrame );
	debug->DrawTriangleList( m_CameraGizmoMesh );
	debug->PopFillMode();
	debug->PopColor();

	Sb::CMatrix4x4f m;
	Sb::CVector3f dir[4];
	Sb::CVector3f edge[4];

	dir[0] = -Sb::CVector3f::Axis[ nAxis_Z ];
	m.SetFromAxisAndAngle( nAxis_Y, cam->GetFieldOfView().GetValue() / 2.0f );
	dir[0].Transform( m );

	dir[1] = -Sb::CVector3f::Axis[ nAxis_Z ];
	m.SetFromAxisAndAngle( nAxis_Y, - cam->GetFieldOfView().GetValue() / 2.0f );
	dir[1].Transform( m );

	dir[2] = -Sb::CVector3f::Axis[ nAxis_Z ];
	m.SetFromAxisAndAngle( nAxis_X, cam->GetFieldOfView().GetValue() / 2.0f * cam->GetAspect().GetValue() );
	dir[2].Transform( m );

	dir[3] = -Sb::CVector3f::Axis[ nAxis_Z ];
	m.SetFromAxisAndAngle( nAxis_X, -cam->GetFieldOfView().GetValue() / 2.0f * cam->GetAspect().GetValue() );
	dir[3].Transform( m );

	edge[0] = dir[0] + dir[2];
	edge[1] = dir[0] + dir[3];
	edge[2] = dir[1] + dir[2];
	edge[3] = dir[1] + dir[3];

	edge[0].Normalize();
	edge[1].Normalize();
	edge[2].Normalize();
	edge[3].Normalize();

	edge[0] = edge[0] * cam->GetFarPlane().GetValue();
	edge[1] = edge[1] * cam->GetFarPlane().GetValue();
	edge[2] = edge[2] * cam->GetFarPlane().GetValue();
	edge[3] = edge[3] * cam->GetFarPlane().GetValue();


	debug->PushColor( Sb::CColor( 0.5f, 0.5f, 1.0f ) );

	debug->BeginLineList();

	debug->DrawLine( Sb::CVector3f::Zero, Sb::CVector3f::Zero + edge[0] );
	debug->DrawLine( Sb::CVector3f::Zero, Sb::CVector3f::Zero + edge[1] );
	debug->DrawLine( Sb::CVector3f::Zero, Sb::CVector3f::Zero + edge[2] );
	debug->DrawLine( Sb::CVector3f::Zero, Sb::CVector3f::Zero + edge[3] );

	debug->DrawLine( edge[0], edge[1] );
	debug->DrawLine( edge[1], edge[3] );
	debug->DrawLine( edge[3], edge[2] );
	debug->DrawLine( edge[2], edge[0] );

	debug->EndLineList();

	debug->PopColor();

	debug->PopWorldMatrix();
	debug->PopViewProjMatrix();
}