#include <Dune/Editor/View/Game.h> 

#include <Dune/Model/Game/Header.h>
#include <Dune/Resources/Icons/GameTools.xpm>
#include <Dune/Resources/Icons/GameType.xpm>

#include <Dune/Editor/Ctrl/Cmd/MergeBank.h>

using namespace Dune;

CViewGame::CViewGame( CViewMainFrame& mainFrame ) :
    QDockWidget( "Game Browser", &mainFrame ),
	CEventClient( mainFrame.GetApp() ),
	m_IconToolsTable( (const char**)GameTools_xpm ),
	m_IconTypeTable( (const char**)GameType_xpm )
{
	QWidget* content = new QWidget();
	m_ToolBar = new QToolBar( this );
	m_ToolBar->setIconSize( QSize( 16, 16 ) );
	
	m_ObjectLibrary = new CObjectLibrary( GetApp(), "Game library", CGameObject::GetStaticTypeInfo()  );
	m_ObjectLibrary->setFloating( TRUE );
	m_ObjectLibrary->hide();
	GetApp().GetMainFrame().addDockWidget( Qt::AllDockWidgetAreas, m_ObjectLibrary );
	connect( m_ObjectLibrary, SIGNAL( OnObjectSelected( CLibraryObject& ) ), this, SLOT( OnUISelectLibraryObject( CLibraryObject& ) ) );

	QAction* objLibToggleAction = m_ObjectLibrary->toggleViewAction();
	objLibToggleAction->setIcon( m_IconToolsTable.GetIcon( 0 ) );
	objLibToggleAction->setToolTip( "Open game library" );
	GetApp().GetMainFrame().GetViewMenu().addAction( objLibToggleAction );

	m_ToolBar->addAction( objLibToggleAction );

	QAction* toolBarAction = m_ToolBar->addAction( m_IconToolsTable.GetIcon( 1 ), "Add game object" );
	toolBarAction->setToolTip( "Add game object" );
	connect( toolBarAction, SIGNAL( triggered( ) ), this, SLOT( OnUIShowAddGameMenu( ) ) );

	QAction* toggleAction = m_ObjectLibrary->toggleViewAction();
	toggleAction->setText( "&Game library" );
	GetApp().GetMainFrame().GetViewMenu().addAction( toggleAction );

	connect( toggleAction, SIGNAL( toggled( bool ) ), toolBarAction, SLOT( setChecked( bool ) ) );

    QVBoxLayout* layout = new QVBoxLayout();

	m_Tree = new CViewGameTree( *this, GetApp() );
	layout->addWidget( m_ToolBar );
	layout->addWidget( m_Tree );

	layout->setContentsMargins( 0, 0, 0, 0 );
	content->setLayout( layout );

	setWidget( content );
}

CViewGame::~CViewGame()
{

}

Void CViewGame::ConfigureAddGameMenu( QMenu& subMenu )
{
	UInt32 id = 0;

	QAction* action = subMenu.addAction( CBankObjectDirectory::GetStaticTypeInfo().GetTypeName() );
	action->setIcon( GetIcon( CBankObjectDirectory::GetStaticTypeInfo() ) );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnUIAddGame() ) );
	subMenu.addSeparator();

	{
		const Sb::CTypeInfo& type = CGameObject::GetStaticTypeInfo();
		action = subMenu.addAction( type.GetTypeName() );
		connect( action, SIGNAL( triggered() ), this, SLOT( OnUIAddGame() ) );
		action->setIcon( GetIcon( type ) );
	}
	{
		const Sb::CTypeInfo& type = CGameObjectPrototype::GetStaticTypeInfo();
		action = subMenu.addAction( type.GetTypeName() );
		connect( action, SIGNAL( triggered() ), this, SLOT( OnUIAddGame() ) );
		action->setIcon( GetIcon( type ) );
	}
}

UInt32 CViewGame::GetIconIdFromType( const Sb::CTypeInfo& info ) const
{
	UInt32 iconId = 0;
	return iconId;
}

QIcon CViewGame::GetIcon( const Sb::CTypeInfo& type ) const
{
	return m_IconTypeTable.GetIcon( GetIconIdFromType( type ) );
}

void CViewGame::OnUIAddGame( )
{
	CBank* currentBank = GetApp().GetProjectCtrl().GetCurrentBank();
	if( !currentBank )
	{
		QMessageBox::warning( this, "Warning", "There is no active bank, please add one before" );
		return;
	}

	QAction* senderAction = (QAction*) sender();

	QString fullTypeName = "Dune::"+senderAction->text();
	QByteArray assetType = fullTypeName.toLocal8Bit();
	const Sb::CTypeInfo& assetTI = CBankObject::GetStaticTypeInfo().GetDerivedClass( assetType.data() );

	//COMMAND: AddObject
	CCmd* cmd = NEWOBJ( CCmdAddObject, ( GetApp().GetProjectCtrl(), GetProject()->GetRootGame(), assetTI ) );
	GetApp().GetCommandProcessor().Submit( *cmd );

}

void CViewGame::OnUIShowAddGameMenu()
{
	QMenu menu;
	ConfigureAddGameMenu( menu );
	menu.exec( QCursor::pos() );
}

void CViewGame::OnUISelectLibraryObject( CLibraryObject& obj )
{
	CCtrlProject& ctrl = GetApp().GetProjectCtrl(); 
	DebugAssert( ctrl.GetCurrentBank() );

	CCmdMergeBank* mergeBank = NEWOBJ( CCmdMergeBank, ( ctrl, obj.GetBank(), *ctrl.GetCurrentBank(), ctrl.GetProject()->GetRootGame() ) );	
	GetApp().GetCommandProcessor().Submit( *mergeBank );	
}

CViewGameTree&	CViewGame::GetTree()
{
	return *m_Tree;
}