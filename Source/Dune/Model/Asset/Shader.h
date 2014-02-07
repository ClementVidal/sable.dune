#ifndef _DUNE_MODEL_ASSET_SHADER_
#define _DUNE_MODEL_ASSET_SHADER_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Graphics/Material/Header.h>
#include <Dune/Model/Asset/FBX/Header.h>



namespace Dune
{

class CPropertyBool;

/**
\ingroup ModelAsset

Shader asset
*/
class CAssetShader : public CAsset
{

    DEFINE_MANAGED_CLASS( CAssetShader )

public:

    /** @name Constructor/Destructor*/
    //@{
    CAssetShader();
    ~CAssetShader();
    //@}	

    /** @name Accessors*/
    //@{
	CPropertyFilePath& GetPixelShaderFilePath();
	CPropertyFilePath& GetVertexShaderFilePath();
	CPropertyFilePath& GetGeometryShaderFilePath();
	CPropertyHandle& GetStatesBlock();
    //@}

    /** @name Manipulator*/
    //@{
	Bool            Serialize( Sb::CPersistentArchive& ar );
	Void            PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& out  ) const;
	Void            BakeObjectSet( CBakeContext& context, CBakeObjectSet& out ) ;
	Void			InitializeObjectSet( CBakeContext& context, CBakeObjectSet& objSet );
	//@}

	/** @name Attributes names*/
	//@{
	static String PropRasterizerStates;
	static String PropBlendStates;
	static String PropDepthStencilStates;
	//@}

private:

    // Attributes
	CPropertyFilePath*	m_PixelShaderFilePath;
	CPropertyFilePath*	m_VertexShaderFilePath;
	CPropertyFilePath*	m_GeometryShaderFilePath;
	CPropertyHandle*	m_DepthStencil;
	CPropertyHandle*	m_Rasterizer;
	CPropertyHandle*	m_Blend;

	// Functions
    Sb::CShaderProgram*     CompileProgram( CBakeContext& context, const Sb::CFilePath& path, EShaderProgramType type );

};

};

#endif