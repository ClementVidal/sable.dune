#ifndef _DUNE_MODEL_ASSET_FBX_MATERIALCONVERTER_
#define _DUNE_MODEL_ASSET_FBX_MATERIALCONVERTER_

#include <Sable/Core/Common/DataTypes.h>

#include <Sable/Core/String/StaticString.h>

namespace Dune
{

/**
\ingroup ModelAsset

Helper class used to process a collada material
*/
class CFBXMaterialConverter 
{

public:

	/** @name Constructor/Destructor*/
	//@{
	struct STextureInfo
	{
		STextureInfo() { IsValid = FALSE; }
		Sb::CFilePath			Path;
		Bool					IsValid;
		Sb::CStringIdentifier	Name;
	};
	//@}

    /** @name Constructor/Destructor*/
    //@{
    CFBXMaterialConverter( /*domMaterial& daeMaterial*/ );
    ~CFBXMaterialConverter();
    //@}	

    /** @name Accessors*/
    //@{
	String				GetName() const;
	const STextureInfo&	GetDiffuseTexture() const;
	//@}

    /** @name Manipulator*/
    //@{
	//@}

private:

    // Attributes
	STextureInfo			m_DiffuseTexture;
	STextureInfo			m_SpecularTexture;


	// Functions
	Void Parse();

};

};

#endif