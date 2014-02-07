#include <Dune/Common/LibraryObject.h>

#include <Dune/Common/XMLDataLayout.h>
#include <Dune/Bake/DependencySolver.h>
#include <Sable/Core/Persistent/Archive.h>

#include <Dune/Resources/Icons/UnknownLibraryObject.xpm>

#include <QtGui/QPixmap>

using namespace Dune;

CLibraryObject::CLibraryObject( )
{
	m_Object = NULL;
	LoadUnknownLibraryObjectIcon();
}

CLibraryObject::CLibraryObject( CBankObject& object, const Sb::CString& description )
{
	Sb::CMap<CHandle,CHandle>	bankMappingTable;

	m_Object = &object;
	m_Description = description;

	m_Bank.Clear();
	m_Bank.Merge( object, CHandle(), bankMappingTable );

	m_Object = m_Bank.GetObj( bankMappingTable.GetItem( object.GetHandle() ) );
	DebugAssert( m_Object );

	LoadUnknownLibraryObjectIcon();
}

CLibraryObject::~CLibraryObject()
{
}

Void CLibraryObject::LoadUnknownLibraryObjectIcon()
{
	QPixmap iconPixmap( UnknownLibraryObject_xpm );
	QImage image = iconPixmap.toImage().convertToFormat( QImage::Format_ARGB32 );

	m_ThumbRawData.SetItemCount( image.byteCount() );
	Sb::MemoryCopy( image.bits(), m_ThumbRawData.GetBuffer(), image.byteCount() );
}

Bool CLibraryObject::Serialize( Sb::CPersistentArchive& ar )
{
	CHandle object;
	UInt32 tagsCount = m_Tags.GetItemCount();
	UInt32 thumbRawDataByteCount = m_ThumbRawData.GetItemCount();

	if( ar.IsStoring() )
	{
		DebugAssert( m_Object );
		object = m_Object->GetHandle();
	}

	Int16 version = 0;
	if( ar.BeginScope( WT("MetaInfo"), version ) == version )
	{
		ar.Serialize( m_Description, WT( "Description" ) );
		ar.Serialize( tagsCount, WT("TagsCount") );
		if( ar.IsLoading() )
			m_Tags.SetItemCount( tagsCount );

		for( UInt32 i=0;i<tagsCount;i++ )
		{
			ar.Serialize( m_Tags[i], WT( "Tag" ) );
		}

		ar.Serialize( object, WT( "Object" ) );

		ar.Serialize( thumbRawDataByteCount, WT("ThumbRawDataByteCount") );
		if( ar.IsLoading() )
			m_ThumbRawData.SetItemCount( thumbRawDataByteCount );

		ar.Serialize( m_ThumbRawData.GetBuffer(), thumbRawDataByteCount, WT("ThumbRawData") );

		ar.EndScope();
	}

	ar.Serialize( m_Bank, WT( "Bank" ) );

	ar.FlushData();

	if( ar.IsLoading() )
	{
		DebugAssert( m_Object == NULL );
		m_Object = m_Bank.GetObj( object );
		DebugAssert( m_Object );
	}
	return TRUE;
}

const Sb::CFilePath& CLibraryObject::GetFilePath() const
{
	return m_FilePath;
}

const Sb::CArray<Byte>& CLibraryObject::GetThumbRawData() const
{
	return m_ThumbRawData;
}

const Sb::CArray<Sb::CString>& CLibraryObject::GetTags() const
{
	return m_Tags;
}

CBankObject* CLibraryObject::GetObject() const
{
	return m_Object;
}

String CLibraryObject::GetName() const
{
	if( m_Object )
		return m_Object->GetName();

	return NULL;
}


Void CLibraryObject::SetDescription( String desc )
{
	m_Description = desc;
}

const Sb::CString& CLibraryObject::GetDescription() const
{
	return m_Description;
}

CBank& CLibraryObject::GetBank()
{
	return m_Bank;
}

Bool CLibraryObject::Load( const Sb::CFilePath& path )
{
	if( path.IsValid() == FALSE )
		return FALSE;

	m_FilePath = path;

	Sb::CXMLDataLayout      dataLayout( path );
	Sb::CPersistentArchive ar(dataLayout, nAccesMode_Read );

	m_Bank.Clear();

	Serialize( ar );

	return TRUE;
}

Bool CLibraryObject::Save( const Sb::CFilePath& path )
{
	if( m_Object == NULL || path.IsValid() == FALSE )
		return FALSE;

	Sb::CFilePath finalPath = path;
	finalPath.SetExtension( "dlibobj" );

	m_FilePath = path;

	return Save();
}


Bool CLibraryObject::Save( )
{
	Sb::CXMLDataLayout      dataLayout( m_FilePath );
	Sb::CPersistentArchive ar(dataLayout, nAccesMode_Write );

	if( ! Serialize( ar ) )
		return FALSE;

	return TRUE;
}

