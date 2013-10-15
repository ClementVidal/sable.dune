#include <Dune\Model\Asset\ParticleSystem.h>

#include <Dune\Common\XMLDataLayout.h>
#include <Dune\Model\Project\Project.h>
#include <Sable\Graphics\Particle\Header.h>

using namespace Dune;

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CAssetParticleSystem, CAsset )

String CAssetParticleSystem::PropFilePath = "FilePath";
String CAssetParticleSystem::PropMaterial = "Material";

CAssetParticleSystem::CAssetParticleSystem() :
    CAsset()
{
    m_FilePath = new CPropertyFilePath( PropFilePath );
    m_FilePath->SetWildcard( "Particle file (*.dpart)" );
	AddProperty( *m_FilePath );

	m_Material = new CPropertyHandle( PropMaterial );
	AddProperty( *m_Material );


    SetName("ParticleSystem");
}

CAssetParticleSystem::~CAssetParticleSystem()
{
}

CPropertyFilePath& CAssetParticleSystem::GetFilePath() const
{
	return (CPropertyFilePath&) *GetProperty(PropFilePath);
}

CPropertyHandle& CAssetParticleSystem::GetMaterial() const
{
	return (CPropertyHandle&) *GetProperty(PropMaterial);
}

Bool CAssetParticleSystem::SerializeParticleFile( Sb::CParticleGroup& group, EAccesMode mode ) const
{
	const Sb::CFilePath& path = GetFilePath().GetValue();
	if( path.IsValid() )
	{
        Sb::CXMLDataLayout      dataLayout( path );
		Sb::CPersistentArchive ar( dataLayout, mode );

		if( ar.Serialize( group, WT("ParticleGroup") ) )
		{
			ar.FlushData();

			return TRUE;
		}
	
	}
	return FALSE;
} 

Void CAssetParticleSystem::PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet  ) const
{
	Sb::CParticleSystem* ps = NEWOBJ( Sb::CParticleSystem, ( ) );
	Sb::CParticleGroup* pss = NEWOBJ( Sb::CParticleGroup, ( 256 ) );

	SerializeParticleFile( *pss, nAccesMode_Read );

	ps->SetGroup( *pss );

	objSet.AddObject( *ps );
}

Void CAssetParticleSystem::BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
	Sb::CParticleSystem* ps = objSet.GetObj<Sb::CParticleSystem>( );

	if( ps )
	{
		Sb::CMaterial* psm = NULL;

		CBakeObjectSet* set = context.GetObjectSet( GetMaterial().GetValue() );
		if( set )
		{
			psm = set->GetObj<Sb::CMaterial>();
			if( psm )
			{
				ps->SetMaterial( psm );
				objSet.NeedInit( TRUE );
			}
		}
		
		CAsset::BakeObjectSet( context, objSet );
	}
}

Void CAssetParticleSystem::InitializeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
	Sb::CParticleSystem* ps = objSet.GetObj<Sb::CParticleSystem>( );
	if( ps )
	{
		ps->Initialize();
	}
}