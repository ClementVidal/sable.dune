#include <Dune\Editor\Sable\EditorRenderer.h>

#include <Sable\Graphics\RenderPass\Lighting.h>
#include <Sable\Graphics\RenderPass\Depth.h>
#include <Sable\Graphics\PostProcess\Glow.h>
#include <Sable\Graphics\Debug\RenderPass.h>
#include <Sable\Gui\Common\RenderPass.h>
#include <Sable\Graphics\RenderTarget\Texture.h>
#include <Sable\Graphics\LensFlare\RenderPass.h>

using namespace Dune;

class Sb::CRenderPass;

IMPLEMENT_MANAGED_CLASS1( Dune, CSableEditorRenderer, Sb::CRenderer );

CSableEditorRenderer::CSableEditorRenderer( ) :
    Sb::CRenderer( )
{
}

CSableEditorRenderer::~CSableEditorRenderer()
{

}

Void CSableEditorRenderer::Initialize( const Sb::CRenderTargetInfo& rtInfo )
{
	m_RendererPassDeth = NEWOBJ( Sb::CRenderPassDepth, ( *this ) );
	m_RendererPassDefault = NEWOBJ( Sb::CRenderPassLighting, ( *this ) );
	m_UIRendererPass = NEWOBJ( Sb::CGuiRenderPass, ( *this ) );
    m_DebugRenderer = NEWOBJ( Sb::CDebugRenderPass, ( *this ) );

    Sb::CRenderer::Initialize( );

	PushRenderPass( *m_RendererPassDeth );
	PushRenderPass( *m_RendererPassDefault );
	PushRenderPass( *m_UIRendererPass );
	PushRenderPass( *m_DebugRenderer );

    Sb::CRenderTargetInfo depthInfo;
    depthInfo.TextureInfo = rtInfo.TextureInfo;
    depthInfo.MultiSamplingQuality = nMultiSamplingQuality_None;
    m_RendererPassDeth->Initialize( *this, depthInfo );
    m_RendererPassDefault->Initialize(*this );
    m_UIRendererPass->Initialize(*this );
    m_DebugRenderer->Initialize(*this );	
}

Void CSableEditorRenderer::Render( Sb::CRenderTarget& rt, Sb::CRenderTargetDepthStencil& ds, Sb::CViewport& vp )
{
    m_UIRendererPass->Update();
	{
		DebugGraphicCommandGroup( "EditorRenderer" );
		Sb::CRenderer::Render( rt, ds, vp );
	}

}

