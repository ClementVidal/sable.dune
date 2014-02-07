#ifndef _DUNE_ASSET_STATESRASTERIZER_
#define _DUNE_ASSET_STATESRASTERIZER_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/File/FilePath.h>
#include <Sable/Core/Collection/Array.h>

#include <Dune/Model/Asset/Asset.h>
#include <Dune/Model/Property/Property.h>



namespace Dune
{

/**
\ingroup ModelAsset

States blend representation
*/
class CAssetStatesRasterizer : public CAsset
{

    DEFINE_MANAGED_CLASS( CAssetStatesRasterizer )

public:

    /** @name Constructor/Destructor*/
    //@{
    CAssetStatesRasterizer();
    ~CAssetStatesRasterizer();
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
	static String PropCullMode;
	static String PropFillMode;
    //@}

    /** @name Enum*/
    //@{
	static CPropertyEnum::SEntry EnumCullMode[];
	static CPropertyEnum::SEntry EnumFillMode[];
    //@}

private:

    // Attributes
	CPropertyEnum*	m_CullMode;
	CPropertyEnum*	m_FillMode;

	// Methods

};

};

#endif