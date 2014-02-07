#include <Dune/Model/Asset/Material.h>

#include <Dune/Model/Asset/FBX/MaterialConverter.h>
#include <Dune/Model/Asset/Texture2D.h>
#include <Dune/Model/Asset/Texture3D.h>
#include <Dune/Model/Asset/Shader.h>
#include <Dune/Model/Asset/StatesSampler.h>
#include <Dune/Common/Log.h>

#include <Sable/Graphics/Shader/Header.h>

using namespace Dune;

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CAssetMaterial, CFBXDocInterface )

CAssetMaterial::CAssetMaterial() :
    CFBXDocInterface( )
{
    SetName("Material");
	
	m_Shader = NEWOBJ( CPropertyHandle, ( "Shader" ) );
	AddProperty( *m_Shader );
	m_Shader->AddFilter( CAssetShader::GetStaticTypeInfo() );
}

CAssetMaterial::~CAssetMaterial()
{
}

Bool CAssetMaterial::Serialize( Sb::CPersistentArchive& ar )
{
	if( !CFBXDocInterface::Serialize( ar ) )
		return FALSE;

	Int16 version = 0;
	if( ar.BeginScope( WT("Parameters"), version ) == version )
	{
		PropertyTable::Iterator it;
		const PropertyTable& pTable = GetPropertyTable();
		UInt32 parametersCount = 0;

		if( ar.IsStoring() )
		{
			ForEachItem( it, pTable )
			{
				if( Sb::StringCompare( (*it)->GetGroup(), "Parameters" ) == 0 )
				{
					parametersCount++;
				}
			}
		}

		ar.Serialize( parametersCount, WT("Count") );

		Char pName[256];
		Char fullTypeName[256];

		if( ar.IsStoring() )
		{
			ForEachItem( it, pTable )
			{
				if( (*it)->GetIsAutoSerialized() == FALSE && Sb::StringCompare( (*it)->GetGroup(), "Parameters" ) == 0 )
				{
					Sb::StringCopy( (*it)->GetName(), pName, 256 );
					Sb::StringCopy( (*it)->GetTypeInfo().GetFullTypeName(), fullTypeName, 256 );

					if( ar.BeginScope( WT("Property"), version ) == version )
					{
						ar.Serialize( pName, 256, WT("Name") );
						ar.Serialize( fullTypeName, 256, WT("Type") );
						(*it)->Serialize( ar );
						ar.EndScope();
					}
				}
			}
		}
		else
		{
			for( UInt32 i=0;i<parametersCount;i++ )
			{
				if( ar.BeginScope( WT("Property"), version ) == version )
				{
					ar.Serialize( pName, 256, WT("Name") );
					ar.Serialize( fullTypeName, 256, WT("Type") );
					CPropertyInterface* p =  GetProperty( pName );

					DebugAssert( !p );

					const Sb::CTypeInfo& info = CPropertyInterface::GetStaticTypeInfo().GetDerivedClass( fullTypeName );
					if( info == Sb::CTypeInfo::NullType )
					{
						CoreManager.GetLog().LogWarning( "Trying to deserialize material property (Name: %d) with an invalid type: %s", (UInt32)pName, fullTypeName );
					}
					else
					{
						p = (CPropertyInterface*)info.CreateObject( NULL );
						p->Initialize( pName );
						p->SetGroup( "Parameters" );
						p->SetIsAutoSerialized( FALSE );

						if( p->GetTypeInfo().IsKindOf( CPropertyHandle::GetStaticTypeInfo() ) )
						{
							((CPropertyHandle*)p)->SetSerializeFilters( TRUE );
						}

						AddProperty( *p );

						p->Serialize( ar );
					}

					ar.EndScope();
				}
			}
		}
		ar.EndScope();
	}

	return TRUE;
}

Void CAssetMaterial::PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet  ) const
{
	Sb::CMaterial* material = NEWOBJ( Sb::CMaterial, () );
	objSet.AddObject( *material );
}

Void CAssetMaterial::BakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet )
{
	Sb::CString materialName = GetName();
	context.LogInfo( Sb::CString( "LogMaterial") + materialName );

	Sb::CMaterial* material = objSet.GetObj<Sb::CMaterial>( );

	// Fill material
	if( material )
	{
		CBankObject::PropertyTable::Iterator it;
		const CBankObject::PropertyTable& table = GetPropertyTable();
		
        // Clear all existing material parameters
        material->RemoveAllParameters();

		ForEachItem( it, table )
		{
			if( Sb::StringCompare( (*it)->GetGroup(), "Parameters" ) == 0 )
			{
				Sb::CStringIdentifier name = (*it)->GetName();

				// If it's a handle property, check it's type
				if( (*it)->GetTypeInfo().IsKindOf( CPropertyHandle::GetStaticTypeInfo() ) )
				{
					CPropertyHandle* prop = &(CPropertyHandle&) *(*it);
					CBankObject* bankObj = GetObjAccessor()->GetObj( prop->GetValue() );
					if( bankObj )
					{
						if( bankObj->GetTypeInfo().IsKindOf( CAssetTexture2D::GetStaticTypeInfo() ) )
						{
							const CBakeObjectSet* set = context.GetObjectSet( bankObj->GetHandle() );
							if( set )
							{
								if( material->GetParameter( name ) == NULL )
									material->AddParameter( name );
					
								material->SetParameterValue( name, *set->GetObj<Sb::CTexture2D>( 0 ) );
							}
						}
						else if( bankObj->GetTypeInfo().IsKindOf( CAssetTexture3D::GetStaticTypeInfo() ) )
						{
							const CBakeObjectSet* set = context.GetObjectSet( bankObj->GetHandle() );
							if( set )
							{
								material->AddParameter( name );
                                material->SetParameterValue( name, *set->GetObj<Sb::CTexture3D>( 0 ) );
							}
						}
						else if( bankObj->GetTypeInfo().IsKindOf( CAssetStatesSampler::GetStaticTypeInfo() ) )
						{
							const CBakeObjectSet* set = context.GetObjectSet( bankObj->GetHandle() );
							if( set )
							{
								material->AddParameter( name );
								material->SetParameterValue( name, *set->GetObj<Sb::CStatesSampler>( 0 ) );
							}
						}
					}
				}
				// Sb::CColor property
				else if( (*it)->GetTypeInfo().IsKindOf( CPropertyColor::GetStaticTypeInfo() ) )
				{
					CPropertyColor* prop = &(CPropertyColor&) *(*it);
                    material->AddParameter( name );
                    material->SetParameterValue( name, prop->GetValue() );
				}
				// Vector3 property
				else if( (*it)->GetTypeInfo().IsKindOf( CPropertyVector3f::GetStaticTypeInfo() ) )
				{
					CPropertyVector3f* prop = &(CPropertyVector3f&) *(*it);
                    material->AddParameter( name );
                    material->SetParameterValue( name, prop->GetValue() );
				}
				// Vector2 property
				else if( (*it)->GetTypeInfo().IsKindOf( CPropertyVector2f::GetStaticTypeInfo() ) )
				{
					CPropertyVector2f* prop = &(CPropertyVector2f&) *(*it);
                    material->AddParameter( name );
                    material->SetParameterValue( name, prop->GetValue() );
				}
				else
				{
					DebugError("Invalid type");
				}
			}
		}

		{
			// Attach shaders
			CPropertyHandle* prop = (CPropertyHandle*)GetProperty( "Shader" );
			if( prop )
			{
				const CBakeObjectSet* set = context.GetObjectSet( prop->GetValue() );
				if( set )
				{
					material->SetShader( set->GetObj<Sb::CShader>( 0 ) );
				}
			}
		}

		objSet.NeedInit( TRUE );
		CAsset::BakeObjectSet( context, objSet );
	}
}

Void CAssetMaterial::InitializeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
	Sb::CMaterial* mat = objSet.GetObj<Sb::CMaterial>( );
	if( mat )
	{
		mat->Load();
	}
}

CPropertyHandle& CAssetMaterial::GetShader()
{
	return *m_Shader;
}

Void CAssetMaterial::OnSetFBXBinding()
{
/*	domMaterial* domMat = (domMaterial*) GetColladaElement();

	if( domMat == NULL )
	{
		Sb::DebugLogWarning( "Missing material: %s in collada document.", GetName() );
		return;
	}

	CFBXMaterialConverter conv( *domMat );

	SetName( conv.GetName() );

	if( conv.GetDiffuseTexture().IsValid )
	{
		CPropertyHandle* prop = (CPropertyHandle*)GetProperty( "Diffuse" );

		if( !prop )
		{
			prop = NEWOBJ( CPropertyHandle, ( "Diffuse" ) );
			prop->SetGroup( "Parameters" );
			AddProperty( *prop );
		}

		if( prop->GetValue().IsValid() == FALSE )
		{
			Char tmp[128];
			CAssetTexture2D* t = (CAssetTexture2D*)GetBank()->AddObject( CAssetTexture2D::GetStaticTypeInfo(), GetParent() );

			t->GetFilePath().SetValue( conv.GetDiffuseTexture().Path );
			t->SetName( conv.GetDiffuseTexture().Path.GetFullTypeName( tmp, 128 ) );

			prop->SetValue( t->GetHandle() );
		}
	}
*/
}