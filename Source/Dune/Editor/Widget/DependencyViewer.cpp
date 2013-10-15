#include <Dune\Editor\Widget\DependencyViewer.h> 

#include <Dune\Model\Bank\Object.h>
#include <Sable\Core\Collection\Map.h>

#include <QtGui\QVBoxLayout>

using namespace Dune;

CDependencyViewer::CDependencyViewer( CApplication& app, const CBankObject& obj ) :
    QDialog( &app.GetMainFrame(), Qt::Tool )
{
    m_App = &app;
    m_Object = &obj;

    QVBoxLayout *layout = new QVBoxLayout( );
    QToolBar* toolbar = new QToolBar();

    setWindowTitle( "Dependency viewer" );

    QAction* action = NULL;
    action = toolbar->addAction( "Out" );
    action->setChecked( TRUE );
    connect( action, SIGNAL( triggered() ), this, SLOT( OnUIDisplayOutputDependency() ) );

    action = toolbar->addAction( "In" );
    action->setChecked( FALSE );
    connect( action, SIGNAL( triggered() ), this, SLOT( OnUIDisplayInputDependency() ) );

    m_TreeCtrl = new QTreeWidget( this );
	QHBoxLayout* okCancelLayout = new QHBoxLayout(); 
    QPushButton* okButton = new QPushButton( this );
    okButton->setText( "Ok" );
    connect( okButton, SIGNAL( clicked() ), this, SLOT( OnUIOk() ) );
    connect( m_TreeCtrl, SIGNAL( itemExpanded( QTreeWidgetItem* ) ), this, SLOT( OnUIExpandOrCollapse( QTreeWidgetItem* ) ) );
    connect( m_TreeCtrl, SIGNAL( itemCollapsed( QTreeWidgetItem* ) ), this, SLOT( OnUIExpandOrCollapse( QTreeWidgetItem* ) ) );
    okCancelLayout->addStretch( 1 );
	okCancelLayout->addWidget( okButton );

    layout->addWidget( toolbar );
    layout->addWidget( m_TreeCtrl );
    layout->addLayout( okCancelLayout );
    layout->setContentsMargins( 0,0,0,0 );

    resize( 500, 300 );
    setLayout( layout );

	QStringList header;
	header << "Name" << "Type" << "RefCount" << "Handle";
	m_TreeCtrl->setHeaderLabels( header );

    m_TreeCtrl->clear();
    Populate( *m_Object, *m_TreeCtrl->invisibleRootItem(), TRUE );

	m_TreeCtrl->invisibleRootItem()->setExpanded( TRUE );

    m_TreeCtrl->resizeColumnToContents( 0 );
	m_TreeCtrl->resizeColumnToContents( 1 );
	m_TreeCtrl->resizeColumnToContents( 2 );
}

CDependencyViewer::~CDependencyViewer()
{

}

Bool CDependencyViewer::OnUIDisplayOutputDependency()
{
    m_TreeCtrl->clear();
    Populate( *m_Object, *m_TreeCtrl->invisibleRootItem(), TRUE );
    return TRUE;
}

Bool CDependencyViewer::OnUIDisplayInputDependency()
{
    m_TreeCtrl->clear();
    Populate( *m_Object, *m_TreeCtrl->invisibleRootItem(), FALSE );
    return TRUE;
}

Bool CDependencyViewer::OnUIExpandOrCollapse( QTreeWidgetItem* item )
{
    m_TreeCtrl->resizeColumnToContents( 0 );
    m_TreeCtrl->resizeColumnToContents( 1 );
    return TRUE;
}

Bool CDependencyViewer::OnUIOk()
{
    done( 1 );
    return TRUE;
}

CApplication& CDependencyViewer::GetApp()
{
    return *m_App;
}

Void CDependencyViewer::Populate( const CBankObject& obj, QTreeWidgetItem& parent, Bool outputOrInput )
{
	const CBankObject::DependencyList* list = NULL;
    
    if( outputOrInput )
        list = &obj.GetOutputDependencyList();
    else 
        list = &obj.GetInputDependencyList();

	CBankObject::DependencyList::Iterator it;

	QTreeWidgetItem* group = NULL;
	QTreeWidgetItem* item = NULL;
	ForEachItem( it, *list )
	{
		CBankObject* childObj = GetApp().GetProject()->GetObj( (*it).Handle );
		if( childObj )
		{
			const Sb::CTypeInfo& ti = childObj->GetTypeInfo();
		
			item = new QTreeWidgetItem();
			item->setText( 0, childObj->GetName()  );
			item->setText( 1, ti.GetTypeName()  );
			item->setText( 2, QString::number( it->Count ) );
			item->setText( 3, CLexicalCast::ValueToString( childObj->GetHandle() ).GetBuffer() );
			parent.addChild( item );
			Populate( *childObj, *item, outputOrInput );
		}
	}
}
