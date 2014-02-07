#include <Dune/Model/Asset/FBX/Doc.h>

#include <Dune/Common/Log.h>

#include <fbxsdk.h>

IMPLEMENT_MANAGED_CLASS( Dune, CFBXDoc );

using namespace Dune;

CFBXDoc::CFBXDoc( CFBXManager& fbxManager ):
	Sb::CManagedObject()
{
	m_FBXManager = &fbxManager;
	m_FBXScene = NULL;
}

CFBXDoc::~CFBXDoc()
{
	if( m_FBXScene )
		m_FBXScene->Destroy();
}

Bool CFBXDoc::Load( const Sb::CFilePath& filePath )
{
	m_Path.SetEmpty();

	Sb::DebugLogInfo( "Loading FBX document: %s ...", filePath.GetPath() );

	Char buffer[512];
	Sb::StringSetFormattedText( buffer, 512, "file:///%s", filePath.GetPath() );

	FbxImporter* fbxImporter = NULL;

	fbxImporter = FbxImporter::Create( m_FBXManager->GetFBXSdkManager(), "");
	if( !fbxImporter->Initialize( filePath.GetBuffer(), -1, m_FBXManager->GetFBXIOSettings() ) )
	{
		CoreManager.GetLog().LogError( "Failed to load FBX file: %s", fbxImporter->GetStatus().GetErrorString() );
		// Destroy the importer.
		fbxImporter->Destroy(); 

		m_FBXScene = NULL;

		return FALSE;
	}

	m_FBXScene = FbxScene::Create( m_FBXManager->GetFBXSdkManager(), filePath.GetBuffer() );

	fbxImporter->Import( m_FBXScene );

	// Destroy the importer.
	fbxImporter->Destroy(); 

	m_Path = filePath;
	QFileInfo fileName( QString( (char*)GetPath().GetPath() ) );
	m_LastLoadingDate = fileName.lastModified();

	FbxAxisSystem coordSys( FbxAxisSystem::eDirectX );
	coordSys.ConvertScene( m_FBXScene );

	return TRUE;
}

FbxScene* CFBXDoc::GetFBXScene() const
{
	return m_FBXScene;
}

/**
Reload the FBX if necessary, i.e if the last modification date is 
less than the current modification date
\return TRUE if the FBX was effectivly loaded, FALSE otherwise
*/
Bool CFBXDoc::ReloadIfNecessary()
{
	if( GetPath().IsValid() )
	{
		if( IsValid() )
		{
			QFileInfo fileName( QString( (char*)GetPath().GetPath() ) );
			QDateTime time = fileName.lastModified();

			if( time != m_LastLoadingDate )
			{
				ReLoad();
				return TRUE;
			}
		}
		else
		{
			ReLoad( );
			return TRUE;
		}
	}

	return FALSE;
}

Void CFBXDoc::ReLoad()
{
	UnLoad();
	Load( m_Path );
}

Void CFBXDoc::UnLoad( )
{
	m_FBXScene->Destroy();
	m_FBXScene = NULL;
}

CFBXObject* CFBXDoc::GetObject( String id )
{
	FbxNode* n = GetObject( m_FBXScene->GetRootNode(), (FbxUInt64) Sb::StringToInt32( id ) );
	if( n )
	{
		return NEWOBJ( CFBXObject, (*this, n ) );
	}

	return NULL;
}

FbxNode* CFBXDoc::GetObject( FbxNode* root, FbxUInt64 id )
{
	for( Int32 i=0; i<root->GetChildCount(); i++ )
	{
		if( root->GetChild( i )->GetUniqueID() == id )
		{
			return (FbxNode*) root->GetChild( i );
		}
		else
		{
			GetObject( root->GetChild( i ), id );
		}
	}
	return NULL;
}

const Sb::CFilePath& CFBXDoc::GetPath() const
{
	return m_Path;
}

Bool CFBXDoc::IsValid() const
{
	return m_FBXScene && m_Path.Exist();
}