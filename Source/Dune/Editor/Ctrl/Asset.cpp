#include <Dune/Editor/Ctrl/Asset.h>

#include <Dune/Editor/View/Property.h>
#include <Dune/Model/Asset/Header.h>
#include <Dune/Editor/View/AssetTree.h>
#include <Dune/Editor/Ctrl/Project.h>
#include <Dune/Model/Property/Property.h> 
#include <Dune/Common/LexicalCast.h>
#include <Dune/Common/LibraryObject.h>
#include <Dune/Editor/Ctrl/Cmd/Header.h>
#include <Dune/Bake/DependencySolver.h>

using namespace Dune;

CCtrlAsset::CCtrlAsset( CApplication& app ) :
    CCtrl( app )
{
}

CCtrlAsset::~CCtrlAsset()
{
}

/**
Use this method to add Controller related processing when an object is created
*/
Void CCtrlAsset::OnAddObject( const CHandle& parent, const CHandle& h )
{
	CAssetParticleSystem* particleSystem = GetProject()->GetObj<CAssetParticleSystem>( h );

	// When creating a new ParticleSystem, automatically Look for a default material and shader
	// and assign it
	if( particleSystem )
	{
		Sb::CList<CHandle> list;
		Sb::CList<CHandle>::Iterator it;
		CAssetMaterial* defaultMaterial = NULL ;
		CAssetShader* defaultShader = NULL ;

		GetProject()->IssueQuery( "Name = DefaultParticleSystemMaterial", list );
		if( list.GetItemCount() )
			defaultMaterial = GetProject()->GetObj<CAssetMaterial>( list.GetFirstItem() );

		GetProject()->IssueQuery( "Name = DefaultParticleSystemShader", list );
		if( list.GetItemCount() )
			defaultShader = GetProject()->GetObj<CAssetShader>( list.GetFirstItem() );

		if( defaultMaterial == NULL || defaultShader == NULL )
		{
			CCmdAddObject* addMaterial = NULL;
			CCmdAddObject* addShader = NULL;

			if( !defaultMaterial )
			{
				addMaterial = NEWOBJ( CCmdAddObject, ( GetApp().GetProjectCtrl(), GetProject()->GetRootAsset(), CAssetMaterial::GetStaticTypeInfo() ) );	
				GetApp().GetCommandProcessor().Submit( *addMaterial );	
			}
			if( !defaultShader )
			{
				addShader =	NEWOBJ( CCmdAddObject, ( GetApp().GetProjectCtrl(), GetProject()->GetRootAsset(), CAssetShader::GetStaticTypeInfo() ) );
				GetApp().GetCommandProcessor().Submit( *addShader );	
			}

			if( addMaterial )
			{
				defaultMaterial = GetProject()->GetObj<CAssetMaterial>( addMaterial->GetAddedObject() );
                CCmdSetObjectProperty* cmd = NEWOBJ( CCmdSetObjectProperty, ( GetApp().GetProjectCtrl(),  *defaultMaterial->GetProperty( "Name" ), Sb::CString( "DefaultParticleSystemMaterial" ) ) );
				GetApp().GetCommandProcessor().Submit( *cmd );
			}
			if( addShader )
			{
				defaultShader = GetProject()->GetObj<CAssetShader>( addShader->GetAddedObject() );
				CCmdSetObjectProperty* cmd = NEWOBJ( CCmdSetObjectProperty, ( GetApp().GetProjectCtrl(),  *defaultShader->GetProperty( "Name" ), Sb::CString( "DefaultParticleSystemShader" ) ) );
				GetApp().GetCommandProcessor().Submit( *cmd );
			}
		}

		DebugAssert( defaultShader && defaultMaterial );

		CCmdSetObjectProperty* cmd = NULL;
		
		cmd = NEWOBJ( CCmdSetObjectProperty, ( GetApp().GetProjectCtrl(), 
			defaultShader->GetVertexShaderFilePath(), 
			CoreManager.GetFileSystem().GetFilePath( "System/Shader", "RendererPass/Particle/Particle.vsh") ) );
		GetApp().GetCommandProcessor().Submit( *cmd );

		cmd = NEWOBJ( CCmdSetObjectProperty, ( GetApp().GetProjectCtrl(), 
			defaultShader->GetPixelShaderFilePath(), 
			CoreManager.GetFileSystem().GetFilePath( "System/Shader", "RendererPass/Particle/Particle.psh" ) ) );
		GetApp().GetCommandProcessor().Submit( *cmd );

		cmd = NEWOBJ( CCmdSetObjectProperty, ( GetApp().GetProjectCtrl(), 
			defaultShader->GetGeometryShaderFilePath(), 
			CoreManager.GetFileSystem().GetFilePath( "System/Shader", "RendererPass/Particle/Particle.gsh" ) ) );
		GetApp().GetCommandProcessor().Submit( *cmd );

        defaultMaterial->GetShader().SetValue( defaultShader->GetHandle() );
		particleSystem->GetMaterial().SetValue( defaultMaterial->GetHandle() );
	}
}

Void CCtrlAsset::ExportToLibrary( CBankObject& obj, const Sb::CFilePath& path )
{
	CLibraryObject libObj( obj, "" );

	libObj.Save( path );

}