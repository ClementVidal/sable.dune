#ifndef _DUNE_MODEL_ASSET_FBX_MODELCONVERTER_
#define _DUNE_MODEL_ASSET_FBX_MODELCONVERTER_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Graphics/Geometry/Header.h>

#define MAXUVSET 8

namespace Dune
{

class CFBXObject;

/**
\ingroup ModelAsset

Helper class used to process a collada domMesh 
*/
class CFBXModelConverter 
{

public:

	/** @name DataTypes*/
	//@{
	struct SVertex
	{
		bool operator == ( const SVertex& other ) const;
		SVertex& operator = ( const SVertex& other ) ;
		Sb::CVector3f	Pos;
		Sb::CVector2f	UV[MAXUVSET];
		Sb::CVector3f	Normal;
		Sb::CVector3f	Tangent;
		Sb::CVector3f	Binormal;
	};
	typedef Sb::CArray<SVertex> VertexBuffer;
    //@}	

    /** @name Constructor/Destructor*/
    //@{
    CFBXModelConverter( CBakeContext& context, CFBXObject& obj );
    ~CFBXModelConverter();


    /** @name Accessors*/
    //@{
	//@}

    /** @name Manipulator*/
    //@{
	Bool	ConvertVisualGeometry( Sb::CGeometryModel& model );
	//@}

private:

    // DataTypes

	// Attributes
	CBakeContext*		m_Context;
	CFBXObject*			m_FBXObject;
	Sb::CBitArray32		m_Components;
	UInt32				m_VertexByteCount;

	// Functions
	Bool SplitPolygonsByMaterial( Sb::CArray< Sb::CArrayUInt32 >& polygons );
	Bool CheckSupportedData( FbxMesh* fbxMesh );
	Bool CreateGlobalVertexBuffer( VertexBuffer& vertexDataArray );
	Bool SplitBuffer( const Sb::CArrayUInt32& polygonGroup, const VertexBuffer& globalVertexBuffer, VertexBuffer& vertexBuffer );
	Bool IndexVertexBuffer( const VertexBuffer& src, VertexBuffer& dst, Sb::CArrayUInt32& indexArray );
	Bool CreateGeometryMesh( const VertexBuffer& vertexBuffer, const Sb::CArrayUInt32& indexBuffer, Sb::CGeometryMesh& mesh );

	Sb::CGeometryVertexLayout*	CreateVertexLayout( const Sb::CBitArray32& component );
	Sb::CGeometryVertexBuffer*	CreateVertexBuffer( const VertexBuffer& srcBuffer );
	Sb::CGeometryIndexBuffer*	CreateIndexBuffer( const Sb::CArrayUInt32& srcBuffer );

	Sb::CVector3f	ConvertPosition( const Sb::CVector3f& p ) const;
	Sb::CVector3f	ConvertVector( const Sb::CVector3f& p ) const;

};

template< class DataType >
HashValue ComputeHashValue( const CFBXModelConverter::SVertex& data )
{
	Sb::CCrc32 crc;
	crc.Compute( (Byte*) &data, sizeof( CFBXModelConverter::SVertex ) );
	return crc.Get();
}

};

#endif