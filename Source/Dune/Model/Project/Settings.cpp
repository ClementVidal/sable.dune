#include <Dune\Model\Project\Settings.h>

using namespace Dune;

CProjectSettings::CProjectSettings( )
{
}

CProjectSettings::~CProjectSettings()
{
}

const CProjectSettings::PathList& CProjectSettings::GetAssetPathList() const
{
	return m_AssetPathList;
}

Void CProjectSettings::AddAssetPath( const Sb::CFilePath& path )
{
	if( !m_AssetPathList.HasItem( path ) )
		m_AssetPathList.AddItemAtBegin( path );
}

Bool CProjectSettings::Serialize( Sb::CPersistentArchive& ar )
{
    ar.Serialize( m_Name, WT("Name") );
    ar.Serialize( m_Description, WT("Description") );
    ar.Serialize( m_BakeOutputPath, WT("BakeOutputPath") );

	Int32 pathCount = m_AssetPathList.GetItemCount();
	ar.Serialize( pathCount, WT("AssetPathCount") );

	if( ar.IsLoading() )
		m_AssetPathList.SetItemCount( pathCount );

	Int16 version = 0;
	if( ar.BeginScope( WT("AssetPath"), version ) == version )
	{
		PathList::Iterator it;
		ForEachItem( it, m_AssetPathList )
		{
			ar.Serialize( *it, WT("Path") );
		}

		ar.EndScope();
	}

    return TRUE;
}

Void CProjectSettings::SetName( String name )
{
    m_Name = name;
}

String CProjectSettings::GetName() const
{
    return m_Name;
}

Void CProjectSettings::SetDescription( String name )
{
    m_Description = name;
}

String CProjectSettings::GetDescription() const
{
    return m_Description;
}

const Sb::CFilePath& CProjectSettings::GetBakeOutputPath() const
{
	return m_BakeOutputPath;
}

Void CProjectSettings::SetBakeOutputPath( const Sb::CFilePath& path )
{
	m_BakeOutputPath = path;
}