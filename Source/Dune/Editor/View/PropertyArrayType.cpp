#include <Dune\Editor\View\PropertyArrayType.h> 

#include <Dune\Common\Log.h>

#include <QtGui\QColorDialog>

using namespace Dune;

CViewPropertyArray::CPropertyProxy::CPropertyProxy( CViewPropertyBase& p )
{
    RemoveButton = new QPushButton();
    RemoveButton->setText( "-" );
    RemoveButton->setMaximumSize( QSize( 10, 10 ) );
	RemoveButton->setToolTip( "Remove item" );

    Property = &p;

    QHBoxLayout* layout = new QHBoxLayout;
    
    layout->addWidget( RemoveButton );
    layout->addWidget( &p );
    layout->setContentsMargins( 0,0,0,0 );

    setLayout( layout );

}

CViewPropertyArray::CViewPropertyArray( CApplication* app, CPropertyArray* p ) :
    CViewPropertyBase( app, p )
{
    delete m_ValueLineEdit;
    m_ValueLineEdit = NULL;

    m_AddButton = new QPushButton();
    m_AddButton->setText( "Add" );
    m_AddButton->setMaximumWidth( 75 );

    m_ClearButton = new QPushButton();
    m_ClearButton->setText( "Clear" );
    m_ClearButton->setMaximumWidth( 75 );

	m_CountLabel = new QLabel();
	m_CountLineEdit = new QLineEdit();

	m_CountLabel->setText("Size: ");
	m_CountLineEdit->setText( QString::number( p->GetItemCount() ) );
	m_CountLineEdit->setEnabled( FALSE );

    m_Layout->addWidget( m_AddButton );
	m_Layout->addWidget( m_ClearButton );
	m_Layout->addWidget( m_CountLabel );
	m_Layout->addWidget( m_CountLineEdit );
}

CViewPropertyArray::~CViewPropertyArray()
{

}

Void CViewPropertyArray::Initialize( QTreeWidget& tree, QTreeWidgetItem& root, QWidget* widgetToAdd )
{
    CViewPropertyBase::Initialize( tree, root, widgetToAdd );

    Bool disabled = !m_Property->GetIsEnabled();

    CPropertyArray* p = (CPropertyArray*) m_Property;

    UInt32 i=0;
    for(i=0;i<p->GetItemCount();i++)
    {
        CViewPropertyBase* child = CViewPropertyBase::CreateViewFromModel( m_App, p->GetItemAtIndex( i ) );
       
        CPropertyProxy* proxy = new CPropertyProxy( *child );

        proxy->Property->Initialize( tree, *TreeItem, proxy );

        if( disabled )
            proxy->setEnabled( FALSE );

	}

	m_CountLineEdit->setText( QString::number( p->GetItemCount() ) );
}
