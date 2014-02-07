#ifndef _DUNE_MODEL_ASSET_HELPER_
#define _DUNE_MODEL_ASSET_HELPER_

#include <Sable/Core/Common/DataTypes.h>
#include <Dune/Model/Asset/FBX/Header.h>



namespace Dune
{

class CPropertyVector3f;

/**
\ingroup ModelAsset

Helper asset
*/
class CAssetHelper : public CFBXDocInterface
{

    DEFINE_MANAGED_CLASS( CAssetHelper )

public:

    /** @name Constructor/Destructor*/
    //@{
    CAssetHelper( );
    ~CAssetHelper();
    //@}	

    /** @name Accessors*/
    //@{
	CPropertyVector3f& GetWorldPosition() const;
    //@}

    /** @name Manipulator*/
    //@{
	Bool            Serialize( Sb::CPersistentArchive& ar );
	Void            PrepareBakeObjectSet(  CBakeContext& context, CBakeObjectSet& out  ) const;
	Void            BakeObjectSet( CBakeContext& context, CBakeObjectSet& out );
	Void			OnSetFBXBinding();
    //@}

	/** @name Attributes names*/
	//@{
	static String PropPosition;
	//@}

private:

    // Attributes
	
	// Functions

};

};

#endif