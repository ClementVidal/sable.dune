#ifndef _DUNE_ASSET_STATESSAMPLER_
#define _DUNE_ASSET_STATESSAMPLER_

#include <Sable\Core\Common\DataTypes.h>
#include <Sable\Core\File\FilePath.h>
#include <Sable\Core\Collection\Array.h>

#include <Dune\Model\Asset\Asset.h>
#include <Dune\Model\Property\Property.h>



namespace Dune
{

/**
\ingroup ModelAsset

States blend representation
*/
class CAssetStatesSampler : public CAsset
{

    DEFINE_MANAGED_CLASS( CAssetStatesSampler )

public:

    /** @name Constructor/Destructor*/
    //@{
    CAssetStatesSampler();
    ~CAssetStatesSampler();
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
	static String PropMipFilter;
	static String PropMinFilter;
	static String PropMagFilter;
	static String PropAddressU;
	static String PropAddressV;
	static String PropAddressW;
    //@}

    /** @name Enum*/
    //@{
	static CPropertyEnum::SEntry EnumFilterMode[];
	static CPropertyEnum::SEntry EnumAddressMode[];
    //@}

private:

    // Attributes
	CPropertyEnum*	m_MinFilter;
	CPropertyEnum*	m_MipFilter;
	CPropertyEnum*	m_MagFilter;
	CPropertyEnum*	m_AddressU;
	CPropertyEnum*	m_AddressV;
	CPropertyEnum*	m_AddressW;

	// Methods

};

};

#endif