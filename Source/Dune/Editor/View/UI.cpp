#include <Dune/Editor/View/UI.h> 

#include <Dune/Resources/Icons/ElementView.xpm>


using namespace Dune;

CViewUI::CViewUI( CViewMainFrame& mainFrame ) :
	QDockWidget( "UI Browser", &mainFrame ),
	m_IconToolsTable( (const char**)AssetTools_xpm ),
	m_IconTypeTable( (const char**)ElementView_xpm ),
	CEventClient( mainFrame.GetApp() )
{
	QWidget* content = new QWidget();
	m_ToolBar = new QToolBar( this );
	m_ToolBar->setIconSize( QSize( 16, 16 ) );

	m_ObjectLibrary = new CObjectLibrary( GetApp(), "Gui library", CWidget::GetStaticTypeInfo()  );
	m_ObjectLibrary->setFloating( TRUE );
	m_ObjectLibrary->hide();
	GetApp().GetMainFrame().addDockWidget( Qt::AllDockWidgetAreas, m_ObjectLibrary );
	connect( m_ObjectLibrary, SIGNAL( OnObjectSelected( CLibraryObject& ) ), this, SLOT( OnUISelectLibraryObject( CLibraryObject& ) ) );

	QAction* objLibToggleAction = m_ObjectLibrary->toggleViewAction();
	objLibToggleAction->setIcon( m_IconToolsTable.GetIcon( 0 ) );
	objLibToggleAction->setToolTip( "Open gui library" );
	GetApp().GetMainFrame().GetViewMenu().addAction( objLibToggleAction );

	m_ToolBar->addAction( objLibToggleAction );

	QAction* toolBarAction = m_ToolBar->addAction( m_IconToolsTable.GetIcon( 1 ), "Add UI" );
	toolBarAction->setToolTip( "Add UI" );
	connect( toolBarAction, SIGNAL( triggered( ) ), this, SLOT( OnUIShowAddMenu( ) ) );

    QVBoxLayout* layout = new QVBoxLayout();

	m_Tree = new CViewUITree( *this, GetApp() );	
	layout->addWidget( m_ToolBar );
	layout->addWidget( m_Tree );

	layout->setContentsMargins( 0, 0, 0, 0 );
	content->setLayout( layout );

	setWidget( content );
}

CViewUI::~CViewUI()
{

}

void CViewUI::OnUIShowAddMenu()
{
	QMenu menu;
	ConfigureAddMenu( menu );
	menu.exec( QCursor::pos() );
}

Bool CViewUI::HasIcon( const Sb::CTypeInfo& type ) const
{
	return TRUE;
}

UInt32 CViewUI::GetIconIdFromType( const Sb::CTypeInfo& info ) const
{
	UInt32 iconId = 0;

	if( info == CWidgetFrame::GetStaticTypeInfo() )
	{
		iconId = 3;
	}
	else if( info == CLayoutVBox::GetStaticTypeInfo() )
	{
		iconId = 2;
	}
	else if( info == CLayoutHBox::GetStaticTypeInfo() )
	{
		iconId = 1;
	}
	else if( info == CWidget::GetStaticTypeInfo() )
	{
		iconId = 4;
	}
	else if( info == CWidgetText::GetStaticTypeInfo() )
	{
		iconId = 5;
	}
	return iconId;
}

QIcon CViewUI::GetIcon( const Sb::CTypeInfo& type ) const
{
	return m_IconTypeTable.GetIcon( GetIconIdFromType( type ) );
}

Void CViewUI::ConfigureAddMenu( QMenu& subMenu )
{
	{
		const Sb::CTypeInfo& type = CBankObjectDirectory::GetStaticTypeInfo();
		QAction* action = subMenu.addAction( type.GetTypeName() );
		connect( action, SIGNAL( triggered() ), this, SLOT( OnUIAddElement() ) );
		action->setIcon( GetIcon( type ) );
	}
	subMenu.addSeparator();
	{
		const Sb::CTypeInfo& type = CWidget::GetStaticTypeInfo();
		QAction* action = subMenu.addAction( type.GetTypeName() );
		connect( action, SIGNAL( triggered() ), this, SLOT( OnUIAddElement() ) );
		action->setIcon( GetIcon( type ) );
	}
	{
		const Sb::CTypeInfo& type = CWidgetFrame::GetStaticTypeInfo();
		QAction* action = subMenu.addAction( type.GetTypeName() );
		connect( action, SIGNAL( triggered() ), this, SLOT( OnUIAddElement() ) );
		action->setIcon( GetIcon( type ) );
	}
	{
		const Sb::CTypeInfo& type = CWidgetText::GetStaticTypeInfo();
		QAction* action = subMenu.addAction( type.GetTypeName() );
		connect( action, SIGNAL( triggered() ), this, SLOT( OnUIAddElement() ) );
		action->setIcon( GetIcon( type ) );
	}
	{
		const Sb::CTypeInfo& type = CLayoutHBox::GetStaticTypeInfo();
		QAction* action = subMenu.addAction( type.GetTypeName() );
		connect( action, SIGNAL( triggered() ), this, SLOT( OnUIAddElement() ) );
		action->setIcon( GetIcon( type ) );
	}
	{
		const Sb::CTypeInfo& type = CLayoutVBox::GetStaticTypeInfo();
		QAction* action = subMenu.addAction( type.GetTypeName() );
		connect( action, SIGNAL( triggered() ), this, SLOT( OnUIAddElement() ) );
		action->setIcon( GetIcon( type ) );
	}
}

void CViewUI::OnUISelectLibraryObject( CLibraryObject& obj )
{
	CCtrlProject& ctrl = GetApp().GetProjectCtrl(); 
	DebugAssert( ctrl.GetCurrentBank() );

	CCmdMergeBank* mergeBank = NEWOBJ( CCmdMergeBank, ( ctrl, obj.GetBank(), *ctrl.GetCurrentBank(), ctrl.GetProject()->GetRootUI() ) );	
	GetApp().GetCommandProcessor().Submit( *mergeBank );	
}

void CViewUI::OnUIAddElement( )
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
	CCmd* cmd = NEWOBJ( CCmdAddObject, ( GetApp().GetProjectCtrl(), GetProject()->GetRootUI(), assetTI ) );
	GetApp().GetCommandProcessor().Submit( *cmd );

}
