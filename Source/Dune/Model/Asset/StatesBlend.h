#ifndef _DUNE_ASSET_STATESBLEND_
#define _DUNE_ASSET_STATESBLEND_

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
class CAssetStatesBlend : public CAsset
{

    DEFINE_MANAGED_CLASS( CAssetStatesBlend )

public:

    /** @name Constructor/Destructor*/
    //@{
    CAssetStatesBlend();
    ~CAssetStatesBlend();
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
	static String PropBlendEnable;
	static String PropSrcColorBlendMode;
	static String PropDstColorBlendMode;
	static String PropSrcAlphaBlendMode;
	static String PropDstAlphaBlendMode;
	static String PropColorBlendOp;
	static String PropAlphaBlendOp;
    //@}

	/** @name Enum*/
	//@{
	static CPropertyEnum::SEntry EnumBlendMode[];
	static CPropertyEnum::SEntry EnumBlendOp[];
	//@}

private:

    // Attributes
	CPropertyBool*	m_BlendEnable;
	CPropertyEnum*	m_SrcColorBlendMode;
	CPropertyEnum*	m_DstColorBlendMode;
	CPropertyEnum*	m_SrcAlphaBlendMode;
	CPropertyEnum*	m_DstAlphaBlendMode;
	CPropertyEnum*	m_AlphaBlendOp;
	CPropertyEnum*	m_ColorBlendOp;

	// Methods
};

};

#endif
