#include <Dune/Model/Asset/Model.h>

#include <Dune/Bake/Context/Header.h>

#include <Sable/Graphics/Geometry/Model.h>
#include <Sable/Physic/Shape/Header.h>
#include <Sable/Physic/Common/Header.h>

#include <NxPhysicsSDK.h>
#include <NxCooking.h>
#include <NxTriangleMeshDesc.h>

using namespace Dune;

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CAssetModel, CFBXDocInterface )

String CAssetModel::PropSubsets = "Subsets";

CAssetModel::CAssetModel() :
    CFBXDocInterface( )
{
	m_Subsets = new CPropertyArray( PropSubsets, CPropertyHandle::GetStaticTypeInfo() );
    AddProperty( *m_Subsets );

    SetName("Model");
}

CAssetModel::~CAssetModel()
{
}

Void CAssetModel::RemoveAllSubset()
{
	m_Subsets->ClearAllItems();
}

Void CAssetModel::AddSubset()
{
	CPropertyHandle* p = (CPropertyHandle*)m_Subsets->AddItemAtEnd( );
    p->AddFilter( CAssetMaterial::GetStaticTypeInfo() );
}

Bool CAssetModel::Serialize( Sb::CPersistentArchive& ar )
{
	return CFBXDocInterface::Serialize( ar );
}

Void CAssetModel::OnSetFBXBinding()
{
	CFBXObject* fbxObj = GetFBXObject();
	if( fbxObj == NULL )
		return;

	FbxNode* fbxNode = (FbxNode*) fbxObj->GetFBXObject();

	if( m_Subsets->GetItemCount() == fbxNode->GetMaterialCount() )
		return;

	for(UInt32 i=0;i<(UInt8) fbxNode->GetMaterialCount();i++)
	{
		AddSubset();
	}
}

Void CAssetModel::PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet  ) const
{
	Sb::CGeometryModel* model = NEWOBJ( Sb::CGeometryModel, () );
	objSet.AddObject( *model );
}

Void CAssetModel::BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
	if( !GetFBXDocFilePath().GetValue().Exist() )
	{
		context.LogError( "FBX File does not exist: %s (Referenced by asset: %s)", GetFBXDocFilePath().GetValue().GetPath(), objSet.GetName() );
	}

	if( context.GetTypeInfo().IsKindOf( CBakeContextExport::GetStaticTypeInfo() ) )
	{
		CBakeContextExport& contextExport = (CBakeContextExport&) context;

		Sb::CGeometryModel* model = objSet.GetObj<Sb::CGeometryModel>( );
		Sb::CPhysicShapeTriangleMesh* physicShape = objSet.GetObj<Sb::CPhysicShapeTriangleMesh>( );
		if( model && GetFBXDoc() )
		{
			CFBXModelConverter converter( contextExport, *GetFBXObject() );
		}
	}

    Sb::CGeometryModel* model = objSet.GetObj<Sb::CGeometryModel>( );
    if( model && GetFBXDoc() )
    {
	/*	domGeometry* domGeom = (domGeometry*)GetColladaElement();
		if( !domGeom )
		{
			context.LogError( "Invalid Collada element id: %s in file: %s", GetFBXId().GetValue().GetBuffer(), GetFBXDoc()->GetPath().GetBuffer() );
		}
		else*/
		{
			CFBXModelConverter converter( context, *GetFBXObject() );
			converter.ConvertVisualGeometry( *model );
			objSet.NeedInit( TRUE );

			UInt32 i;
			for(i=0;i<m_Subsets->GetItemCount();i++)
			{
				CBakeObjectSet* materialObjSet = context.GetObjectSet( ((const CPropertyHandle&)m_Subsets->GetItemAtIndex( i )).GetValue() );

				if( materialObjSet )
				{
					Sb::CMaterial* material = material = materialObjSet->GetObj<Sb::CMaterial>();
					model->GetSubset(i).Material = material;
				}
			}
		}
		
    }
 
    CAsset::BakeObjectSet( context, objSet );
}

Void CAssetModel::InitializeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
    Sb::CGeometryModel* model = objSet.GetObj<Sb::CGeometryModel>( );
    model->Load();
}