#include <Dune/Editor/View/MainFrame.h>

#include <Dune/Editor/View/Property.h>
#include <Dune/Editor/View/Log.h>
#include <Dune/Editor/View/CommandStacks.h>
#include <Dune/Editor/View/Asset.h>
#include <Dune/Editor/View/Scene.h>
#include <Dune/Editor/View/UI.h>
#include <Dune/Editor/View/Editor/EditorContainer.h>
#include <Dune/Editor/Widget/BakeWindow.h> 
#include <Dune/Editor/Ctrl/Project.h>
#include <Dune/Common/Log.h>
#include <Dune/Resources/Icons/MainFrame.xpm>

#include <QtGui/QShortcut>
#include <QtGui/QMessageBox>

#define PROJECT_MENU_ID 2

using namespace Dune;

CViewMainFrame::CViewMainFrame( CApplication& app ) : 
	QMainWindow( ),
	CEventClient( app )
{
    m_App = &app;
	m_PropertyView = NULL;
    m_EditorView = NULL;
    m_RecentProjectsMenu = NULL;
    setWindowState( windowState() | Qt::WindowMaximized );
}

CViewMainFrame::~CViewMainFrame()
{

}

CApplication& CViewMainFrame::GetApp() const
{
    return *m_App;
}

Void CViewMainFrame::PulseProgressBar( Bool onOff, String desc )
{
	m_MenuProgressBar->setEnabled( onOff );
	m_MenuProgressBar->setTextVisible( onOff );
	if( onOff )
	{
		m_MenuProgressBar->setRange( 0, 0 );
		QString format = QString( desc ) + QString( "%p" );
		m_MenuProgressBar->setFormat( format );
	}
	else
	{
		m_MenuProgressBar->setRange( 0, 100 );
		m_MenuProgressBar->setValue( 0 );
		m_MenuProgressBar->setFormat( QString( "" ) );
	}
}

Void CViewMainFrame::SetupMenu()
{
	// Setup Menu
	m_MenuBar = new QMenuBar( this );

	QAction* action;
	QMenu* menuFile = new QMenu( "&File", this );

	// New
	action = menuFile->addAction( "&New..." );

	QShortcut* shortcut = new QShortcut( QKeySequence( Qt::CTRL + Qt::Key_N ), this );
	connect(shortcut, SIGNAL(activated()), this, SLOT(OnUINew()));
	connect(action, SIGNAL(triggered()), this, SLOT(OnUINew()));

	// Open
	action = menuFile->addAction( "&Open..." );
	shortcut = new QShortcut( QKeySequence( Qt::CTRL + Qt::Key_O ), this );
	connect(shortcut, SIGNAL(activated()), this, SLOT(OnUIOpen()));
	connect(action, SIGNAL(triggered()), this, SLOT(OnUIOpen()));

	// Save
	menuFile->addSeparator();
	m_ActionSave = menuFile->addAction( "&Save" );
	shortcut = new QShortcut( QKeySequence( Qt::CTRL + Qt::Key_S ), this );
	connect(shortcut, SIGNAL(activated()), this, SLOT(OnUISave()));
	connect(m_ActionSave, SIGNAL(triggered()), this, SLOT(OnUISave()));

	// Save as
	m_ActionSaveAs = menuFile->addAction( "&Save as..." );
	connect(m_ActionSaveAs, SIGNAL(triggered()), this, SLOT(OnUISaveAs()));
	menuFile->addSeparator();
	m_ActionClose = menuFile->addAction( "&Close" );
	connect(m_ActionClose, SIGNAL(triggered()), this, SLOT(OnUIClose()));
	menuFile->addSeparator();

	m_RecentProjectsMenu = new QMenu( "&Recent Project", this );
	menuFile->addMenu( m_RecentProjectsMenu );

	// Quit
	menuFile->addSeparator();
	action = menuFile->addAction( "&Quit" );
	connect(action, SIGNAL(triggered()), this, SLOT(OnUIQuit()));

	// MenuEdit
	m_MenuEdit = new QMenu( "&Edit", this );

	// Undo
	action = m_MenuEdit->addAction( "&Undo" );
	shortcut = new QShortcut( QKeySequence( Qt::CTRL + Qt::Key_Z ), this );
	connect(shortcut, SIGNAL(activated()), this, SLOT(OnUIUndo()));
	connect(action, SIGNAL(triggered()), this, SLOT(OnUIUndo()));

	// Redo
	action = m_MenuEdit->addAction( "&Redo" );
	shortcut = new QShortcut( QKeySequence( Qt::CTRL + Qt::Key_Y ), this );
	connect(shortcut, SIGNAL(activated()), this, SLOT(OnUIRedo()));
	connect(action, SIGNAL(triggered()), this, SLOT(OnUIRedo()));

	// Menu project
	m_MenuProject = new QMenu( "&Project", this );

	// Bake
	action = m_MenuProject->addAction( "&Bake..." );
	shortcut = new QShortcut( QKeySequence( Qt::CTRL + Qt::Key_B ), this );
	connect(shortcut, SIGNAL(activated()), this, SLOT(OnUIBakeProject()));
	connect(action, SIGNAL(triggered()), this, SLOT(OnUIBakeProject()));

	// Settings
	action = m_MenuProject->addAction( "&Settings..." );
	connect(action, SIGNAL(triggered()), this, SLOT(OnUIEditSettings()));

	// View menu
	m_ViewMenu = new QMenu( "&View", this );

	m_MenuBar->addMenu( menuFile );
	m_MenuBar->addMenu( m_MenuEdit );
	m_MenuBar->addMenu( m_MenuProject );
	m_MenuBar->addMenu( m_ViewMenu );

	setMenuBar( m_MenuBar );
}

Void CViewMainFrame::Initialize()
{
	setDockOptions( QMainWindow::AllowTabbedDocks | QMainWindow::AllowNestedDocks );
    m_StatusBar = new QStatusBar( this );
	m_MenuProgressBar = new QProgressBar();
	m_MenuProgressBar->setMaximumWidth( 200 );
	m_StatusBar->addPermanentWidget ( new QLabel( "Status: " ) );
	m_StatusBar->addPermanentWidget ( m_MenuProgressBar );

	setStatusBar( m_StatusBar );

	SetupMenu();

    {
        m_BankView = new CViewBank( *this );
        addDockWidget(Qt::LeftDockWidgetArea, m_BankView, Qt::Vertical);
    }

	{
		m_AssetView = new CViewAsset( *this );
		addDockWidget(Qt::LeftDockWidgetArea, m_AssetView, Qt::Horizontal);
	}

	{
		m_GameView = new CViewGame( *this );
		addDockWidget(Qt::LeftDockWidgetArea, m_GameView, Qt::Horizontal);
	}

	{
		m_SceneView = new CViewScene( *this );
		addDockWidget(Qt::LeftDockWidgetArea, m_SceneView, Qt::Horizontal);
	}

	{
		m_UIView = new CViewUI( *this );
		addDockWidget(Qt::LeftDockWidgetArea, m_UIView, Qt::Horizontal);
	}

	splitDockWidget( m_BankView, m_AssetView, Qt::Vertical );
	tabifyDockWidget( m_AssetView, m_UIView );
	tabifyDockWidget( m_AssetView, m_SceneView );
	tabifyDockWidget( m_AssetView, m_GameView );

    {
		m_PropertyView = new CViewProperty( *this );
		addDockWidget(Qt::RightDockWidgetArea, m_PropertyView, Qt::Vertical);
    }


	{
		m_EditorView = new CViewEditorContainer( *this );
		setCentralWidget(m_EditorView);
	}
	
	{
		m_LogView = new CViewLog( *this );
		addDockWidget(Qt::BottomDockWidgetArea, m_LogView, Qt::Horizontal);
    }

	{
		m_CommandStacksView = new CViewCommandStacks( *this );
		addDockWidget(Qt::BottomDockWidgetArea, m_CommandStacksView, Qt::Horizontal);
	}
	
	tabifyDockWidget( m_CommandStacksView, m_LogView );

	QAction* action;

	action = m_BankView->toggleViewAction();
	action->setText( "&Bank View" );
	m_ViewMenu->addAction( action );

    action = m_AssetView->toggleViewAction();
    action->setText( "&Asset View" );
    m_ViewMenu->addAction( action );

	action = m_SceneView->toggleViewAction();
	action->setText( "&Scene View" );
	m_ViewMenu->addAction( action );

	action = m_GameView->toggleViewAction();
	action->setText( "&Game View" );
	m_ViewMenu->addAction( action );

	action = m_UIView->toggleViewAction();
	action->setText( "&UI View" );
	m_ViewMenu->addAction( action );

	action = m_PropertyView->toggleViewAction();
	action->setText( "&Property View" );
	m_ViewMenu->addAction( action );

	action = m_LogView->toggleViewAction();
	action->setText( "&Log View" );
	m_ViewMenu->addAction( action );

	action = m_CommandStacksView->toggleViewAction();
	action->setText( "&Command Stacks View" );
	m_ViewMenu->addAction( action );

    EnableUI( FALSE );

    UpdateRecentProjectMenu();
	UpdateTitle();
	
}

QMenu& CViewMainFrame::GetViewMenu()
{
	return *m_ViewMenu;
}

Void CViewMainFrame::OnSubmitCommand( const CCmd& cmd )
{
	UpdateTitle();
}


Void CViewMainFrame::EnableUI( Bool onOff )
{
    m_MenuEdit->setEnabled ( onOff );
	m_MenuProject->setEnabled ( onOff );
	m_ActionClose->setEnabled( onOff );
	m_ActionSaveAs->setEnabled( onOff );
	m_ActionSave->setEnabled( onOff );

	m_SceneView->setEnabled( onOff ); 
	m_GameView->setEnabled( onOff );
	m_BankView->setEnabled( onOff ); 
	m_AssetView->setEnabled( onOff ); 
	m_UIView->setEnabled( onOff ); 
	m_EditorView->setEnabled( onOff ); 
	m_LogView->setEnabled( onOff );  
	m_PropertyView->setEnabled( onOff ); 
	m_CommandStacksView->setEnabled( onOff ); 

}


CViewEditorContainer& CViewMainFrame::GetEditorContainerView()
{
    return *m_EditorView;
}

void CViewMainFrame::OnUIQuit( )
{
	GetApp().quit();
}

void CViewMainFrame::OnUINew( )
{
    if( m_App->GetProjectCtrl().GetProject() )
    {
		QMessageBox msgBox;
		msgBox.setInformativeText("All yout changes will be lost, do you really want to continue ?");
		msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
		msgBox.setDefaultButton(QMessageBox::No);
        if( msgBox.exec() == QMessageBox::No )
            return;
    }

    OnUICreateProject( );

}

Void CViewMainFrame::UpdateRecentProjectMenu( )
{
	QList<QAction *>::Iterator it;
	QList<QAction *> actionList = m_RecentProjectsMenu->actions();

	for(it=actionList.begin(); it != actionList.end(); it++ )
	{
		m_RecentProjectsMenu->removeAction( *it );
	}

    const CApplicationSettings::RecentProjectList& list = GetApp().GetSettings().GetRecentProjectList();
    CApplicationSettings::RecentProjectList::Iterator prjIt;

    Int32 i = 0;
    ForEachItem( prjIt, list )
    {
        if( prjIt.GetData().IsEmpty() == FALSE )
		{
			QAction* action = m_RecentProjectsMenu->addAction( prjIt.GetData().GetBuffer() );
			connect( action, SIGNAL( triggered() ), this, SLOT( OnUIOpenRecentProject() ) );
			i++;
		}
    }

}

void CViewMainFrame::OnUIOpen( )
{
	// Popup file dialog
	QString fileName = QFileDialog::getOpenFileName(this, "Open Project", "s:", "Dune project (*.dprj)", NULL, QFileDialog::DontUseNativeDialog );

	if( fileName.length() )
	{
        OnUILoadProject( Sb::CFilePath( fileName.toLocal8Bit().constData() ) );
    }
}

Void CViewMainFrame::OnLoadDAE( Bool onOff )
{
	PulseProgressBar( onOff );
}

Void CViewMainFrame::OnProjectCreated( CProject& p )
{
	GetApp().GetSettings().AddRecentProject( p );

	UpdateRecentProjectMenu();
	UpdateTitle();
	EnableUI( TRUE );

//	EVENT_CONNECT_SLOT( GetProject()->GetRepo().GetFBXManager().SignalLoadDAE, m_SlotLoadDAE, &CViewMainFrame::OnLoadDAE );
}

Void CViewMainFrame::OnProjectDestroyed()
{
	EnableUI( FALSE );
}

Void CViewMainFrame::UpdateTitle()
{
	QString s = "Dune";

	if( GetProject() )
	{
		const Sb::CFilePath& f = GetProject()->GetProjectFilePath();
		s.append( " - " );
		s.append( f.GetPath() );
		if( GetCommandProcessor().GetUndoStack().GetItemCount() )
		{
			s.append( " *" );
		}
	}
	
	setWindowTitle ( s );
}

Bool CViewMainFrame::OnUIOpenRecentProject( )
{
	QAction* action = (QAction*)sender();

    OnUILoadProject( Sb::CFilePath( action->text().toLocal8Bit().constData() ) );

	return TRUE;
}

void CViewMainFrame::OnUISaveAs( )
{
	QString fileName = QFileDialog::getSaveFileName(this, "Save Project", "s:", "Dune project (*.dprj)", NULL, QFileDialog::DontUseNativeDialog );

	if( fileName.length() )
	{
        OnUISaveProject( Sb::CFilePath( fileName.toLocal8Bit().constData() ) );
    }
}

void CViewMainFrame::OnUIClose( )
{	
    OnUICloseProject();
}

void CViewMainFrame::OnUISave( )
{
    OnUISaveProject( Sb::CFilePath() );
}

CViewBank& CViewMainFrame::GetBankView() const
{
    return *m_BankView;
}

CViewAsset&	 CViewMainFrame::GetAssetView() const
{
	return *m_AssetView;
}

CViewScene&	 CViewMainFrame::GetSceneView() const
{
	return *m_SceneView;
}

CViewUI&  CViewMainFrame::GetUIView() const
{
	return *m_UIView;
}

CViewProperty& CViewMainFrame::GetPropertyView() const
{
    return *m_PropertyView;
}

void CViewMainFrame::OnUIEditSettings( )
{
    /*CViewProjectSettings settings( *this );

    settings.ShowModal();
	*/
}

void CViewMainFrame::OnUIBakeProject( )
{
    CBakeWindow bake( GetApp(), *GetProject() );

    bake.exec();
}

void CViewMainFrame::OnUIRedo( )
{
	GetCommandProcessor().Redo();
}

void CViewMainFrame::OnUIUndo( )
{
	GetCommandProcessor().Undo();
}

Bool CViewMainFrame::OnUICreateProject()
{
    //COMMAND: CloseProject
    CCmd* cmd = NEWOBJ( CCmdCloseProject, ( m_App->GetProjectCtrl() ) );
    if( ! GetApp().GetCommandProcessor().Submit( *cmd ) )
        return FALSE;

    // Popup file dialog
    QString fileName = QFileDialog::getSaveFileName(this, "Create Project", "s:", "Dune project (*.dprj)", NULL, QFileDialog::DontUseNativeDialog  );

    if( fileName.length() )
    {
        QByteArray ba = fileName.toLocal8Bit();
        String path = ba.constData();

        //COMMAND: CreateProject
        CCmd* cmd = NEWOBJ( CCmdCreateProject, ( m_App->GetProjectCtrl(), Sb::CFilePath( path ) ) );
        if( !GetApp().GetCommandProcessor().Submit( *cmd ) )
            return FALSE;
    }

    return FALSE;
}

Bool CViewMainFrame::OnUICloseProject()
{
    CCmd* cmd;
    //COMMAND: CloseProject
    cmd = NEWOBJ( CCmdCloseProject, ( m_App->GetProjectCtrl() ) );
    if( !GetCommandProcessor().Submit( *cmd ) )
        return FALSE;

    return TRUE;
}

Bool CViewMainFrame::OnUILoadProject( const Sb::CFilePath& path )
{
    CCmd* cmd;
    //COMMAND: CloseProject
    cmd = NEWOBJ( CCmdCloseProject, ( m_App->GetProjectCtrl() ) );
    if( ! GetCommandProcessor().Submit( *cmd ) )
        return FALSE;

    //COMMAND: CreateProject
    cmd = NEWOBJ( CCmdCreateProject, ( m_App->GetProjectCtrl(), Sb::CFilePath( path ) ) );
    if( ! GetCommandProcessor().Submit( *cmd ) )
        return FALSE;

    //COMMAND: LoadProject
    cmd = NEWOBJ( CCmdLoadProject, ( m_App->GetProjectCtrl(), Sb::CFilePath( path ) ) );
    if( ! GetCommandProcessor().Submit( *cmd ) )
        return FALSE;

    return TRUE;
}

Bool CViewMainFrame::OnUISaveProject( const Sb::CFilePath& path )
{
    CCmd* cmd;
    //COMMAND: SaveProject
    cmd = NEWOBJ( CCmdSaveProject, ( m_App->GetProjectCtrl(), Sb::CFilePath( path ) ) );
    return GetCommandProcessor().Submit( *cmd );
}
