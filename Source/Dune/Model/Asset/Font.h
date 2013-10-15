#ifndef _DUNE_MODEL_ASSET_FONT_
#define _DUNE_MODEL_ASSET_FONT_

#include <Sable\Core\Common\DataTypes.h>
#include <Sable\Core\String\String.h>
#include <Sable\Core\Collection\Array.h>

#include <Dune\Model\Asset\Asset.h>

namespace Dune
{

class CPropertyFilePath;

/**
\ingroup ModelAsset

Font modelisation
*/
class CAssetFont : public CAsset
{

    DEFINE_MANAGED_CLASS( CAssetFont )

public:

    /** @name Constructor/Destructor*/
    //@{
    CAssetFont();
    ~CAssetFont();
    //@}

    /** @name Accessors*/
    //@{
	CPropertyFilePath& GetFilePath() const;
    //@}

    /** @name Manipulator*/
    //@{
	Void PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& out  ) const;
	Void BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet );
	Void InitializeObjectSet(CBakeContext& context, CBakeObjectSet& objSet );
    //@}

    /** @name Attributes names*/
    //@{
    static String PropFilePath;
    //@}

private:

    // Attributes
	CPropertyFilePath* m_FilePath;

};

};

#endif