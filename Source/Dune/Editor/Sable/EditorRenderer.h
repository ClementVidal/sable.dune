#ifndef _DUNE_EDITOR_EDITOR_SABLE_RENDERER_
#define _DUNE_EDITOR_EDITOR_SABLE_RENDERER_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/ManagedObject/Ref.h>

#include <Sable/Graphics/Renderer/Header.h>


#define RENDERERPASS_DEPTH 0
#define RENDERERPASS_STATIC_LIGHTING 1

namespace Sable
{
    class CRenderPassDepth;
    class CRenderPassLighting;
    class CPostProcessGlow;
    class CGuiRenderPass;
    class CWorldLensFlareRenderer;
    class CDebugRenderPass;
	class CRenderTargetInfo;
	class CRenderTarget;
	class CRenderTargetDepthStencil;
}

namespace Dune
{

/** 
\ingroup GraphicsRenderer
Represent the Main Renderer of the system.
\n It can be queried to create material using CMainRenderer::CreateMaterial
*/
class CSableEditorRenderer : public Sb::CRenderer
{
	DEFINE_MANAGED_CLASS( CSableEditorRenderer );

public:

	/** @name Operator*/
	//@{
	CSableEditorRenderer( );
	~CSableEditorRenderer();
	//@}

	/** @name Accessors*/
	//@{
	//@}

	/** @name Manipulator*/
	//@{
	Void	Initialize( const Sb::CRenderTargetInfo& rtInfo );
	Void	Render( Sb::CRenderTarget& rt, Sb::CRenderTargetDepthStencil& ds, Sb::CViewport& vp );
	//@}

private:	

	// Types
	
	// Method

	// Attribute
	Sb::CRef<Sb::CRenderPassDepth>           m_RendererPassDeth ;
	Sb::CRef<Sb::CRenderPassLighting>         m_RendererPassDefault ;
	Sb::CRef<Sb::CPostProcessGlow>           m_PostProcessGlow ;
    Sb::CRef<Sb::CGuiRenderPass>              m_UIRendererPass;
    Sb::CRef<Sb::CWorldLensFlareRenderer>    m_LensFlareRenderer;
    Sb::CRef<Sb::CDebugRenderPass>           m_DebugRenderer;

	// Static Attributes

};

}

#endif
