#include <Dune\Editor\Widget\BakeWindow.h> 

#include <Dune\Model\Project\Project.h>
#include <Dune\Editor\Application\Application.h>

#include <QtGui\QLayout.h>

using namespace Dune;


CBakeWindow::CBakeWindow( CApplication& app, CProject& prj ) :
    QDialog( (QWidget*)&app.GetMainFrame(), Qt::Tool ),
	m_BakeDispatcher( prj )
{
	Sb::CFilePath outputPath;

	setWindowTitle( "Bake project");

	if( prj.GetSettings().GetBakeOutputPath().IsValid() )
	{
		outputPath = prj.GetSettings().GetBakeOutputPath();
	}
	else
	{
		Char prjDir[256];
		// Be sure that the outputPath will be considered as a directory
		Sb::StringAppend( prj.GetProjectDirectory(), "/", prjDir, 256 );

		outputPath = Sb::CFilePath( prjDir );
		outputPath.AppendDir( "Bank" );
		prj.GetSettings().SetBakeOutputPath( outputPath );
	}

    QVBoxLayout *mainLayout = new QVBoxLayout( );

    QHBoxLayout* subLayoutA = new QHBoxLayout( );
    QHBoxLayout* subLayoutSubA = new QHBoxLayout( );
    QHBoxLayout* subLayoutB = new QHBoxLayout( );
	QHBoxLayout* subLayoutC = new QHBoxLayout( );
    QHBoxLayout* subLayoutD = new QHBoxLayout( );
    QHBoxLayout* subLayoutE = new QHBoxLayout( );

    QWidget* w;

    // Sub sizer 1
    w = new QLabel( "Output Path" );
    subLayoutA->addWidget( w );

    m_OutputPath = new QLineEdit( );
	m_OutputPath->setText( outputPath.GetPath() );
	subLayoutSubA->addWidget(m_OutputPath );
    w = new QPushButton( "..." );
	connect( w, SIGNAL( clicked( ) ), this, SLOT( OnUISelectOutputPath( ) ) );
	subLayoutSubA->addWidget( w );

    subLayoutA->addLayout( subLayoutSubA );

	// Sub sizer B
	m_OptimizeModels = new QCheckBox( "Optimize models" );
	m_OptimizeModels->setCheckState( Qt::Unchecked );
	subLayoutB->addWidget( m_OptimizeModels );

    // Sub sizer 2
    w = new QLabel( "Progress" );
    subLayoutC->addWidget( w );
    m_ProgressGauge = new QProgressBar( );
    subLayoutC->addWidget( m_ProgressGauge );

    // Sub sizer 3
    w = new QLabel( "Log" );
    subLayoutD->addWidget( w );
    m_LogWindow = new QListWidget( );
    subLayoutD->addWidget( m_LogWindow );

    // Sub sizer 4
    m_BakeCloseBtn = new QPushButton( "Bake" );
	connect( m_BakeCloseBtn, SIGNAL( clicked() ), this, SLOT( OnUIBake() ) );
	m_BakeCloseBtn->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    subLayoutE->addWidget(m_BakeCloseBtn, 0, Qt::AlignRight );
    m_CancelBtn = new QPushButton( "Cancel" );
	m_CancelBtn->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
	connect( m_BakeCloseBtn, SIGNAL( clicked() ), this, SLOT( OnUIClose() ) );
	subLayoutE->addWidget(m_CancelBtn, 0, Qt::AlignRight );
	subLayoutE->addStretch();

    m_CancelBtn->	setEnabled( FALSE );

    mainLayout->addLayout( subLayoutA );
    mainLayout->addLayout( subLayoutB );
    mainLayout->addLayout( subLayoutC );
    mainLayout->addLayout( subLayoutD );
    mainLayout->addLayout( subLayoutE );

    setLayout( mainLayout );

    resize( 700, 60 );
 
    m_Project = &prj;

    m_BakeStarted = FALSE;

	m_BakeCloseBtn->setFocus ();

	connect( &m_Timer, SIGNAL( timeout() ), this, SLOT( OnTimerTimeout() ) );
	m_Timer.setSingleShot ( FALSE );

	m_ProgressGauge->setRange( 0, 100 );
	m_ProgressGauge->setValue( 0 );
	m_ProgressGauge->setFormat( "" );

    // Cette "indirection" est necessaire de manière a transferer le controle de la sortie log au thread principal
    // pour pallier a certain problèmes lié au fait que CListWidget n'est pas ThreadSafe
    bool r = connect( this, SIGNAL( SignalOutputLogMsg( int, const QString& ) ), this, SLOT( OutputLogMsg( int, const QString& ) ), Qt::QueuedConnection );
}

CBakeWindow::~CBakeWindow()
{

}

Void CBakeWindow::OnLogMsg( CBakeContext::EType type, String msg )
{
    Q_EMIT( SignalOutputLogMsg( type, msg ) );
}

Bool CBakeWindow::OutputLogMsg( int type, const QString& msg )
{
	QListWidgetItem* item = new QListWidgetItem( );
	QString text;

    if( type == CBakeContext::nType_Error )
    {
        item->setTextColor( QColor( 255, 0, 0 ) );
        text =  QString("Error : ") + msg;
    }
    else if( type == CBakeContext::nType_Warning )
    {
        item->setTextColor( QColor( 255, 127, 0 ) );
        text =  QString("Warning : ") + msg;
    }
    else if( type == CBakeContext::nType_Info )
    {
        item->setTextColor( QColor( 0, 0, 255 ) );
        text =  QString("Info : ") + msg;
    }

	item->setText( text );

	m_LogWindow->addItem( item );
	m_LogWindow->scrollToItem( item );

    return TRUE;
}

Bool CBakeWindow::OnTimerTimeout()
{
    if( m_BakeContext )
    {
        if( m_BakeContext->Update() == FALSE )
        {
			m_ProgressGauge->setRange( 0, 100 );
			m_ProgressGauge->setValue( 0 );
            // Destroy baking context
            m_BakeContext = NULL;
            m_CancelBtn->setEnabled( FALSE );
            m_BakeCloseBtn->setEnabled( TRUE );
            m_ProgressGauge->setValue( 100 );
			m_BakeCloseBtn->setFocus();
			m_BakeCloseBtn->setText( "Close" );
            m_Timer.stop();
        }
    }
	return TRUE;
}

Bool CBakeWindow::OnUIClose( )
{
    if( m_BakeContext )
    {
        m_BakeContext->Cancel();
    }

	return TRUE;
}

Bool CBakeWindow::ExtractOutputPath()
{
    Char   outpuPath[256];

    // Grab output path
    QString outputPath = m_OutputPath->text();

    QFileInfo  outputPathName ( outputPath  );

	Sb::StringCopy( outputPath.toAscii().constData(), outpuPath, 256 );

	// Be sure that the outputPath will be considered as a directory
	Sb::StringAppend( outpuPath, "/", outpuPath, 256 );

    m_BakeConfig.OutputPath = Sb::CFilePath( outpuPath );

	// Save output path to project settings
	m_Project->GetSettings().SetBakeOutputPath( m_BakeConfig.OutputPath );

    return TRUE;

}

Bool CBakeWindow::OnUIBake( )
{
    DebugAssert( m_BakeContext == NULL );

    if( !m_BakeStarted )
    {
        if( ExtractOutputPath() )
        {
            m_LogWindow->clear();
            m_CancelBtn->setEnabled( TRUE );
            m_BakeCloseBtn->setEnabled( FALSE );

			m_BakeConfig.OptimizeModels = m_OptimizeModels->checkState() == Qt::Checked;

            // Start baking in a new context
            m_BakeContext = NEWOBJ( CBakeContextExport, ( *m_Project, m_BakeDispatcher, m_BakeConfig ) );
            
			EVENT_CONNECT_SLOT( m_BakeContext->SignalLog, m_SlotLog, &CBakeWindow::OnLogMsg );

			m_BakeContext->Start( );

            m_BakeStarted = TRUE;

			m_Timer.setInterval( 100 );
			m_Timer.start( 100 );
			m_ProgressGauge->setRange( 0, 0 );
        }
    }
    else
    {
        done(0);
    }

	return TRUE;
}

Bool CBakeWindow::OnUISelectOutputPath( )
{
	// Popup file dialog
	QString fileName = QFileDialog::getExistingDirectory(this, "Output path", "s:", QFileDialog::ShowDirsOnly | QFileDialog::DontUseNativeDialog  );

	if( fileName.length() )
	{
		 m_OutputPath->setText( fileName.toAscii().constData() );
	}
	return TRUE;
}
