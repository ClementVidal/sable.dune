#include <Dune/Common/ObjLoader.h>

#include <Sable/Core/Common/Buffer.h>
#include <Sable/Core/File/File.h>

using namespace Dune;

CObjLoader::CObjLoader( )
{
}

CObjLoader::~CObjLoader( )
{
}

Bool CObjLoader::Load( const Sb::CFilePath& filePath )
{
	Sb::CFile file;
	if( !file.Open( filePath, nAccesMode_Read ) )
		return FALSE;

	return Load( file );

}

Bool CObjLoader::Load( const Sb::CString& data )
{
	Sb::CArray<Byte> byteArray;
	Sb::CBuffer buffer;
	
	byteArray.SetItemCount( data.GetLength()+1 );
	Sb::MemoryCopy( data.GetBuffer(), byteArray.GetBuffer(), data.GetLength()+1 );

	buffer.Open( byteArray, nAccesMode_Read );

	return Load( buffer );
}

const CObjLoader::SVertexTable& CObjLoader::GetVertexTable() const
{
	return m_VertexTable;
}

const CObjLoader::SFaceTable& CObjLoader::GetFaceTable() const
{
	return m_FaceTable;
}

Bool CObjLoader::Load( Sb::CIODevice& device )
{
	Char line[1024];
	UInt32 posCount = 0;
	UInt32 faceCount = 0;

	while( device.ReadLine( line, 1024 ) != -1)
	{
		if( line[0] == 'v' )
			posCount++;
		else if( line[0] == 'f' )
			faceCount++;
	}

	m_FaceTable.SetItemCount( faceCount );
	m_VertexTable.SetItemCount( posCount );

	device.SetPosition( 0 );

	faceCount = 0;
	posCount = 0;

	while( device.ReadLine( line, 1024 ) != -1)
	{
		if( line[0] == 'v' )
			ParsePosition( line, posCount++ );
		else if( line[0] == 'f' )
			ParseFace( line, faceCount++ );
	}

	return TRUE;
}

Bool CObjLoader::ParsePosition( String line, UInt32 index )
{
	Float32 x = 0.0f;
	Float32 y = 0.0f;
	Float32 z = 0.0f;

	if( sscanf( line, "v %f %f %f", &x, &y, &z ) != 3 )
		return FALSE;

	m_VertexTable[index].Pos = Sb::CVector3f( x, y, z );

	return FALSE;
}

Bool CObjLoader::ParseFace( String line, UInt32 index )
{
	UInt32 a = 0;
	UInt32 b = 0;
	UInt32 c = 0;

	if( sscanf( line, "f %d %d %d", &a, &b, &c ) != 3 )
		return FALSE;

	m_FaceTable[index].Id[0] = a-1;
	m_FaceTable[index].Id[1] = b-1;
	m_FaceTable[index].Id[2] = c-1;

	return TRUE;
}
