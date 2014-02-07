#ifndef _DUNE_ASSET_STATESBLOCK_
#define _DUNE_ASSET_STATESBLOCK_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/File/FilePath.h>
#include <Sable/Core/Collection/Array.h>

#include <Dune/Model/Asset/Asset.h>
#include <Dune/Model/Property/Property.h>



namespace Dune
{

/**
\ingroup ModelAsset

States block representation
*/
class CAssetStatesBlock : public CAsset
{

    DEFINE_MANAGED_CLASS( CAssetStatesBlock )

public:

    /** @name Constructor/Destructor*/
    //@{
    CAssetStatesBlock();
    ~CAssetStatesBlock();
    //@}

    /** @name Accessors*/
    //@{
    //@}

    /** @name Manipulator*/
    //@{
    Void            PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& out  ) const;
    Void            BakeObjectSet(CBakeContext& context, CBakeObjectSet& out );
	Void			InitializeObjectSet(CBakeContext& context, CBakeObjectSet& objSet );
    //@}

    /** @name Attributes names*/
    //@{
    static String PropSamplerStates;
    static String PropRasterizerStates;
    static String PropBlendStates;
    static String PropDepthStencilStates;
    //@}

    /** @name Enum*/
    //@{
    //@}

private:

    // Attributes
	CPropertyHandle*	m_DepthStencil;
	CPropertyHandle*	m_Sampler;
	CPropertyHandle*	m_Rasterizer;
	CPropertyHandle*	m_Blend;

};

};

#endif