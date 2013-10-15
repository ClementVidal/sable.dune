#include <Dune\Editor\Widget\ObjectPicker.h> 

#include <Dune\Model\Project\Project.h>
#include <Dune\Model\Asset\Asset.h>
#include <Dune\Editor\Application\Application.h>

using namespace Dune;

CWidgetObjectPicker::CWidgetObjectPicker( CApplication& app, const CProject& prj, const SConfig* config) :
    QDialog( &app.GetMainFrame(), Qt::Tool  )
{
    setWindowTitle( "Object picker" );

    m_App = &app;
    m_Project = &prj;

	QVBoxLayout* mainLayout = new QVBoxLayout();
	QHBoxLayout* bottomLayout = new QHBoxLayout();

	m_ToolBar = new QToolBar( this );

	QStringList header;
	header << "Name" << "Type";
	m_TreeCtrl = new QTreeWidget( this );
	m_TreeCtrl->setHeaderLabels( header );
    m_TreeCtrl->setIndentation( 0 );
	connect( m_TreeCtrl, SIGNAL( itemSelectionChanged() ), this, SLOT( OnUISelectionChanged() ) );
	connect( m_TreeCtrl, SIGNAL( itemDoubleClicked( QTreeWidgetItem*, int ) ), this, SLOT( OnUIItemDoubleClicked( QTreeWidgetItem*, int ) ) );

	m_OkButton = new QPushButton( this );
	m_OkButton->setText( "OK" );
	connect( m_OkButton, SIGNAL( clicked() ), this, SLOT( OnUIOk() ) );
	m_CancelButton = new QPushButton( this );
	m_CancelButton->setText( "Cancel" );
	connect( m_CancelButton, SIGNAL( clicked() ), this, SLOT( OnUICancel() ) );

	bottomLayout->addWidget( m_OkButton );
	bottomLayout->addWidget( m_CancelButton );

	m_AssetTypeCombo = new QComboBox( );
	connect( m_AssetTypeCombo, SIGNAL( currentIndexChanged(const QString &) ), this, SLOT( OnUIFilterChange(const QString &) ) );
	QLabel* comboStaticText = new QLabel( m_ToolBar );
	comboStaticText->setText( "Filter by type: " );

	m_ToolBar->addWidget( comboStaticText );
	m_ToolBar->addWidget( m_AssetTypeCombo );

	mainLayout->addWidget( m_ToolBar );
	mainLayout->addWidget( m_TreeCtrl );
	mainLayout->addLayout( bottomLayout );
	mainLayout->setContentsMargins( 0, 0, 0, 0 );
	setLayout( mainLayout );

    if( config && config->Filter.GetItemCount() > 0 )
    {
        PopulateTree( config->Filter[0]->GetTypeName() );
    }
    else
    {
        PopulateTree( "*" );
    }

	PopulateFilter( config );

    m_Asset.SetNull();
	setWindowTitle( "Pick object");

	m_TreeCtrl->resizeColumnToContents( 0 );
}

CWidgetObjectPicker::~CWidgetObjectPicker()
{

}

QSize CWidgetObjectPicker::sizeHint () const
{
	return QSize( 350, 300 );
}

Bool CWidgetObjectPicker::OnUIOk()
{
	done( 1 );
	return TRUE;
}

Bool CWidgetObjectPicker::OnUICancel()
{
	done( 0 );
	return TRUE;
}

Bool CWidgetObjectPicker::OnUIFilterChange( const QString & text )
{
	m_TreeCtrl->clear();

	QByteArray arr = text.toLocal8Bit();
	PopulateTree( arr.data() );
	return TRUE;
}

Bool CWidgetObjectPicker::OnUIItemDoubleClicked( QTreeWidgetItem* i , int c )
{
    CTreeItemData* data = (CTreeItemData*)i;

    if( data )
        m_Asset = data->Asset;
    else 
        m_Asset.SetNull();

    done( 1 );

	return TRUE;
}

Bool CWidgetObjectPicker::OnUISelectionChanged( )
{
    if( m_TreeCtrl->selectedItems().count() == 0 )
		return FALSE;

    CTreeItemData* data = (CTreeItemData*)m_TreeCtrl->selectedItems().first();

    if( data )
        m_Asset = data->Asset;
    else 
        m_Asset.SetNull();

	return TRUE;
}

const CHandle& CWidgetObjectPicker::GetAsset() const
{
    return m_Asset;
}

CWidgetObjectPicker::CTreeItemData::CTreeItemData( const CHandle& asset )
{
    Asset = asset;
}

Void CWidgetObjectPicker::PopulateFilter( const SConfig* config )
{
    Int32 itemCount = 0, i;
    QStringList comboValue;
    QString initialValue;

    if( config )
    {
        itemCount = config->Filter.GetItemCount();
        if( itemCount > 64 )
        {
            Sb::DebugLogError( "CWidgetObjectPicker::PopulateFilter - No enough room in filter combo box");
            itemCount = 64;
        }

		if( itemCount == 0 )
		{
			comboValue.push_back( "*" );
			itemCount++;
		}
		else
		{
			for( i=0;i<itemCount;i++ )
			{
				comboValue.push_back( config->Filter[i]->GetTypeName() );
			}
		}
    }
    else
    {
        itemCount = CAsset::GetStaticTypeInfo().GetDerivedClassCount();
        if( itemCount > 64 )
        {
            Sb::DebugLogError( "CWidgetObjectPicker::PopulateFilter - No enough room in filter combo box");
            itemCount = 64;
        }
        for( i=0;i<itemCount;i++ )
        {
            comboValue.push_back( CAsset::GetStaticTypeInfo().GetDerivedClass( i ).GetTypeName() );
        }

		comboValue.push_back( "*" );
    }

	m_AssetTypeCombo->addItems( comboValue );
}

Void CWidgetObjectPicker::PopulateTree( String typeName )
{
    QTreeWidgetItem* rootItem;

    Sb::CList<CHandle>::Iterator it;
    Sb::CList<CHandle> result;
    CProject* prj = NULL;
    CCtrlProject* ctrlPrj = &GetApp().GetProjectCtrl();

    if( ctrlPrj )
    {
        prj = ctrlPrj->GetProject();
        if( prj )
        {
            Char query[128];
            if( Sb::StringCompare( typeName, "*" ) == 0 )
				Sb::StringSetFormattedText( query, 128, "IsKindOf(%s)", "Dune:CAsset" );
            else
                Sb::StringSetFormattedText( query, 128, "IsKindOf(%s)", typeName );

            GetApp().GetProject()->IssueQuery(query, result );

            rootItem = m_TreeCtrl->invisibleRootItem();

            ForEachItem( it, result )
            {
                CBankObject* bankObj = GetApp().GetProject()->GetObj( *it );
                
				CTreeItemData* item = new CTreeItemData( bankObj->GetHandle() );
				item->setText( 0, bankObj->GetName() );
				item->setText( 1, bankObj->GetTypeInfo().GetTypeName() );
				rootItem->addChild( item );
            }
        }
    }
}

CApplication& CWidgetObjectPicker::GetApp()
{
    return *m_App;
}
