#include <Dune\Editor\Widget\ObjectLibrary.h> 

#include <QtCore\QDiriterator>

using namespace Dune;

CObjectLibrary::CObjectLibrary( CApplication& app, String name, const Sb::CTypeInfo& objType  ) :
    QDockWidget( name, &app.GetMainFrame() ),
	CEventClient( app )
{
	m_ObjectType = &objType;

	QWidget* content = new QWidget();

	m_ToolBar = new QToolBar( );
	m_List = new QTreeWidget( );

	QStringList header;
	header << "Name" << "Description" << "Actions";
	m_List->setHeaderLabels( header );

	m_List->setVerticalScrollMode( QAbstractItemView::ScrollPerPixel );
	m_List->setIconSize( QSize( 128, 128 ) );

	QLineEdit* filterInput = new QLineEdit();

	m_ToolBar->addWidget( filterInput );

	QVBoxLayout* layout = new QVBoxLayout();

	layout->setContentsMargins( 0, 0, 0, 0 );
	layout->addWidget( m_ToolBar );
	layout->addWidget( m_List );
	content->setLayout( layout );

	setWidget( content );

	setMinimumSize( QSize( 600, 300 ) );

	connect( &m_SaveObjSignalMapper, SIGNAL( mapped( int ) ), this, SLOT( OnUISaveObj( int ) ) );
	connect( &m_AddObjSignalMapper, SIGNAL( mapped( int ) ), this, SLOT( OnUIAddObj( int ) ) );

	m_FileWatcher = NULL;
	
}

CObjectLibrary::~CObjectLibrary()
{

}

void CObjectLibrary::OnUISaveObj( int id )
{
	STreeItem* treeItem = (STreeItem*)id;

	CLibraryObject obj;
	if( !obj.Load( treeItem->ItemPath ) )
		return;

	obj.SetDescription( treeItem->TextEditText->document()->toPlainText().toLocal8Bit().constData() );

	obj.Save();
	
}

void CObjectLibrary::OnUIAddObj( int id )
{
	STreeItem* treeItem = (STreeItem*)id;
	Sb::CRef<CLibraryObject> obj = NEWOBJ( CLibraryObject, () );
	if( obj->Load( treeItem->ItemPath ) == FALSE )
		return;

	Q_EMIT OnObjectSelected( *obj );
}

Void CObjectLibrary::AppendItem( const QFileInfo& file )
{
	CLibraryObject obj;

	if( ! obj.Load( Sb::CFilePath( file.absoluteFilePath().toLocal8Bit().constData() ) ) )
		return;

	if( !obj.GetObject()->GetTypeInfo().IsKindOf( *m_ObjectType ) )
		return;

	STreeItem* item = new STreeItem();
	item->setText( 0, obj.GetName() );

	QImage image( obj.GetThumbRawData().GetBuffer(), 128, 128, QImage::Format_ARGB32 );
	QPixmap pixmap = QPixmap::fromImage( image );
	QIcon icon( pixmap );
	item->setIcon( 0, icon );
	item->setText( 1, obj.GetDescription().GetBuffer() );
	//item->setFlags( Qt::ItemIsEditable | item->flags() );
	item->ItemPath = obj.GetFilePath();

	QWidget* actionWidget = new QWidget;
	QHBoxLayout* vboxLayout = new QHBoxLayout;
	QPushButton* addObjBtn = new QPushButton( "Add" );
	QPushButton* saveObjBtn = new QPushButton( "Save" );

	connect(addObjBtn, SIGNAL( clicked() ), &m_AddObjSignalMapper, SLOT( map() ) );
	connect(saveObjBtn, SIGNAL( clicked() ), &m_SaveObjSignalMapper, SLOT( map() ) );
	m_AddObjSignalMapper.setMapping( addObjBtn, (int) item );
	m_SaveObjSignalMapper.setMapping( saveObjBtn, (int) item );

	item->TextEditText = new QTextEdit;
	item->TextEditText->setMaximumHeight( 128 );

	vboxLayout->addWidget( addObjBtn );
	vboxLayout->addWidget( saveObjBtn );
	actionWidget->setLayout( vboxLayout );

	m_List->invisibleRootItem()->addChild( item );
	m_List->setItemWidget( item, 2, actionWidget );
	m_List->setItemWidget( item, 1, item->TextEditText );
}

Void CObjectLibrary::OnProjectLoaded( CProject& p )
{
	CEventClient::OnProjectLoaded( p );

	if( m_FileWatcher )
		delete m_FileWatcher;

	m_FileWatcher = new QFileSystemWatcher( this );
	connect( m_FileWatcher, SIGNAL( directoryChanged ( const QString& ) ), this, SLOT( OnPathContentChanged( const QString& ) ) );

	CProjectSettings::PathList::Iterator it;
	ForEachItem( it, p.GetSettings().GetAssetPathList() )
	{
		m_FileWatcher->addPath( it->GetBuffer() );
	}
	Populate( p );
}

void CObjectLibrary::OnPathContentChanged( const QString& dir )
{

}

Void CObjectLibrary::Populate( CProject& p )
{
	CProjectSettings::PathList::Iterator it;
	const CProjectSettings::PathList& pathList = p.GetSettings().GetAssetPathList();

	ForEachItem( it, pathList )
	{
		Sb::CFilePath path = (*it);

		if( path.IsRelative() )
		{
			path = Sb::CFilePath( p.GetProjectDirectory() );
			path.AppendRelativePath( (*it) );
		}

		if( path.Exist() )
		{
			QDirIterator dirIterator( path.GetBuffer(), QDir::Files| QDir::NoDotAndDotDot, QDirIterator::Subdirectories );
			while (dirIterator.hasNext()) 
			{
				QFileInfo fleInfo( dirIterator.next() );
				AppendItem( fleInfo );
			}
		}
	}

	m_List->resizeColumnToContents( 0 );
	m_List->resizeColumnToContents( 1 );
	m_List->resizeColumnToContents( 2 );
}