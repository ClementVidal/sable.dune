#ifndef _DUNE_MODEL_ASSET_MATERIAL_
#define _DUNE_MODEL_ASSET_MATERIAL_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Graphics/Material/Header.h>
#include <Dune/Model/Asset/FBX/Header.h>



namespace Dune
{

class CPropertyHandle;
class CPropertyBool;

/**
\ingroup ModelAsset

Material asset
*/
class CAssetMaterial : public CFBXDocInterface
{

    DEFINE_MANAGED_CLASS( CAssetMaterial )

public:

    /** @name Constructor/Destructor*/
    //@{
    CAssetMaterial();
    ~CAssetMaterial();
    //@}	

    /** @name Accessors*/
    //@{
    CPropertyHandle& GetShader();
	//@}

    /** @name Manipulator*/
    //@{
	Bool            Serialize( Sb::CPersistentArchive& ar );
	Void            PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& out  ) const;
	Void            BakeObjectSet( CBakeContext& context, CBakeObjectSet& out ) ;
	Void			InitializeObjectSet(CBakeContext& context, CBakeObjectSet& objSet );
	Void			OnSetFBXBinding();
    //@}

	/** @name Attributes names*/
	//@{
	
	//@}

private:

    // Attributes
	CPropertyHandle*	m_Shader;

	// Functions

};

};

#endif