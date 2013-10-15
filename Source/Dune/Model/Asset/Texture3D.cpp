#include <Dune\Model\Asset\Texture3D.h>

#include <Dune\Model\Project\Project.h>
#include <Sable\Graphics\Texture\Texture3D.h>
#include <Sable\Graphics\Common\DataTypes.h>
#include <Sable\Media\Dds\DDSFile.h>
#include <Sable\Media\Tga\TGAFile.h>

#include <QtCore\QFile>
#include <QtCore\QString>

using namespace Dune;

String CAssetTexture3D::PropFilePath = "FilePath";
String CAssetTexture3D::PropSize = "Size";
String CAssetTexture3D::PropDepth = "Depth";
String CAssetTexture3D::PropMipMapCount = "MipMapCount";

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CAssetTexture3D, CAsset )


CAssetTexture3D::CAssetTexture3D( ) :
    CAsset()
{
    m_PropFilePath = new CPropertyFilePath( PropFilePath );
    m_PropFilePath->SetWildcard( "DDS file (*.dds)" );
    AddProperty( *m_PropFilePath );

    m_PropSize = new CPropertyVector2f( PropSize );
    m_PropSize->SetIsEnabled( FALSE );
	m_PropSize->SetGroup( "Info" );
    AddProperty( *m_PropSize );

    m_PropMipMapCount = new CPropertyInt32( PropMipMapCount );
    m_PropMipMapCount->SetIsEnabled( FALSE );
	m_PropMipMapCount->SetGroup( "Info" );
	AddProperty( *m_PropMipMapCount );

	m_PropDepth = new CPropertyInt32( PropDepth );
	m_PropDepth->SetIsEnabled( FALSE );
	m_PropDepth->SetGroup( "Info" );
	AddProperty( *m_PropDepth );

    SetName("Texture3D");
}

CAssetTexture3D::~CAssetTexture3D()
{
}

CPropertyFilePath& CAssetTexture3D::GetFilePath() const
{
	return (CPropertyFilePath&) *GetProperty(PropFilePath);
}

Void CAssetTexture3D::PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet  ) const
{
    Sb::CTexture3D* texture = NEWOBJ( Sb::CTexture3D, () );
    objSet.AddObject( *texture );
}

Void CAssetTexture3D::BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
    Sb::CTexture3D* texture = objSet.GetObj<Sb::CTexture3D>( );
    if( texture )
    {
		if( !GetFilePath().GetValue().Exist() )
		{
			context.LogError( "Texture file \"%s\" does not exist\n", GetFilePath().GetValueAsString().GetBuffer() );
			return;
		}

		// Set raw data
		texture->SetRawDataFromFile( GetFilePath().GetValue() );

		objSet.NeedInit( TRUE );

        CAsset::BakeObjectSet( context, objSet );
    }
}

Void CAssetTexture3D::InitializeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
	Sb::CTexture3D* texture = objSet.GetObj<Sb::CTexture3D>( );
	if( texture )
	{
		texture->Load();
	}
}

Void CAssetTexture3D::OnPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue )
{
	if( &p == m_PropFilePath )
	{
		const Sb::CFilePath& n = CastProperty( p, CPropertyFilePath )->GetValue();
		if( n.Exist() )
		{
			Sb::CDDSFile ddsFile;
			Sb::CTGAFile tgaFile;
			Char ext[6];
			Sb::CImageFile* imageFile = NULL;

			n.GetExtension( ext, 6 );
			Sb::StringToLower( ext, ext, 6 );

			if( Sb::StringCompare( ext, "tga" ) == 0 )
			{
				imageFile = &tgaFile;
			}
			else if( Sb::StringCompare( ext, "dds" ) == 0 )
			{
				imageFile = &ddsFile;
			}
			else
			{
				return;
			}

			if( imageFile->Load( n ) == FALSE )
				return;

			const Sb::CTextureInfo& ti = imageFile->GetTextureInfo();
			m_PropSize->SetValue( Sb::CVector2f( ti.Width, ti.Height ) );
			m_PropMipMapCount->SetValue( ti.MipMapCount );
			m_PropDepth->SetValue( ti.SliceCount );

		}
	}
	CAsset::OnPropertyChanged( p, oldValue );
}