#include <Dune\Model\Asset\FBX\Manager.h>

using namespace Dune;

CFBXManager::CFBXManager( )
{
	// Create the FBX SDK manager
	m_SdkManager = FbxManager::Create();

	m_IOSettings = FbxIOSettings::Create(m_SdkManager, IOSROOT );
	m_SdkManager->SetIOSettings(m_IOSettings);
}

CFBXManager::~CFBXManager()
{

}

FbxIOSettings* CFBXManager::GetFBXIOSettings() const
{
	return m_IOSettings;
}

FbxManager*	CFBXManager::GetFBXSdkManager() const
{
	return m_SdkManager;
}

CFBXDoc* CFBXManager::Add( const Sb::CFilePath& path )
{
	CFBXDoc* dae = Get( path );

	if( !dae )
	{
		dae = NEWOBJ( CFBXDoc, ( *this ) );
		dae->Load( path );
		m_Table.AddItem( path, dae );
	}

	return dae;
}

CFBXDoc* CFBXManager::Get( const Sb::CFilePath& path )
{
	if( m_Table.HasItem( path ) )
		return m_Table.GetItem( path );

	return NULL;
}

CFBXDoc* CFBXManager::GetOrAdd( const Sb::CFilePath& path )
{
	CFBXDoc* dae = Get( path );

	if( dae == NULL )
		dae = Add( path );

	return dae;
}

Void CFBXManager::Remove( const Sb::CFilePath& path )
{
	CFBXDoc* dae = Get( path );

	if( dae )
		m_Table.RemoveItemAtKey( path );
}
