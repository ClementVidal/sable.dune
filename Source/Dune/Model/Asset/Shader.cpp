#include <Dune\Model\Asset\Shader.h>

#include <Dune\Model\Asset\StatesBlend.h>
#include <Dune\Model\Asset\StatesRasterizer.h>
#include <Dune\Model\Asset\StatesDepthStencil.h>
#include <Dune\Model\Asset\StatesSampler.h>


#include <QtCore\QDir>
#include <QtCore\QFile>
#include <QtCore\QString>
#include <QtCore\QProcessEnvironment>
#include <QtCore\QFileInfo>

#include <D3DX9Effect.h>

using namespace Dune;

String CAssetShader::PropRasterizerStates = "RasterizerStates";
String CAssetShader::PropBlendStates = "BlendStates";
String CAssetShader::PropDepthStencilStates = "DepthStencilStates";

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CAssetShader, CAsset )

CAssetShader::CAssetShader() :
    CAsset( )
{
    SetName("Shader");

	m_PixelShaderFilePath = NEWOBJ( CPropertyFilePath, ( "PixelShaderFilePath" ) );
	AddProperty( *m_PixelShaderFilePath );

    m_VertexShaderFilePath = NEWOBJ( CPropertyFilePath, ( "VertexShaderFilePath" ) );
    AddProperty( *m_VertexShaderFilePath );

    m_GeometryShaderFilePath = NEWOBJ( CPropertyFilePath, ( "GeometryShaderFilePath" ) );
    AddProperty( *m_GeometryShaderFilePath );

	m_Rasterizer = NEWOBJ( CPropertyHandle, ( PropRasterizerStates ) );
	AddProperty( *m_Rasterizer );
	m_Rasterizer->AddFilter( CAssetStatesRasterizer::GetStaticTypeInfo() );
	m_Rasterizer->SetGroup( "RenderStates" );

	m_Blend = NEWOBJ( CPropertyHandle, ( PropBlendStates ) );
	AddProperty( *m_Blend );
	m_Blend->AddFilter( CAssetStatesBlend::GetStaticTypeInfo() );
	m_Blend->SetGroup( "RenderStates" );

	m_DepthStencil = NEWOBJ( CPropertyHandle, ( PropDepthStencilStates ) );
	AddProperty( *m_DepthStencil );
	m_DepthStencil->AddFilter( CAssetStatesDepthStencil::GetStaticTypeInfo() );
	m_DepthStencil->SetGroup( "RenderStates" );
}

CAssetShader::~CAssetShader()
{
}

CPropertyFilePath& CAssetShader::GetVertexShaderFilePath()
{
	return *m_VertexShaderFilePath;
}

CPropertyFilePath& CAssetShader::GetPixelShaderFilePath()
{
    return *m_PixelShaderFilePath;
}

CPropertyFilePath& CAssetShader::GetGeometryShaderFilePath()
{
    return *m_GeometryShaderFilePath;
}

Bool CAssetShader::Serialize( Sb::CPersistentArchive& ar )
{
	return CAsset::Serialize( ar );
}

Void CAssetShader::PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet  ) const
{
	Sb::CShader* shader = NEWOBJ( Sb::CShader, ( ) );
	objSet.AddObject( *shader );
}

Sb::CShaderProgram* CAssetShader::CompileProgram( CBakeContext& context, const Sb::CFilePath& path, EShaderProgramType type )
{
    Sb::CShaderProgram* program = NULL;

    if( path.Exist() )
    {
        Sb::CShaderCompiler compiler;
        if( !compiler.Compile( type, path ) )
        {
			if( type == nShaderProgramType_Vertex )
			{
				context.LogError( "Failed to compile vertex shader: %s (%s)", GetName(), path.GetBuffer() );
			}
			else if( type == nShaderProgramType_Pixel )
			{
				context.LogError( "Failed to compile pixel shader: %s (%s)", GetName(), path.GetBuffer() );
			}
			else if( type == nShaderProgramType_Geometry )
			{
				context.LogError( "Failed to compile geometry shader: %s (%s)", GetName(), path.GetBuffer() );
			}

			context.LogError( "Error: %s", compiler.GetErrorString() );

            return NULL;
        }
        program = NEWOBJ( Sb::CShaderProgram, () );
        // Set raw data
        Sb::CBuffer byteCodeBuffer;
        byteCodeBuffer.Open( compiler.GetByteCode(), nAccesMode_Read );
        program->SetType( type );
        program->SetRawData( byteCodeBuffer );
    }
	else if( path.IsValid() )
	{
		context.LogError( "Shader file does not exist: %s (%s)", GetName(), path.GetBuffer() );

		return NULL;
	}


    return program;
}

Void CAssetShader::BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
	Sb::CShader* shader = objSet.GetObj<Sb::CShader>( );

	if( shader )
	{
		CBakeContextExport& contextExport = (CBakeContextExport&) context;

        Sb::CShaderProgram* vertexProgram  = NULL;
		Sb::CShaderProgram* pixelProgram  = NULL;
		Sb::CShaderProgram* geometryProgram = NULL;

		if( !GetVertexShaderFilePath().GetValue().IsValid() )
			Sb::DebugLogWarning( "Invalid vertex shader file path for (See shader: %s)", GetName());
		else if( !GetVertexShaderFilePath().GetValue().Exist() )
			Sb::DebugLogWarning( "Vertex shader file path does not point to an existing file. (See shader: %s)", GetName());
		else
			vertexProgram = CompileProgram( context, GetVertexShaderFilePath().GetValue(), nShaderProgramType_Vertex );

		if( !GetPixelShaderFilePath().GetValue().IsValid() )
			Sb::DebugLogWarning( "Invalid pixel shader file path for (See shader: %s)", GetName());
		else if( !GetPixelShaderFilePath().GetValue().Exist() )
			Sb::DebugLogWarning( "Pixel shader file path does not point to an existing file. (See shader: %s)", GetName());
		else
			pixelProgram = CompileProgram( context, GetPixelShaderFilePath().GetValue(), nShaderProgramType_Pixel );

		if( GetGeometryShaderFilePath().GetValue().IsValid() )
		{
			if( !GetGeometryShaderFilePath().GetValue().Exist() )
				Sb::DebugLogWarning( "Geometry geometry file path does not point to an existing file. (See shader: %s)", GetName());
			else
				geometryProgram = CompileProgram( context, GetGeometryShaderFilePath().GetValue(), nShaderProgramType_Geometry );
		}

        shader->SetPixelShaderProgram( pixelProgram );
        shader->SetVertexShaderProgram( vertexProgram );
        shader->SetGeometryShaderProgram( geometryProgram );

		// Assign blend states if any
		const CBakeObjectSet* set = NULL;
        set = context.GetObjectSet( m_Blend->GetValue() );
        shader->SetBlendStates( set?set->GetObj<Sb::CStatesBlend>( 0 ):NULL );

		// Assign depth stencil states if any
	    set = context.GetObjectSet( m_DepthStencil->GetValue() );
	    shader->SetDepthStencilStates( set?set->GetObj<Sb::CStatesDepthStencil>( 0 ):NULL );

		// Assign rasterizer states if any
		set = context.GetObjectSet( m_Rasterizer->GetValue() );
		shader->SetRasterizerStates( set?set->GetObj<Sb::CStatesRasterizer>( 0 ):NULL );

		objSet.NeedInit( TRUE );
		CAsset::BakeObjectSet( context, objSet );
	}
}


Void CAssetShader::InitializeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
	Sb::CShader* sh = objSet.GetObj<Sb::CShader>( );
	if( sh )
	{
		sh->Load();
	}
}
