#ifndef _DUNE_MODEL_ASSET_AUDIOBUFFER_
#define _DUNE_MODEL_ASSET_AUDIOBUFFER_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/String/String.h>
#include <Sable/Core/Collection/Array.h>

#include <Dune/Model/Asset/Asset.h>



namespace Dune
{

class CPropertyFilePath;

/**
\ingroup ModelAsset

Font modelisation
*/
class CAssetAudioBuffer : public CAsset
{

    DEFINE_MANAGED_CLASS( CAssetAudioBuffer )

public:

    /** @name Constructor/Destructor*/
    //@{
	CAssetAudioBuffer();
    ~CAssetAudioBuffer();
    //@}

    /** @name Accessors*/
    //@{
	CPropertyFilePath& GetFilePath() const;
    //@}

    /** @name Manipulator*/
    //@{
	Void PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& out  ) const;
	Void BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet );
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