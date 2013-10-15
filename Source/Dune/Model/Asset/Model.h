#ifndef _DUNE_MODEL_ASSET_MODEL_
#define _DUNE_MODEL_ASSET_MODEL_

#include <Sable\Core\Common\DataTypes.h>
#include <Sable\Graphics\Geometry\Header.h>
#include <Dune\Model\Asset\FBX\Header.h>

namespace Dune
{

class CPropertyBool;

/**
\ingroup ModelAsset

Model asset
*/
class CAssetModel : public CFBXDocInterface
{

    DEFINE_MANAGED_CLASS( CAssetModel )

public:

    /** @name Constructor/Destructor*/
    //@{
    CAssetModel();
    ~CAssetModel();
    //@}	

    /** @name Accessors*/
    //@{
    //@}

    /** @name Manipulator*/
	//@{
	Void			RemoveAllSubset();
	Void			AddSubset();
	Bool            Serialize( Sb::CPersistentArchive& ar );
	Void            PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& out  ) const;
	Void            BakeObjectSet(CBakeContext& context, CBakeObjectSet& out ) ;
    Void            InitializeObjectSet(CBakeContext& context, CBakeObjectSet& objSet );
    //@}

	/** @name Attributes names*/
	//@{
	static String PropSubsets;
	//@}

private:

    // Attributes
	CPropertyArray*	m_Subsets;

	// Functions
	Void	OnSetFBXBinding();

};

};

#endif