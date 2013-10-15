#include <Dune\Editor\Widget\ShaderEditor.h> 

#include <Dune\Resources\Icons\ShaderEditor.xpm>
#include <Sable\Graphics\Shader\Compiler.h>
#include <Dune\Editor\Widget\ShaderEditorSyntaxHighlighter.h> 

#include <QtGui\QSplitter>

using namespace Dune;

CShaderEditor::CShaderEditor( CApplication& app, CAssetShader& shader ) :
    QDockWidget( "Shader Editor", &app.GetMainFrame() ),
	m_IconTable( (const char** ) ShaderEditor_xpm )
{
	setAttribute(Qt::WA_DeleteOnClose);

	UInt32 border = 4;
    m_App = &app;
	m_Shader = NULL;

	m_Frame = new QWidget( this );

	QAction* action = NULL;
	QToolBar*	toolBar = new QToolBar( m_Frame );
	action = toolBar->addAction( m_IconTable.GetIcon( 0 ), "Open" );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnUIOpen() ) );
	action = toolBar->addAction( m_IconTable.GetIcon( 1 ), "Save" );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnUISave() ) );
	toolBar->addSeparator();
	action = toolBar->addAction( m_IconTable.GetIcon( 2 ), "Undo" );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnUIUndo() ) );
	action = toolBar->addAction( m_IconTable.GetIcon( 3 ), "Redo" );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnUIRedo() ) );
    toolBar->addSeparator();
    action = toolBar->addAction( m_IconTable.GetIcon( 4 ), "Compile" );
    connect( action, SIGNAL( triggered() ), this, SLOT( OnUICompile() ) );

	m_TextEdit[nShaderProgramType_Vertex] = new QTextEdit( m_Frame);
	m_TextEdit[nShaderProgramType_Vertex]->setUndoRedoEnabled( TRUE );
	m_TextEdit[nShaderProgramType_Vertex]->setTabStopWidth( QFontMetrics( m_TextEdit[nShaderProgramType_Vertex]->currentFont( ) ).width(' ') * 4 );
    CShaderEditorSyntaxHighlighter* highlighterVertex = new CShaderEditorSyntaxHighlighter(m_TextEdit[nShaderProgramType_Vertex]->document());

    m_TextEdit[nShaderProgramType_Pixel] = new QTextEdit( m_Frame);
    m_TextEdit[nShaderProgramType_Pixel]->setUndoRedoEnabled( TRUE );
    m_TextEdit[nShaderProgramType_Pixel]->setTabStopWidth( QFontMetrics( m_TextEdit[nShaderProgramType_Pixel]->currentFont( ) ).width(' ') * 4 );
    CShaderEditorSyntaxHighlighter* highlighterPixel = new CShaderEditorSyntaxHighlighter(m_TextEdit[nShaderProgramType_Pixel]->document());

    m_TextEdit[nShaderProgramType_Geometry] = new QTextEdit( m_Frame);
    m_TextEdit[nShaderProgramType_Geometry]->setUndoRedoEnabled( TRUE );
    m_TextEdit[nShaderProgramType_Geometry]->setTabStopWidth( QFontMetrics( m_TextEdit[nShaderProgramType_Geometry]->currentFont( ) ).width(' ') * 4 );
    CShaderEditorSyntaxHighlighter* highlighterGeometry = new CShaderEditorSyntaxHighlighter(m_TextEdit[nShaderProgramType_Geometry]->document());

    m_TabWidget = new QTabWidget( m_Frame );
    m_TabWidget->addTab( m_TextEdit[nShaderProgramType_Vertex], "Vertex Program" );
    m_TabWidget->addTab( m_TextEdit[nShaderProgramType_Pixel], "Pixel Program" );
    m_TabWidget->addTab( m_TextEdit[nShaderProgramType_Geometry], "Geometry Program" );

    m_CurrentProgramEdited = nShaderProgramType_Vertex;

    connect( m_TabWidget, SIGNAL( currentChanged( int ) ), this, SLOT( OnUITabChanged( int ) ) );

	m_LogWindow = new QListWidget( m_Frame );
    m_LogWindow->setVerticalScrollMode( QAbstractItemView::ScrollPerPixel );
    connect( m_LogWindow, SIGNAL( itemDoubleClicked ( QListWidgetItem*) ), this, SLOT( OnUISelectError( QListWidgetItem* ) ) );

	QSplitter* splitter = new QSplitter( Qt::Vertical, m_Frame );
	splitter->addWidget( m_TabWidget );
	splitter->addWidget( m_LogWindow );
	splitter->setStretchFactor( 0, 4 );
	splitter->setStretchFactor( 1, 1 );
	QVBoxLayout *layout = new QVBoxLayout( m_Frame );

	layout->addWidget( toolBar );
	layout->addWidget( splitter );

	layout->setContentsMargins( 0, 0, 0, 0 );
	m_Frame->setLayout( layout );      // use the sizer for layout

	setWidget( m_Frame );
	resize( QSize( 800, 600 ) );

	SetShader( shader );
}

CShaderEditor::~CShaderEditor()
{
	if( m_Shader)
	{
		SaveFilesAndBake();
        if( m_Shader->GetIsEdited() )
            m_Shader->SetIsEdited( FALSE );
	}
}

void CShaderEditor::OnUISelectError( QListWidgetItem * item )
{
    // Regexp to match error
    QRegExp regExp( "(.*\\.(.*))\\((\\d*),(\\d*)\\)" );
    if ( regExp.indexIn(item->text()) != -1 )
    {
        QString fileExt = regExp.cap( 2 );
        QString lineNo = regExp.cap( 3 );
        QString columnNo = regExp.cap( 4 );

        if( fileExt == "vsh")
            GoTo( nShaderProgramType_Vertex, lineNo.toUInt()-1, columnNo.toUInt() );
        else if( fileExt == "psh")
            GoTo( nShaderProgramType_Pixel, lineNo.toUInt()-1, columnNo.toUInt() );
        else if( fileExt == "gsh")
            GoTo( nShaderProgramType_Geometry, lineNo.toUInt()-1, columnNo.toUInt() );
    }
}

Bool CShaderEditor::LoadFile(const Sb::CFilePath &filename, EShaderProgramType programType )
{
	QFile file( filename.GetBuffer() );
	if (!file.open(QIODevice::ReadOnly))
	{
		return FALSE;	
	}
	m_TextEdit[programType]->document()->setPlainText( QString( file.readAll() ) );
	return TRUE;
}

Bool CShaderEditor::SaveFile(const Sb::CFilePath &filename, EShaderProgramType programType )
{
	QFile file( filename.GetBuffer() );
	if (!file.open(QIODevice::WriteOnly))
	{
		return FALSE;	
	}
	file.write( m_TextEdit[programType]->document()->toPlainText().toLocal8Bit() );
    file.close();
	
    return TRUE;
}

CApplication& CShaderEditor::GetApp()
{
    return *m_App;
}

Bool CShaderEditor::OnUIOpen( )
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open Shader", "s:", "Shader (*.psh *.vsh *.gsh)", NULL, QFileDialog::DontUseNativeDialog  );

	if( fileName.length() )
	{
        if( QFileInfo( fileName ).suffix() == "psh")
        {
            QByteArray arr = fileName.toLocal8Bit();
            m_Shader->GetPixelShaderFilePath().SetValue( Sb::CFilePath( arr.data() ) );
        }
        else if( QFileInfo( fileName ).suffix() == "vsh")
        {
            QByteArray arr = fileName.toLocal8Bit();
            m_Shader->GetVertexShaderFilePath().SetValue( Sb::CFilePath( arr.data() ) );
        }
        else if( QFileInfo( fileName ).suffix() == "gsh")
        {
            QByteArray arr = fileName.toLocal8Bit();
            m_Shader->GetGeometryShaderFilePath().SetValue( Sb::CFilePath( arr.data() ) );
        }

        SetShader( *m_Shader );
	}

	return TRUE;
}

Bool CShaderEditor::OnUISave( )
{
	SaveFilesAndBake();
	return TRUE;
}

Bool CShaderEditor::OnUIUndo( )
{
	m_TextEdit[nShaderProgramType_Vertex]->undo();
	return TRUE;
}

Bool CShaderEditor::OnUIRedo( )
{
	m_TextEdit[nShaderProgramType_Vertex]->redo();
	return TRUE;
}

Bool CShaderEditor::OnUICompile( )
{
    ClearLog();

    SaveFilesAndBake();

    if( m_Shader->GetVertexShaderFilePath().GetValue().Exist() )
    {
        Sb::CShaderCompiler compiler;
        if( !compiler.Compile( nShaderProgramType_Vertex, m_Shader->GetVertexShaderFilePath().GetValue() ) )
        {
            Log( "Vertex Shader: ");
            Log( compiler.GetErrorString() );
        }
        else
        {
            Log( "Vertex Shader compiled successfully.");
        }
    }

    if( m_Shader->GetPixelShaderFilePath().GetValue().Exist() )
    {
        Sb::CShaderCompiler compiler;
        if( !compiler.Compile( nShaderProgramType_Pixel, m_Shader->GetPixelShaderFilePath().GetValue() ) )
        {
            Log( "Pixel Shader: ");
            Log( compiler.GetErrorString() );
        }
        else
        {
            Log( "Pixel Shader compiled successfully.");
        }
    }

    if( m_Shader->GetGeometryShaderFilePath().GetValue().Exist() )
    {
        Sb::CShaderCompiler compiler;
        if( !compiler.Compile( nShaderProgramType_Geometry, m_Shader->GetGeometryShaderFilePath().GetValue() ) )
        {
            Log( "Geometry Shader: ");
            Log( compiler.GetErrorString() );
        }
        else
        {
            Log( "Geometry Shader compiled successfully.");
        }
    }

	return TRUE;
}

Void CShaderEditor::GoTo( EShaderProgramType program, Int32 lineNo, Int32 columnNo )
{
    int i = 0;
    QTextCursor cursor = m_TextEdit[program]->textCursor();
    cursor.beginEditBlock();
    i = cursor.movePosition( QTextCursor::Start, QTextCursor::MoveAnchor );
    i = cursor.movePosition( QTextCursor::Down, QTextCursor::MoveAnchor, lineNo );
    //i = cursor.movePosition( QTextCursor::Right, QTextCursor::MoveAnchor, columnNo );
    cursor.endEditBlock();
    m_TextEdit[program]->setTextCursor( cursor );

    if( program == nShaderProgramType_Vertex )
        m_TabWidget->setCurrentIndex ( 0 );
    else if( program == nShaderProgramType_Pixel )
        m_TabWidget->setCurrentIndex ( 1 );
    else if( program == nShaderProgramType_Geometry )
        m_TabWidget->setCurrentIndex ( 2 );

    m_TextEdit[program]->setFocus();
}

void CShaderEditor::OnUITabChanged( int i )
{
    if( i == 0 )
        m_CurrentProgramEdited = nShaderProgramType_Vertex;
    else if( i == 1 )
        m_CurrentProgramEdited = nShaderProgramType_Pixel;
    else if( i == 2 )
        m_CurrentProgramEdited = nShaderProgramType_Geometry;
}

Void CShaderEditor::ClearLog( )
{
    m_LogWindow->clear();
}

Void CShaderEditor::Log( String txt )
{
	QListWidgetItem* item = new QListWidgetItem();
	item->setText( txt );
	m_LogWindow->addItem( item );	
	m_LogWindow->scrollToItem( item );
}

Void CShaderEditor::SaveFilesAndBake()
{
    SaveFile( m_Shader->GetVertexShaderFilePath().GetValue(), nShaderProgramType_Vertex );
    SaveFile( m_Shader->GetPixelShaderFilePath().GetValue(), nShaderProgramType_Pixel );
    SaveFile( m_Shader->GetGeometryShaderFilePath().GetValue(), nShaderProgramType_Geometry );

    GetApp().GetEditorContainerView().BakeObject( m_Shader->GetHandle() );
}

Void CShaderEditor::SetShader( CAssetShader& asset )
{
	m_Shader = &asset;
	LoadFile( m_Shader->GetVertexShaderFilePath().GetValue(), nShaderProgramType_Vertex );
	LoadFile( m_Shader->GetPixelShaderFilePath().GetValue(), nShaderProgramType_Pixel );
	LoadFile( m_Shader->GetGeometryShaderFilePath().GetValue(), nShaderProgramType_Geometry );
}
