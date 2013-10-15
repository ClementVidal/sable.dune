#ifndef _DUNE_MODEL_ASSET_PARTICLESYSTEM_
#define _DUNE_MODEL_ASSET_PARTICLESYSTEM_

#include <Sable\Core\Common\DataTypes.h>
#include <Sable\Core\String\String.h>
#include <Sable\Core\Collection\Array.h>
#include <Sable\Graphics\Particle\System.h>

#include <Dune\Model\Asset\Asset.h>

namespace Dune
{

class CPropertyFilePath;

/**
\ingroup ModelAsset

Particle system asset
*/
class CAssetParticleSystem : public CAsset
{

    DEFINE_MANAGED_CLASS( CAssetParticleSystem )

public:

    /** @name Constructor/Destructor*/
    //@{
    CAssetParticleSystem( );
    ~CAssetParticleSystem();
    //@}

    /** @name Accessors*/
    //@{
	CPropertyFilePath&	GetFilePath() const;
	CPropertyHandle&	GetMaterial() const;
	//@}

    /** @name Manipulator*/
    //@{
	Bool SerializeParticleFile( Sb::CParticleGroup& group, EAccesMode mode ) const;

	Void PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& out ) const;
	Void BakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet );
	Void InitializeObjectSet( CBakeContext& context, CBakeObjectSet& objSet );
    //@}

    /** @name Attributes names*/
    //@{
    static String PropFilePath;
    static String PropMaterial;
    //@}

private:

    // Attributes
	CPropertyFilePath*	m_FilePath;
	CPropertyHandle*	m_Material;

	// Methods
};

};

#endif