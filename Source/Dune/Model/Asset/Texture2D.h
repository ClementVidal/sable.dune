#ifndef _DUNE_ASSET_TEXTURE_
#define _DUNE_ASSET_TEXTURE_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/File/FilePath.h>
#include <Sable/Core/Collection/Array.h>

#include <Dune/Model/Asset/Asset.h>
#include <Dune/Model/Property/Property.h>



namespace Dune
{

/**
\ingroup ModelAsset

2D texture modelisation
*/
class CAssetTexture2D : public CAsset
{

    DEFINE_MANAGED_CLASS( CAssetTexture2D )

public:

    /** @name Constructor/Destructor*/
    //@{
    CAssetTexture2D( );
    ~CAssetTexture2D();
    //@}

    /** @name Accessors*/
    //@{
	CPropertyFilePath& GetFilePath() const;
    //@}

    /** @name Manipulator*/
    //@{
	Void			OnPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue );
    Void            PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& out  ) const;
    Void            BakeObjectSet(CBakeContext& context, CBakeObjectSet& out );
	Void			InitializeObjectSet(CBakeContext& context, CBakeObjectSet& objSet );
    //@}

    /** @name Attributes names*/
    //@{
    static String PropFilePath;
    static String PropSize;
    static String PropMipMapCount;
    //@}

    /** @name Enum*/
    //@{
    //@}

private:

    // Attributes
	CPropertyFilePath*	m_PropFilePath;
	CPropertyVector2i*	m_PropSize;
	CPropertyInt32*	m_PropMipMapCount;

};

};

#endif