#include <Dune/Model/Asset/FBX/ModelConverter.h>

#include <Sable/Physic/Common/Header.h>
#include <Sable/Graphics/Geometry/Header.h>


using namespace Dune;

CFBXModelConverter::SVertex& CFBXModelConverter::SVertex::operator = ( const SVertex& other ) 
{
	Pos = other.Pos;
	Normal = other.Normal;
	Binormal = other.Binormal;
	Tangent = other.Tangent;

	for( Int32 i=0;i<MAXUVSET; i++ )
	{
		UV[i] = other.UV[i];
	}

	return *this;
}

bool CFBXModelConverter::SVertex::operator == ( const SVertex& other ) const
{
	Bool sameUv = TRUE;

	for( Int32 i=0;i<MAXUVSET; i++ )
	{
		if( UV[i] != other.UV[i] )
		{
			sameUv = FALSE;
			break;
		}
	}
	return Pos == other.Pos && Normal == other.Normal && Tangent == other.Tangent && Binormal == other.Binormal && sameUv;
}

CFBXModelConverter::CFBXModelConverter( CBakeContext& context, CFBXObject& obj )
{
	m_Context = &context;
	m_FBXObject = &obj;
	m_Components.Clear();
	m_VertexByteCount = 0;
	obj.GetDoc().ReloadIfNecessary();
}

CFBXModelConverter::~CFBXModelConverter()
{
}

Bool CFBXModelConverter::SplitPolygonsByMaterial( Sb::CArray< Sb::CArrayUInt32 >& polygonGroups )
{
	FbxNode* fbxNode = ((FbxNode*)m_FBXObject->GetFBXObject());
	FbxMesh* fbxMesh = fbxNode->GetMesh();
	Int32 vtxIndex = 0;

	const FbxLayer* layerMaterial = fbxMesh->GetLayer( 0, FbxLayerElement::eMaterial );
	if( layerMaterial == NULL )
	{	
		int t = fbxMesh->GetControlPointsCount();

		polygonGroups.SetItemCount( 1 );
		Sb::CArrayUInt32& polygonArray = polygonGroups[0];
		polygonArray.SetItemCount( fbxMesh->GetPolygonCount() );
		for( Int32 i=0;i<fbxMesh->GetPolygonCount();i++ )
		{
			polygonArray[i] = i;
		}
		return TRUE;
	}
	const FbxLayerElementMaterial* layerElementMaterial = layerMaterial->GetMaterials();

	FbxLayerElement::EMappingMode mappingMode = layerElementMaterial->GetMappingMode();
	FbxLayerElement::EReferenceMode referenceMode = layerElementMaterial->GetReferenceMode();

	polygonGroups.SetItemCount( fbxNode->GetMaterialCount() );

	if( referenceMode == FbxLayerElement::eIndexToDirect )
	{
		// In case of Multi sub mat
		if( mappingMode == FbxLayerElement::eByPolygon )
		{
			// For each material initialize storage
			for( Int32 materialIndex=0; materialIndex<fbxNode->GetMaterialCount(); materialIndex++ )
			{
				Sb::CArrayUInt32& polygonArray = polygonGroups[materialIndex];

				vtxIndex = 0;
				for( Int32 i=0;i<layerElementMaterial->GetIndexArray().GetCount();i++ )
				{
					if( materialIndex == layerElementMaterial->GetIndexArray()[i] )
					{
						vtxIndex++;
					}
				}
				polygonArray.SetItemCount( vtxIndex );
			}

			// For each material fill polygon table
			for( Int32 materialIndex=0; materialIndex<fbxNode->GetMaterialCount(); materialIndex++ )
			{
				Sb::CArrayUInt32& polygonArray = polygonGroups[materialIndex];

				vtxIndex = 0;
				for( Int32 i=0;i<layerElementMaterial->GetIndexArray().GetCount();i++ )
				{
					if( materialIndex == layerElementMaterial->GetIndexArray()[i] )
					{
						polygonArray[vtxIndex++] = i;
					}
				}
			}
		}
		else if( mappingMode == FbxLayerElement::eAllSame )
		{
			Sb::CArrayUInt32& polygonArray = polygonGroups[0];
			polygonArray.SetItemCount( fbxMesh->GetPolygonCount() );
			for( Int32 i=0;i<fbxMesh->GetPolygonCount();i++ )
			{
				polygonArray[i] = i;
			}
		}
		else
		{
			CoreManager.GetLog().LogError("FBX: Unsupported material mapping mode");
			return FALSE;
		}
	}
	else
	{
		CoreManager.GetLog().LogError("FBX: Unsupported material reference mode");
		return FALSE;
	}


	return TRUE;
}

Sb::CGeometryVertexLayout* CFBXModelConverter::CreateVertexLayout( const Sb::CBitArray32& component )
{
	Sb::CGeometryVertexLayout* vl = NEWOBJ( Sb::CGeometryVertexLayout, () );

	vl->AddElement( nVertexComponent_Position, nVertexComponentType_Float3 );
	m_VertexByteCount = sizeof( Sb::CVector3f );

	if( component.GetBit( nVertexComponent_Normal ) )
	{
		vl->AddElement( nVertexComponent_Normal, nVertexComponentType_Float3 );
		m_VertexByteCount += sizeof( Sb::CVector3f );
	}
	
	if( component.GetBit( nVertexComponent_BiNormal ) )
	{
		vl->AddElement( nVertexComponent_BiNormal, nVertexComponentType_Float3 );
		m_VertexByteCount += sizeof( Sb::CVector3f );
	}	
	
	if( component.GetBit( nVertexComponent_Tangent ) )
	{
		vl->AddElement( nVertexComponent_Tangent, nVertexComponentType_Float3 );
		m_VertexByteCount += sizeof( Sb::CVector3f );
	}

	if( component.GetBit( nVertexComponent_TexCoord0 ) )
	{
		vl->AddElement( nVertexComponent_TexCoord0, nVertexComponentType_Float2 );
		m_VertexByteCount += sizeof( Sb::CVector2f );
	}

	return vl;
}

Sb::CVector3f CFBXModelConverter::ConvertPosition( const Sb::CVector3f& p ) const
{
	return Sb::CVector3f( p.GetX(), p.GetZ(), p.GetY() );
}

Sb::CVector3f CFBXModelConverter::ConvertVector( const Sb::CVector3f& p ) const
{
	return Sb::CVector3f( p.GetX(), p.GetZ(), p.GetY() );
}

Sb::CGeometryVertexBuffer* CFBXModelConverter::CreateVertexBuffer( const VertexBuffer& srcBuffer )
{
	Sb::CGeometryVertexBuffer* vb = NEWOBJ( Sb::CGeometryVertexBuffer, () );


	Sb::CArray<Byte> buffer( srcBuffer.GetItemCount() * m_VertexByteCount );
	Byte* rawBuffer = buffer.GetBuffer();

	for( UInt32 i=0;i<srcBuffer.GetItemCount(); i++ )
	{
		*((Sb::CVector3f*)rawBuffer) = ConvertPosition( srcBuffer[i].Pos );
		rawBuffer += sizeof( Sb::CVector3f );

		if( m_Components.GetBit( nVertexComponent_Normal ) )
		{
			*((Sb::CVector3f*)rawBuffer) = ConvertVector( srcBuffer[i].Normal );
			rawBuffer += sizeof( Sb::CVector3f );
		}
		if( m_Components.GetBit( nVertexComponent_BiNormal ) )
		{
			*((Sb::CVector3f*)rawBuffer) = ConvertVector( srcBuffer[i].Binormal );
			rawBuffer += sizeof( Sb::CVector3f );
		}
		if( m_Components.GetBit( nVertexComponent_Tangent ) )
		{
			*((Sb::CVector3f*)rawBuffer) = ConvertVector( srcBuffer[i].Tangent );
			rawBuffer += sizeof( Sb::CVector3f );
		}
		if( m_Components.GetBit( nVertexComponent_TexCoord0 ) )
		{
			*((Sb::CVector2f*)rawBuffer) = srcBuffer[i].UV[0];
			rawBuffer += sizeof( Sb::CVector2f );
		}
	}

	vb->SetRawData( srcBuffer.GetItemCount(), m_VertexByteCount, buffer.GetBuffer() );

	return vb;
}

Sb::CGeometryIndexBuffer* CFBXModelConverter::CreateIndexBuffer( const Sb::CArrayUInt32& srcBuffer )
{
	Sb::CGeometryIndexBuffer* ib = NEWOBJ( Sb::CGeometryIndexBuffer, () );

	DebugAssert( srcBuffer.GetItemCount() % 3 == 0 );

	UInt32 triCount = srcBuffer.GetItemCount() / 3;
	for( UInt32 i = 0; i<triCount; i++ )
	{
		UInt32 tmp = srcBuffer[i*3 + 0];
		srcBuffer[i*3 + 0] = srcBuffer[i*3 + 2];
		srcBuffer[i*3 + 2] = tmp;
	}

	ib->SetRawData( srcBuffer.GetItemCount(), sizeof( UInt32 ), (Byte*)srcBuffer.GetBuffer() );

	return ib;
}

Bool CFBXModelConverter::CheckSupportedData( FbxMesh* fbxMesh )
{
	m_Components.Clear();
	Int32 layerCountNormal = fbxMesh->GetLayerCount( FbxLayerElement::eNormal );
	if( layerCountNormal > 1 )
	{
		CoreManager.GetLog().LogError("FBX: Too many normal layer");
		return FALSE;
	}
	else if( layerCountNormal == 1 )
	{
		m_Components.SetBit( nVertexComponent_Normal );
	}

	Int32 layerCountBiNormal = fbxMesh->GetLayerCount( FbxLayerElement::eBiNormal );
	if( layerCountBiNormal > 1 )
	{
		CoreManager.GetLog().LogError("FBX: Too many bi normal layer");
		return FALSE;
	}
	else if( layerCountBiNormal == 1 )
	{
		m_Components.SetBit( nVertexComponent_BiNormal );
	}

	Int32 layerCountTangent = fbxMesh->GetLayerCount( FbxLayerElement::eTangent );
	if( layerCountTangent > 1 )
	{
		CoreManager.GetLog().LogError("FBX: Too many tangent layer");
		return FALSE;
	}
	else if( layerCountTangent == 1 )
	{
		m_Components.SetBit( nVertexComponent_Tangent );
	}

	Int32 layerCountUV = fbxMesh->GetLayerCount( FbxLayerElement::eUV );
	if( layerCountUV > 1 )
	{
		CoreManager.GetLog().LogError("FBX: Too many tangent layer");
		return FALSE;
	}
	else if( layerCountUV == 1 )
	{
		m_Components.SetBit( nVertexComponent_TexCoord0 );
	}

	return TRUE;
}

Bool  CFBXModelConverter::CreateGlobalVertexBuffer( VertexBuffer& vertexDataArray )
{
	FbxNode* fbxNode = ((FbxNode*)m_FBXObject->GetFBXObject());
	FbxMesh* fbxMesh = fbxNode->GetMesh();

	UInt32 ctrlPointCount = fbxMesh->GetControlPointsCount();

	if( !CheckSupportedData( fbxMesh ) )
		return FALSE;

	Int32 i = 0, vtx = 0;

	vertexDataArray.SetItemCount( fbxMesh->GetPolygonVertexCount() );

	// Extract position
	vtx = 0;
	for( i=0; i<fbxMesh->GetPolygonCount(); i++ )
	{
		for( Int32 j=0; j<fbxMesh->GetPolygonSize( i ); j++ )
		{
			FbxVector4 v = fbxMesh->GetControlPointAt( fbxMesh->GetPolygonVertex( i, j ) );

			vertexDataArray[vtx++].Pos = Sb::CVector3f( v[0], v[1], v[2] );
		}
	}

	// Check and extract normal
	vtx = 0;
	const FbxLayer* layerNormal = fbxMesh->GetLayer( 0, FbxLayerElement::eNormal );
	if( layerNormal )
	{
		const FbxLayerElementNormal* layerElementNormal = layerNormal->GetNormals();
		if( layerElementNormal->GetMappingMode() != FbxLayerElement::eByPolygonVertex )
		{
			CoreManager.GetLog().LogError("FBX: Invalid normal mapping mode" );
			return FALSE;
		}

		if( layerElementNormal->GetReferenceMode() == FbxLayerElement::eDirect )
		{
			for( i=0; i<layerElementNormal->GetDirectArray().GetCount(); i++ )
			{
				FbxVector4 v = layerElementNormal->GetDirectArray().GetAt( i );
				vertexDataArray[vtx++].Normal = Sb::CVector3f( v[0], v[1], v[2] );
			}
		}
		else
		{
			for( i=0; i<layerElementNormal->GetIndexArray().GetCount(); i++ )
			{
				FbxVector4 v = layerElementNormal->GetDirectArray().GetAt( layerElementNormal->GetIndexArray().GetAt( i ) );
				vertexDataArray[vtx++].Normal = Sb::CVector3f( v[0], v[1], v[2] );
			}
		}
	}

	// Check and extract bi-normal
	vtx = 0;
	const FbxLayer* layerBinormal = fbxMesh->GetLayer( 0, FbxLayerElement::eBiNormal );
	if( layerBinormal )
	{
		const FbxLayerElementBinormal* layerElementBinormal = layerBinormal->GetBinormals();
		if( layerElementBinormal->GetMappingMode() != FbxLayerElement::eByPolygonVertex )
		{
			CoreManager.GetLog().LogError("FBX: Invalid binormal mapping mode" );
			return FALSE;
		}

		if( layerElementBinormal->GetReferenceMode() == FbxLayerElement::eDirect )
		{
			for( i=0; i<layerElementBinormal->GetDirectArray().GetCount(); i++ )
			{
				FbxVector4 v = layerElementBinormal->GetDirectArray().GetAt( i );
				vertexDataArray[vtx++].Binormal = Sb::CVector3f( v[0], v[1], v[2] );
			}
		}
		else
		{
			for( i=0; i<layerElementBinormal->GetIndexArray().GetCount(); i++ )
			{
				FbxVector4 v = layerElementBinormal->GetDirectArray().GetAt( layerElementBinormal->GetIndexArray().GetAt( i ) );
				vertexDataArray[vtx++].Binormal = Sb::CVector3f( v[0], v[1], v[2] );
			}
		}
	}

	// Check and extract tangent
	vtx = 0;
	const FbxLayer* layerTangent = fbxMesh->GetLayer( 0, FbxLayerElement::eTangent );
	if( layerTangent )
	{
		const FbxLayerElementTangent* layerElementTangent = layerBinormal->GetTangents();
		if( layerElementTangent->GetMappingMode() != FbxLayerElement::eByPolygonVertex )
		{
			CoreManager.GetLog().LogError("FBX: Invalid tangent mapping mode" );
			return FALSE;
		}

		if( layerElementTangent->GetReferenceMode() == FbxLayerElement::eDirect )
		{
			for( i=0; i<layerElementTangent->GetDirectArray().GetCount(); i++ )
			{
				FbxVector4 v = layerElementTangent->GetDirectArray().GetAt( i );
				vertexDataArray[vtx++].Tangent = Sb::CVector3f( v[0], v[1], v[2] );
			}
		}
		else
		{
			for( i=0; i<layerElementTangent->GetIndexArray().GetCount(); i++ )
			{
				FbxVector4 v = layerElementTangent->GetDirectArray().GetAt( layerElementTangent->GetIndexArray().GetAt( i ) );
				vertexDataArray[vtx++].Tangent = Sb::CVector3f( v[0], v[1], v[2] );
			}
		}
	}

	// Check and extract UV	
	vtx = 0;
	const FbxLayer* layerUV = fbxMesh->GetLayer( 0, FbxLayerElement::eUV );
	if( layerUV )
	{
		const FbxLayerElementUV* layerElementUV = layerUV->GetUVs( FbxLayerElement::eTextureDiffuse );
		if( layerElementUV->GetMappingMode() != FbxLayerElement::eByPolygonVertex )
		{
			CoreManager.GetLog().LogError("FBX: Invalid normal mapping mode" );
			return FALSE;
		}

		if( layerElementUV->GetReferenceMode() == FbxLayerElement::eDirect )
		{
			for( i=0;i<layerElementUV->GetDirectArray().GetCount(); i++ )
			{
				FbxVector2 v = layerElementUV->GetDirectArray().GetAt( i );
				vertexDataArray[vtx++].UV[0] = Sb::CVector2f( v[0], v[1] );
			}
		}
		else
		{
			for( i=0; i<layerElementUV->GetIndexArray().GetCount(); i++ )
			{
				FbxVector2 v = layerElementUV->GetDirectArray().GetAt( layerElementUV->GetIndexArray().GetAt( i ) );
				vertexDataArray[vtx++].UV[0] = Sb::CVector2f( v[0], v[1] );
			}
		}
	}

	return TRUE;
}

Bool CFBXModelConverter::IndexVertexBuffer( const VertexBuffer& srcVertexBuffer, VertexBuffer& dstVertexBuffer, Sb::CArrayUInt32& indexBuffer )
{
	dstVertexBuffer.SetItemCount( srcVertexBuffer.GetItemCount() );
	indexBuffer.SetItemCount( srcVertexBuffer.GetItemCount() );
	for( UInt32 i=0; i<srcVertexBuffer.GetItemCount(); i++ )
	{
		dstVertexBuffer[i] = srcVertexBuffer[i];
		indexBuffer[i] = i;
	}

	return TRUE;

	VertexBuffer tmpDst( srcVertexBuffer.GetItemCount() );
	Sb::CHashTable<SVertex, UInt32> hashTable;

	hashTable.SetCapacity( srcVertexBuffer.GetItemCount() );
	indexBuffer.SetItemCount( srcVertexBuffer.GetItemCount() );

	Int32 dstVertexBufferSize = 0;

	// Create index buffer
	for( UInt32 i=0; i<srcVertexBuffer.GetItemCount(); i++ )
	{
		UInt32* index = hashTable.GetItemPtr( srcVertexBuffer[i] );
		if( index )
		{
			indexBuffer[i] = *index;
		}
		else
		{
			hashTable.AddItem( srcVertexBuffer[i], dstVertexBufferSize );
			tmpDst[ dstVertexBufferSize ] = srcVertexBuffer[i];
			indexBuffer[i] = dstVertexBufferSize;

			dstVertexBufferSize++;
		}
	}

	// Copy dst vertexBuffer
	dstVertexBuffer.SetItemCount( dstVertexBufferSize );
	for( Int32 i=0;i<dstVertexBufferSize; i++ )
	{
		dstVertexBuffer[i] = tmpDst[i];
	}

	return TRUE;
}

Bool CFBXModelConverter::ConvertVisualGeometry( Sb::CGeometryModel& model )
{
	FbxNode* fbxNode = ((FbxNode*)m_FBXObject->GetFBXObject());
	FbxMesh* fbxMesh = fbxNode->GetMesh();

	if( fbxMesh->IsTriangleMesh() == false )
	{
		FbxGeometryConverter geometryConverter( fbxNode->GetFbxManager() );
		if( !geometryConverter.TriangulateInPlace( fbxNode ) )
		{
			CoreManager.GetLog().LogError("FBX: Failled to triangulate mesh");
			return FALSE;
		}
		CoreManager.GetLog().LogWarning("FBX: File should be triangulated during export");
	}

	VertexBuffer globalVertexBuffer;
	if( ! CreateGlobalVertexBuffer( globalVertexBuffer ) )
		return FALSE;

	Sb::CArray< Sb::CArrayUInt32 > polygonGroups;
	if( ! SplitPolygonsByMaterial( polygonGroups ) )
		return FALSE;

	model.SetSubsetCount( polygonGroups.GetItemCount() );
	for( UInt32 i=0;i<polygonGroups.GetItemCount(); i++ )
	{
		VertexBuffer vertexBuffer;
		if( ! SplitBuffer( polygonGroups[i], globalVertexBuffer, vertexBuffer ) )
			return FALSE;

		VertexBuffer indexedVertexBuffer;
		Sb::CArrayUInt32 indexBuffer;
		if( !IndexVertexBuffer( vertexBuffer, indexedVertexBuffer, indexBuffer ) )
			return FALSE;

		Sb::CGeometryMesh* mesh = NEWOBJ( Sb::CGeometryMesh, () );
		if( ! CreateGeometryMesh( indexedVertexBuffer, indexBuffer, *mesh ) )
			return FALSE;

		model.GetSubset( i ).Mesh = mesh;
	}

	return TRUE;
}

Bool CFBXModelConverter::SplitBuffer( const Sb::CArrayUInt32& polygonGroup, const VertexBuffer& globalVertexBuffer, VertexBuffer& vertexBuffer )
{
	vertexBuffer.SetItemCount( polygonGroup.GetItemCount() * 3 );

	Int32 vtxIndex = 0;
	for( UInt32 i=0;i<polygonGroup.GetItemCount();i++ )
	{
		for( Int32 j=0; j<3; j++ )
		{
			vertexBuffer[vtxIndex] = globalVertexBuffer[ (polygonGroup[i]*3)+j ];
			vtxIndex++;
		}
	}

	return TRUE;
}

Bool CFBXModelConverter::CreateGeometryMesh( const VertexBuffer& vertexBuffer, const Sb::CArrayUInt32& indexBuffer, Sb::CGeometryMesh& mesh )
{
	Sb::CGeometryVertexLayout*	vl = CreateVertexLayout( m_Components );
	Sb::CGeometryVertexBuffer*	vb = CreateVertexBuffer( vertexBuffer );
	Sb::CGeometryIndexBuffer*	ib = CreateIndexBuffer( indexBuffer );

	mesh.SetVertexBuffer( vb );
	mesh.SetIndexBuffer( ib );
	mesh.SetVertexLayout( vl );
		
	return TRUE;
}