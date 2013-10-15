#include <Dune\Editor\Widget\FontEditor.h> 

#include <Dune\Resources\Icons\FontEditor.xpm>

#include <QtGui\QLabel>
#include <QtGui\QGroupBox>
#include <QtGui\QPainter>

using namespace Dune;

CFontEditor::SUnicodeScript CFontEditor::m_UnicodeScriptTable[] = 
{ 
	#include <Dune\Editor\Widget\UnicodeScriptTable.inc>
};

UInt32 CFontEditor::SUnicodeScript::GetCodePointCount() const
{
	return HighCodePoint - LowCodePoint;
}

CFontEditor::CFontEditor( CApplication& app, CAssetFont& asset ) :
    QDockWidget( "FontEditor", &app.GetMainFrame()  ),
	m_IconTable( (const char**) FontEditor_xpm )
{
	UInt32 border = 4;
    m_App = &app;
	m_CurrentUnicodeScript = &m_UnicodeScriptTable[0];

	QFrame*	mainFrame = new QFrame( this );
	setAttribute(Qt::WA_DeleteOnClose);	

	// Create toolbar
	QToolBar*	toolBar = new QToolBar( this );
	
	m_ScrollArea = new QScrollArea( mainFrame );
	m_ImageContainer = new QLabel( mainFrame );

	m_ImageContainer->setBackgroundRole(QPalette::Base);
	m_ImageContainer->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	m_ImageContainer->setScaledContents(true);

	m_ScrollArea->setWidget( m_ImageContainer );
	m_ScrollArea->setAlignment( Qt::AlignCenter );
	m_ScrollArea->setBackgroundRole(QPalette::Dark);
	m_ScrollArea->setFrameStyle( QFrame::Sunken | QFrame::WinPanel );
	
	// Fill resolution combo
	m_BitmapSize = new QComboBox( mainFrame );
	m_BitmapSize->addItem("128");
	m_BitmapSize->addItem("256");
	m_BitmapSize->addItem("512");
	m_BitmapSize->addItem("1024");
	m_BitmapSize->addItem("2048");
	connect( m_BitmapSize, SIGNAL( 	currentIndexChanged( const QString& ) ), this, SLOT( OnUIResolutionChanged( const QString& ) ) );

	// Fill script table
	UInt32 i = 0;
	m_ScriptCombo = new QComboBox( mainFrame );
	connect( m_ScriptCombo, SIGNAL( currentIndexChanged( int ) ), this, SLOT( OnUIScriptChanged( int ) ) );
	m_ScriptCombo->setInsertPolicy( QComboBox::InsertAlphabetically );
	for( i=0;i<UNICODE_SCRIPT_COUNT;i++ )
	{
		m_ScriptCombo->addItem( QString::fromWCharArray(  m_UnicodeScriptTable[i].Name ), QVariant( i ) );
	}

	// Font combo box
	m_FontName = new QFontComboBox( mainFrame );
	connect( m_FontName, SIGNAL( currentFontChanged ( const QFont& ) ), this, SLOT( OnUIFontChanged( const QFont& ) ) );
	
	// Fill Toolbar
	QAction* action;
	action = toolBar->addAction( m_IconTable.GetIcon( 0 ), "Open" );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnUIOpen() ) );
	action = toolBar->addAction( m_IconTable.GetIcon( 1 ), "Save" );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnUISave() ) );
	toolBar->addSeparator();

	// Fill panel 
	m_FontSize = new QSpinBox( mainFrame );
	connect( m_FontSize, SIGNAL( valueChanged( int ) ), this, SLOT( OnUISpinChange( int ) ) );

	// Font Sizer
	QHBoxLayout*	unicodeScriptLayout = new QHBoxLayout( );
	QHBoxLayout*	nameLayout = new QHBoxLayout( );
	QHBoxLayout*	fontSizeLayout = new QHBoxLayout( );
	nameLayout->addWidget( new QLabel( "Name:") );
	nameLayout->addWidget( m_FontName );

	unicodeScriptLayout->addWidget( new QLabel( "Script:") );
	unicodeScriptLayout->addWidget( m_ScriptCombo );

	fontSizeLayout->addWidget( new QLabel("Font pixel size:") );
	fontSizeLayout->addWidget( m_FontSize );

	QVBoxLayout*	fontSettingsLayout = new QVBoxLayout( );
	fontSettingsLayout->addLayout( unicodeScriptLayout );
	fontSettingsLayout->addLayout( nameLayout );
	fontSettingsLayout->addLayout( fontSizeLayout );

	QGroupBox*		fontSettingsGroupBox = new QGroupBox( );
	fontSettingsGroupBox->setLayout( fontSettingsLayout );
	fontSettingsGroupBox->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
	fontSettingsGroupBox->setTitle( "Font settings" );

	// Bitmap Sizer
	QHBoxLayout*	bitmapSizeLayout = new QHBoxLayout( );
	bitmapSizeLayout->addWidget( new QLabel( "Bitmap Size:") );
	bitmapSizeLayout->addWidget( m_BitmapSize );
	
	QGroupBox*		bitmapSettingsGroupBox = new QGroupBox( );
	QVBoxLayout*	bitmapSettingsLayout = new QVBoxLayout( );
	bitmapSettingsLayout->addLayout( bitmapSizeLayout );
	bitmapSettingsGroupBox->setLayout( bitmapSettingsLayout );
	bitmapSettingsGroupBox->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed );
	bitmapSettingsGroupBox->setTitle( "Bitmap settings" );

	QVBoxLayout*	optionsLayout= new QVBoxLayout( );
	optionsLayout->addWidget( bitmapSettingsGroupBox, 0, Qt::AlignTop );
	optionsLayout->addWidget( fontSettingsGroupBox, 0, Qt::AlignTop );
	optionsLayout->addSpacerItem( new QSpacerItem( 10, 10, QSizePolicy::Expanding, QSizePolicy::Expanding ));
	
	QHBoxLayout*	subLayout = new QHBoxLayout( );
	subLayout->addLayout( optionsLayout, 1 );
	subLayout->addWidget(m_ScrollArea, 2 );

	QVBoxLayout*	mainLayout = new QVBoxLayout( );
	mainLayout->setContentsMargins( 0, 0, 0, 0 );
	mainLayout->addWidget( toolBar );
	mainLayout->addLayout( subLayout );

	mainFrame->setLayout( mainLayout );

	setWidget( mainFrame );      // use the sizer for layout

	m_UVOffsetTable.SetItemCount(256);
	
	m_IsDocumentModified = FALSE;

	resize( QSize( 1024, 600 ) );
	SetupControls();
	
	SetFont( asset );
}


CFontEditor::~CFontEditor()
{
	if( m_FontAsset )
	{
		if( m_FontAsset->GetIsEdited() )
			m_FontAsset->SetIsEdited( FALSE );
	}
}

Void CFontEditor::SetupControls()
{
	Char buffer[128];

	Sb::StringSetFormattedText( buffer, 128, "%d", m_FontInfo.BitmapHeight );
	m_BitmapSize->setCurrentIndex( m_BitmapSize->findText( QString( buffer ) ) );

	QString faceName = QString( m_FontInfo.FaceName.GetBuffer() );
	if( faceName.size() )
		m_FontName->setCurrentIndex( m_FontName->findText(  faceName ) );

	m_FontSize->setValue( m_FontInfo.FontSize );
}

Bool CFontEditor::OnUIScriptChanged( int index )
{
	Int32 scriptIndex = m_ScriptCombo->itemData( index ).toInt();

	if( m_CurrentUnicodeScript != &m_UnicodeScriptTable[ scriptIndex ] )
	{
		m_CurrentUnicodeScript = &m_UnicodeScriptTable[ scriptIndex ];
		m_IsDocumentModified = TRUE;
		DrawTable();
	}
	return TRUE;
}

Bool CFontEditor::OnUIFontChanged( const QFont& font )
{
	QByteArray arr = font.family().toLocal8Bit();
	m_FontInfo.FaceName = arr.data();
	DrawTable();
	return TRUE;
}

Bool CFontEditor::OnUIResolutionChanged( const QString& value )
{
	m_FontInfo.BitmapHeight = value.toLong();
	m_FontInfo.BitmapWidth = value.toLong();

	DrawTable();
	return TRUE;
}

CApplication& CFontEditor::GetApp()
{
    return *m_App;
}

Void CFontEditor::SetFont( CAssetFont& font )
{
    m_FontAsset = &font;

	if( m_FontAsset->GetFilePath().GetValue().Exist() )
	{
		ReadFontInfo( m_FontAsset->GetFilePath().GetValue() );
		DrawTable();
	}
}


Void CFontEditor::DrawTable( )
{
	QPixmap pixMap( m_FontInfo.BitmapWidth, m_FontInfo.BitmapHeight );
	DrawTable( nDrawOption_BoundingRect | nDrawOption_CharWidth |nDrawOption_Offset, pixMap );
	m_ImageContainer->setPixmap( pixMap );
	m_ImageContainer->resize( pixMap.width(), pixMap.height() );
}

Void CFontEditor::DrawTable( Int32 options, QPixmap& bitmap )
{
	bitmap.fill( Qt::black );

	QPainter painter( &bitmap );

	QString		currentChar;

	QFont					font;
	font.setPixelSize( m_FontInfo.FontSize );
	font.setFamily( m_FontInfo.FaceName.GetBuffer() );

	QFontMetrics			fontMetrics( font );

	painter.setBackgroundMode( Qt::TransparentMode );
	painter.setFont( font );

	m_FontInfo.LineSpacing = fontMetrics.lineSpacing();

	UInt32 i;

	Int32 margins = 0;
	Int32 cursorPosX = 0;
	Int32 cursorPosY = 0;

	Int32 squareSizeX = m_FontInfo.FontSize;
	Int32 squareSizeY = m_FontInfo.FontSize * 1.5 ;
	Int32 margin = 4;
	Int32 lineSpacing = fontMetrics.lineSpacing();
    Int32 maxAbsoluteHeight = 0;

	for(i=m_CurrentUnicodeScript->LowCodePoint;i<=m_CurrentUnicodeScript->HighCodePoint;i++)
	{
		currentChar = QString(i);

		// Extract char bounds
		QRect bounds = fontMetrics.boundingRect( QChar( i ) );
		// Add margin
		bounds.adjust( -margin, -margin, margin, margin);
		// Go to next line if necessary
		if( cursorPosX + squareSizeX >= bitmap.width() )
		{
			cursorPosX = 0;
			cursorPosY += maxAbsoluteHeight;
            maxAbsoluteHeight = 0;
		}

		Int32 deltaX = (squareSizeX / 2) - fontMetrics.width(QChar(i))/2;
		Int32 deltaY = (squareSizeY / 2) + fontMetrics.height()/2 - fontMetrics.descent();
		Int32 charPosX = cursorPosX + deltaX;
		Int32 charPosY = cursorPosY + deltaY;

		Sb::CTextFont::SChar curChar;
		curChar.BoundX = charPosX + bounds.x();
		curChar.BoundY = charPosY + bounds.y();
		curChar.BoundWidth = bounds.width();
		curChar.BoundHeight = bounds.height();
		curChar.OffsetX = charPosX - curChar.BoundX;
		curChar.OffsetY = curChar.BoundY - cursorPosY;

		curChar.CharWidth = fontMetrics.width(QChar(i));

		painter.setPen(QPen(Qt::white));
		painter.drawText( charPosX, charPosY, currentChar );
	
		if( options & nDrawOption_Offset )
		{
			painter.setPen(QPen(Qt::yellow));
			painter.drawLine( curChar.BoundX, cursorPosY, curChar.BoundX, cursorPosY+curChar.OffsetY );
			painter.drawLine( curChar.BoundX, charPosY, curChar.BoundX+curChar.OffsetX, charPosY);
		}

		if( options & nDrawOption_CharWidth )
		{
			painter.setPen(QPen(Qt::red));
			painter.drawLine( charPosX, charPosY, charPosX + curChar.CharWidth, charPosY );
		}

		if( options & nDrawOption_BoundingRect )
		{
			painter.setPen(QPen(Qt::blue));
			painter.drawRect( curChar.BoundX, curChar.BoundY, curChar.BoundWidth, curChar.BoundHeight );
		}

		curChar.CharWidth = fontMetrics.width( QChar(i) );

        // Store absolute height if necessary
        Int32 absoluteHeight = curChar.OffsetY + curChar.BoundHeight;
        if( absoluteHeight > maxAbsoluteHeight )
            maxAbsoluteHeight = absoluteHeight;

		if( i < m_UVOffsetTable.GetItemCount() )
		{
			m_UVOffsetTable[i] = curChar;
		}

		cursorPosX += deltaX +  bounds.x() + bounds.width();

	}	
}

Bool CFontEditor::OnUISpinChange( int i )
{
	if( m_FontInfo.FontSize != i )
	{
		m_FontInfo.FontSize = i;
		m_IsDocumentModified = TRUE;
	}

	DrawTable();

	return TRUE;
}

Int32 CFontEditor::ReadFontInfo( const Sb::CFilePath& path )
{
	Sb::CTextFont::SHeader header;
	Int32 reso = 512;
	QByteArray bArray;

	// Read current value from UI
	m_FontInfo.BitmapHeight = m_BitmapSize->currentText().toInt();
	m_FontInfo.BitmapWidth = m_BitmapSize->currentText().toInt();
	bArray = m_FontName->currentText().toLocal8Bit();
	m_FontInfo.FaceName = (const Char*) bArray.data();
	m_FontInfo.FontSize = m_FontSize->value();

	if( path.Exist() )
	{
		Sb::CFile file;
		if( file.Open( path, nAccesMode_Read ) )
		{
			file.Read( &header, sizeof( Sb::CTextFont::SHeader ) );

			if( header.IsValidFourCC() )
			{
				if( header.GetVersion() == 0 )
				{
					file.Read(&reso, sizeof(Int32));
					m_FontInfo.BitmapHeight = reso;
					m_FontInfo.BitmapWidth = reso;
				}
				else if( header.GetVersion() == 1 )
				{
					file.Read( &m_FontInfo, sizeof( Sb::CTextFont::SInfo ) );
				}
			}
		}
	}

	SetupControls();

	return reso;
}

Bool CFontEditor::OnUIOpen()
{
	// Popup file dialog
	QString fileName = QFileDialog::getOpenFileName(this, "Font file", "s:", "Font file (*.dfnt)", NULL, QFileDialog::DontUseNativeDialog  );

	if( fileName.length() )
	{
		QByteArray arr = fileName.toLocal8Bit();
		m_FontAsset->GetFilePath().SetValue( Sb::CFilePath( arr.data() ) );

		ReadFontInfo( Sb::CFilePath( arr.data() ) );

		DrawTable();

		m_IsDocumentModified = FALSE;
	}

	return TRUE;
}

Bool CFontEditor::OnUISave()
{
	SaveFile();
	return TRUE;
}

Void CFontEditor::SaveFile()
{
	Sb::CFilePath path;

	if( m_FontAsset->GetFilePath().GetValue().IsValid() == FALSE )
	{
		QString fileName = QFileDialog::getSaveFileName(this, "Save font file", "s:", "Font file (*.dfnt)", NULL, QFileDialog::DontUseNativeDialog  );
		if( fileName.length() == 0 )
		{
			return;
		}

		QByteArray array = fileName.toLocal8Bit();
		m_FontAsset->GetFilePath().SetValue( Sb::CFilePath( array.data() ) );

		SaveFile( Sb::CFilePath( array.data() ) );
	}
	else
	{
		SaveFile( Sb::CFilePath( m_FontAsset->GetFilePath().GetValue().GetPath() ) );
	}
}

Void CFontEditor::SaveFile( const Sb::CFilePath& path )
{
	UInt32 i, x, y;
	Byte value;
	Sb::CFile file;
	Sb::CTextFont::SHeader header;

	if( file.Open( path, nAccesMode_Write ) )
	{
		// Write header
		file.Write( &header, sizeof( Sb::CTextFont::SHeader ) );

		// Write font info
		file.Write( &m_FontInfo, sizeof( Sb::CTextFont::SInfo ) );

		// Write UV Offset table
		for(i=0;i<256;i++)
		{
			file.Write( &m_UVOffsetTable[i], sizeof( Sb::CTextFont::SChar ) );
		}

		// Write image
		QPixmap pixMap( m_FontInfo.BitmapWidth, m_FontInfo.BitmapHeight );
		DrawTable( 0, pixMap );
		QImage image = pixMap.toImage();

		for( y=0;y<m_FontInfo.BitmapHeight;y++ )
		{
			for( x=0;x<m_FontInfo.BitmapWidth;x++ )
			{
				QColor pixel( image.pixel( x, y ) );
				value = (Byte) pixel.red();
				file.Write( &value, sizeof( Byte ) );
			}
		}

		m_IsDocumentModified = FALSE;
	}
}
