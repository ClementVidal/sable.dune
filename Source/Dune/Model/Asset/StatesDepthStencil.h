#ifndef _DUNE_ASSET_STATESDEPTHSTENCIL_
#define _DUNE_ASSET_STATESDEPTHSTENCIL_

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
class CAssetStatesDepthStencil : public CAsset
{

    DEFINE_MANAGED_CLASS( CAssetStatesDepthStencil )

public:

    /** @name Constructor/Destructor*/
    //@{
    CAssetStatesDepthStencil( );
    ~CAssetStatesDepthStencil();
    //@}

    /** @name Accessors*/
    //@{
    //@}

    /** @name Manipulator*/
    //@{
    Void            PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& out  ) const;
    Void            BakeObjectSet(CBakeContext& context, CBakeObjectSet& out );
    //@}

    /** @name Attributes names*/
    //@{
	static String PropTestEnable;
	static String PropWriteEnable;
	static String PropDepthFunc;
    //@}

    /** @name Enum*/
    //@{
	static CPropertyEnum::SEntry EnumDepthFunc[];
    //@}

private:

    // Attributes
	CPropertyBool*	m_TestEnable;
	CPropertyBool*	m_WriteEnable;
	CPropertyEnum*	m_DepthFunc;
	
	// Methods

};

};

#endif