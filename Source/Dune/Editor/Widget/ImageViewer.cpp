#include <Dune/Editor/Widget/ImageViewer.h> 

#include <Sable/Media/Dds/DDSFile.h>
#include <Sable/Media/Tga/TGAFile.h>

#include <QtGui/QPixmap>

using namespace Dune;

CImageViewer::CImageViewer( CApplication& app, CAssetTexture2D& texture ) :
    QDockWidget( "Image Viewer", &app.GetMainFrame() )
{
	setAttribute(Qt::WA_DeleteOnClose);

    m_Texture2D = &texture;
	UInt32 border = 4;
    m_App = &app;

	m_Frame = new QWidget( this );

    m_ImageLabel = new QLabel;
    m_ImageLabel->setBackgroundRole(QPalette::Base);
    m_ImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    m_ImageLabel->setScaledContents(true);

    m_ScrollArea = new QScrollArea;
	m_ScrollArea->setAlignment( Qt::AlignCenter );
    m_ScrollArea->setBackgroundRole(QPalette::Dark);
    m_ScrollArea->setWidget(m_ImageLabel);
 	m_ScrollArea->setFrameStyle( QFrame::Sunken | QFrame::WinPanel );

	setWidget( m_ScrollArea );
	resize( QSize( 800, 600 ) );

    Load();
}

CImageViewer::~CImageViewer()
{
    if( m_Texture2D )
    {
        if( m_Texture2D->GetIsEdited() )
            m_Texture2D->SetIsEdited( FALSE );
    }
}

Void CImageViewer::Load( Sb::CImageFile& image )
{
    UInt32 width = image.GetTextureInfo().Width;
    UInt32 height = image.GetTextureInfo().Height;

	UInt32 byteCount = 0;
	const Byte* srcBuffer = (const Byte*)image.GetSurfaceData( 0,0, byteCount );
	Sb::CArray<Byte> tmpBufferArray;
	tmpBufferArray.SetItemCount( width * height * 4 );
	Byte* dstBuffer = tmpBufferArray.GetBuffer();

	if( image.GetTextureInfo().Format == nTextureFormat_R8G8B8A8 )
	{
		for( UInt32 i=0;i<width*height; i++ )
		{
			// Convert from RGBA to BGRA
			dstBuffer[i*4+0] = srcBuffer[i*4+2];
			dstBuffer[i*4+1] = srcBuffer[i*4+1];
			dstBuffer[i*4+2] = srcBuffer[i*4+0];
			dstBuffer[i*4+3] = srcBuffer[i*4+3];
		}
	}
	else if( image.GetTextureInfo().Format == nTextureFormat_A8 )
	{
		for( UInt32 i=0;i<width*height; i++ )
		{
			dstBuffer[i*4+0] = srcBuffer[i*4];
			dstBuffer[i*4+1] = srcBuffer[i*4];
			dstBuffer[i*4+2] = srcBuffer[i*4];
			dstBuffer[i*4+3] = srcBuffer[i*4];
		}
	}
	else if( image.GetTextureInfo().Format == nTextureFormat_R8G8B8 )
	{
		for( UInt32 i=0;i<width*height; i++ )
		{
			dstBuffer[i*4+0] = srcBuffer[i*0];
			dstBuffer[i*4+1] = srcBuffer[i*1];
			dstBuffer[i*4+2] = srcBuffer[i*2];
			dstBuffer[i*4+3] = 255;
		}
	}

	QImage qimage( dstBuffer, width, height, QImage::Format_ARGB32 );
	m_ImageLabel->setPixmap(QPixmap::fromImage( qimage ));
    m_ImageLabel->resize( m_ImageLabel->pixmap()->size() );
}

Void CImageViewer::Load()
{
    if( m_Texture2D == NULL )
        return;

    Sb::CFilePath filePath = m_Texture2D->GetFilePath().GetValue();
 
    // Detext format
    Sb::CImageFile* imageFile = NULL;
    Sb::CDDSFile ddsFile;
    Sb::CTGAFile tgaFile;

    Char ext[6];

    filePath.GetExtension( ext, 6 );
    Sb::StringToLower( ext, ext, 6 );

    if( Sb::StringCompare( ext, "tga" ) == 0 )
    {
        imageFile = &tgaFile;
    }
    else if( Sb::StringCompare( ext, "dds" ) == 0 )
    {
        imageFile = &ddsFile;
    }
  
    if( imageFile && imageFile->Load( filePath, TRUE ) )
    {
        Load( *imageFile );
    }

}