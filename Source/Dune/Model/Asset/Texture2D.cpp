#include <Dune\Model\Asset\Texture2D.h>

#include <Dune\Model\Project\Project.h>

#include <Sable\Graphics\Texture\Texture2D.h>
#include <Sable\Graphics\Common\DataTypes.h>
#include <Sable\Media\Tga\Header.h>
#include <Sable\Media\DDS\Header.h>

#include <QtCore\QFile>
#include <QtCore\QString>

using namespace Dune;

String CAssetTexture2D::PropFilePath = "FilePath";
String CAssetTexture2D::PropSize = "Size";
String CAssetTexture2D::PropMipMapCount = "MipMapCount";

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CAssetTexture2D, CAsset )


CAssetTexture2D::CAssetTexture2D( ) :
    CAsset()
{
    m_PropFilePath = new CPropertyFilePath( PropFilePath );
    m_PropFilePath->SetWildcard( "DDS file (*.dds);;TGA file (*.tga)" );
    AddProperty( *m_PropFilePath );

    m_PropSize = new CPropertyVector2i( PropSize );
	m_PropSize->SetGroup( "Info" );
    m_PropSize->SetIsEnabled( FALSE );
    AddProperty( *m_PropSize );

    m_PropMipMapCount = new CPropertyInt32( PropMipMapCount );
	m_PropMipMapCount->SetGroup( "Info" );
    m_PropMipMapCount->SetIsEnabled( FALSE );
    AddProperty( *m_PropMipMapCount );

    SetName("Texture");
}

CAssetTexture2D::~CAssetTexture2D()
{
}

CPropertyFilePath& CAssetTexture2D::GetFilePath() const
{
	return (CPropertyFilePath&) *GetProperty(PropFilePath);
}

Void CAssetTexture2D::PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet  ) const
{
    Sb::CTexture2D* texture = NEWOBJ( Sb::CTexture2D, () );
    objSet.AddObject( *texture );
}

Void CAssetTexture2D::BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
    Sb::CTexture2D* texture = objSet.GetObj<Sb::CTexture2D>( );
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

Void CAssetTexture2D::InitializeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
	Sb::CTexture2D* texture = objSet.GetObj<Sb::CTexture2D>( );
	if( texture )
	{
		texture->Load();
	}
}

Void CAssetTexture2D::OnPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue )
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
			m_PropSize->SetValue( Sb::CVector2i( ti.Width, ti.Height ) );
			m_PropMipMapCount->SetValue( ti.MipMapCount );
		}
		else
		{
			m_PropSize->SetValue( Sb::CVector2i::Zero );
			m_PropMipMapCount->SetValue( 0 );
		}
	}

	CAsset::OnPropertyChanged( p, oldValue );
}