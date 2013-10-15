#include <Dune\Model\Asset\Font.h>

#include <Dune\Model\Project\Project.h>
#include <Dune\Bake\Context\Export.h>

#include <Sable\Graphics\Text\Header.h>

#include <QtCore\QDir>
#include <QtCore\QFile>
#include <QtCore\QString>

using namespace Dune;
 

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CAssetFont, CAsset )

String CAssetFont::PropFilePath = "FilePath";

CAssetFont::CAssetFont( ) :
    CAsset()
{
    m_FilePath = new CPropertyFilePath( PropFilePath );
    m_FilePath->SetWildcard( "Font file (*.dfnt)" );

    AddProperty( *m_FilePath );

    SetName("Font");
}

CAssetFont::~CAssetFont()
{
}

CPropertyFilePath& CAssetFont::GetFilePath() const
{
	return (CPropertyFilePath&) *GetProperty(PropFilePath);
}

Void CAssetFont::PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& objSet  ) const
{
	Sb::CTextFont* font = NEWOBJ( Sb::CTextFont, ( ) );
	objSet.AddObject( *font );
}

Void CAssetFont::BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
	Sb::CTextFont* font = objSet.GetObj<Sb::CTextFont>( );

	if( font )
	{
        if( !GetFilePath().GetValue().Exist() )
        {
            context.LogError( "Font file \"%s\" does not exist\n", GetFilePath().GetValueAsString().GetBuffer() );
            return;
        }

		font->SetRawDataFromFile( m_FilePath->GetValue() );

		objSet.NeedInit( TRUE );
		CAsset::BakeObjectSet( context, objSet );
	}
}

Void CAssetFont::InitializeObjectSet(CBakeContext& context, CBakeObjectSet& objSet )
{
	Sb::CTextFont* font = objSet.GetObj<Sb::CTextFont>( );
	if( font )
	{
		font->Load();
	}
}