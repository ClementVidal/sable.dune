#include <Dune/Editor/Widget/GameObjectEditor.h> 

#include <Sable/Game/Service/Service.h>

using namespace Dune;

CGameObjectEditor::CGameObjectEditor( CApplication& app, CGameObjectPrototype& proto ) :
    QDialog( & app.GetMainFrame(), Qt::Tool  )
{
	setWindowTitle( "Game object Editor" );
    m_App = &app;
	m_GameObjectProto = NULL;
	m_ProcessItemChanged = TRUE;

	SetGameObjectPrototype( proto );

	// Editor
	QHBoxLayout* editorLayout = new QHBoxLayout();
	QVBoxLayout* serviceAndParamLayout = new QVBoxLayout();
	m_ObjectTree = new QTreeWidget();
	
	connect( m_ObjectTree, SIGNAL( itemChanged( QTreeWidgetItem*, int ) ), this, SLOT( OnUIItemChanged( QTreeWidgetItem*, int ) ) );
	connect( m_ObjectTree, SIGNAL( itemSelectionChanged() ), this, SLOT( OnUISelectionChanged() ) );

	// Parameter
	QGroupBox* parameterGB = new QGroupBox( "Parameters:" );

	// Parameter type
	m_ParameterTypeList = new QListWidget();
	m_ParameterTypeList->setMaximumHeight( 100 );
	QPushButton* addParamType = new QPushButton( "Add");
	connect( addParamType, SIGNAL( clicked() ), this, SLOT( OnUIAddParam() ) );
	m_RemoveParam = new QPushButton( "Remove");
	connect( m_RemoveParam, SIGNAL( clicked() ), this, SLOT( OnUIRemoveParam() ) );
	QVBoxLayout* paramTypeLayout = new QVBoxLayout();
	paramTypeLayout->addWidget( new QLabel( "Parameter type" ) );
	paramTypeLayout->addWidget( m_ParameterTypeList );
	QHBoxLayout* addRemoveParamLayout = new QHBoxLayout();
	addRemoveParamLayout->addStretch( 1 );
	addRemoveParamLayout->addWidget( addParamType );
	addRemoveParamLayout->addWidget( m_RemoveParam );
	paramTypeLayout->addLayout( addRemoveParamLayout );

	parameterGB->setLayout( paramTypeLayout );

	// Service
	QGroupBox* serviceGB = new QGroupBox( "Services:" );

	// service type
	m_ServiceTypeList = new QListWidget();
	m_ServiceTypeList->setMaximumHeight( 100 );
	QPushButton* addServiceType = new QPushButton( "Add");
	connect( addServiceType, SIGNAL( clicked() ), this, SLOT( OnUIAddService() ) );
	m_RemoveService = new QPushButton( "Remove");
	connect( m_RemoveService, SIGNAL( clicked() ), this, SLOT( OnUIRemoveService() ) );
	QVBoxLayout* serviceTypeLayout = new QVBoxLayout();
	serviceTypeLayout->addWidget( new QLabel( "Service type" ) );
	serviceTypeLayout->addWidget( m_ServiceTypeList );
	QHBoxLayout* addRemoveServiceLayout = new QHBoxLayout();
	addRemoveServiceLayout->addStretch( 1 );
	addRemoveServiceLayout->addWidget( addServiceType ) ;
	addRemoveServiceLayout->addWidget( m_RemoveService ) ;
	serviceTypeLayout->addLayout( addRemoveServiceLayout );

	serviceGB->setLayout( serviceTypeLayout );

	// Group service and parameters
	serviceAndParamLayout->addStretch( 1 );
	serviceAndParamLayout->addWidget( parameterGB );
	serviceAndParamLayout->addWidget( serviceGB );

	// Add tree 
	editorLayout->addLayout( serviceAndParamLayout );
	editorLayout->addWidget( m_ObjectTree );

	// Ok Cancel
	QHBoxLayout* okCancelLayout = new QHBoxLayout();
	QPushButton* okBtn = new QPushButton( "OK" );
	connect( okBtn, SIGNAL( clicked() ), this, SLOT( OnUIOk() ) );
	QPushButton* cancelBtn = new QPushButton( "Cancel" );
	connect( cancelBtn, SIGNAL( clicked() ), this, SLOT( OnUICancel() ) );

	okCancelLayout->addStretch( 1 );
	okCancelLayout->addWidget( okBtn );
	okCancelLayout->addWidget( cancelBtn );

	// setup layout
	QVBoxLayout* vLayout = new QVBoxLayout();

	vLayout->addLayout( editorLayout );
	vLayout->addLayout( okCancelLayout );

	vLayout->setContentsMargins( 0, 0, 0, 0 );

	setLayout( vLayout );

	m_RemoveService->setEnabled( FALSE );
	m_RemoveParam->setEnabled( FALSE );

	Populate();
}

CGameObjectEditor::~CGameObjectEditor()
{
	if( m_GameObjectProto)
	{
    //    if( m_GameObjectProto->GetIsEdited() )
    //       if m_GameObjectProto->SetIsEdited( FALSE );
	}
}

void CGameObjectEditor::OnUIItemChanged( QTreeWidgetItem* item, int column )
{
	if( !m_ProcessItemChanged )
		return;

	Int32 maxItemId = -1;
	if( item->text(0).size() == 0 )
	{
		m_ProcessItemChanged = FALSE;
		item->setText( 0, "Parameter" );
		m_ProcessItemChanged = TRUE;
	}

	if( item->parent() == m_ParameterNode && column == 0 )
	{
		QString rootItemText = item->text(0);
		QRegExp regExp( "(.*[^\\d])(\\d*)" );
		if( regExp.exactMatch( item->text( 0 ) ) )
		{
			rootItemText = regExp.cap(1);
		}

		for( Int32 i = 0; i<m_ParameterNode->childCount(); i++ )
		{
			if( regExp.exactMatch( m_ParameterNode->child( i )->text( 0 ) ) && m_ParameterNode->child( i ) != item  )
			{
				if( regExp.cap(1) == rootItemText )
				{
					Int32 itemId = regExp.cap(2).toLong();
					if( maxItemId < itemId+1 )
						maxItemId = itemId+1;
				}
			}
		}
	
		if( maxItemId > -1 )
		{
			m_ProcessItemChanged = FALSE;
			item->setText( 0, rootItemText + QString::number( maxItemId ) );
			m_ProcessItemChanged = TRUE;
		}
	}
}

Void CGameObjectEditor::Populate()
{
	m_TypeList.push_back( "Int" );
	m_TypeList.push_back( "Float" );
	m_TypeList.push_back( "Bool" );
	m_TypeList.push_back( "Vector2f" );
	m_TypeList.push_back( "Vector3f" );
	m_TypeList.push_back( "Vector4f" );

	for( Int32 i=0;i<m_TypeList.size(); i++ )
	{
		m_ParameterTypeList->addItem( m_TypeList[i] );
	}

	const Sb::CTypeInfo& type = Sb::CGameService::GetStaticTypeInfo();
	for( Int32 i = 0; i<type.GetDerivedClassCount(); i++ )
	{
		m_ServiceTypeList->addItem( type.GetDerivedClass( i ).GetTypeName() );
	}

	// Populate tree
	m_RootNode = new QTreeWidgetItem( );
	m_RootNode->setText( 0, m_GameObjectProto->GetName() );
	m_RootNode->setFlags( Qt::ItemIsEnabled );

	m_ParameterNode = new QTreeWidgetItem( );
	m_ParameterNode->setText( 0, "Parameters" );
	m_ParameterNode->setFlags( Qt::ItemIsEnabled );

	m_ServiceNode = new QTreeWidgetItem(  );
	m_ServiceNode->setText( 0, "Services" );
	m_ServiceNode->setFlags( Qt::ItemIsEnabled );

	m_RootNode->addChild( m_ParameterNode );
	m_RootNode->addChild( m_ServiceNode );

	QStringList header;
	header << "Name" << "Type";
	m_ObjectTree->setHeaderLabels( header );
	m_ObjectTree->setItemsExpandable( FALSE );
	m_ObjectTree->invisibleRootItem()->addChild( m_RootNode );
	m_ObjectTree->setHeaderHidden ( TRUE );
	m_ObjectTree->expandAll();
	m_ObjectTree->setIndentation( 10 );
}

void CGameObjectEditor::OnUISelectionChanged()
{
	if(  m_ObjectTree->selectedItems().size() == 0 )
	{
		m_RemoveParam->setEnabled( FALSE );
		m_RemoveService->setEnabled( FALSE );
		return;
	}

	QTreeWidgetItem* selectedItem = m_ObjectTree->selectedItems().first();
	
	if( selectedItem->parent() == m_ParameterNode )
	{
		m_RemoveParam->setEnabled( TRUE );
		m_RemoveService->setEnabled( FALSE );
	}
	else if( selectedItem->parent() == m_ServiceNode )
	{
		m_RemoveParam->setEnabled( FALSE );
		m_RemoveService->setEnabled( TRUE );
	}
}

void CGameObjectEditor::OnUIAddParam()
{
	QListWidgetItem* selectedItem = m_ParameterTypeList->selectedItems().first();
	String paramName = "New param";
	if( selectedItem )
	{
		Int32 i = 0;
		for( i = 0;i<m_TypeList.size(); i++ )
		{
			if( selectedItem->text() == m_TypeList[i] )
			{
				break;
			}
		}

		QComboBox* comboBox = new QComboBox( );
		comboBox->addItems( m_TypeList );
		comboBox->setCurrentIndex( comboBox->findText( m_TypeList[i] ) );

		QTreeWidgetItem* item = new QTreeWidgetItem();
		item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled );
		m_ParameterNode->addChild( item );
		m_ObjectTree->editItem( item, 0 );
		m_ObjectTree->setItemWidget( item, 1, comboBox );
		m_ObjectTree->setColumnWidth( 0, 150 );
	}

	m_ObjectTree->resizeColumnToContents( 0 );
	m_ObjectTree->resizeColumnToContents( 1 );

}

void CGameObjectEditor::OnUIAddService()
{
	QListWidgetItem* selectedItem = m_ServiceTypeList->selectedItems().first();
	if( selectedItem )
	{
		QTreeWidgetItem* item = new QTreeWidgetItem();
		item->setText( 0, selectedItem->text() );
		item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
		m_ServiceNode->addChild( item );
		m_ObjectTree->setColumnWidth( 0, 150 );
		
		m_ObjectTree->resizeColumnToContents( 0 );
	}
}

void CGameObjectEditor::OnUIRemoveParam()
{

}

void CGameObjectEditor::OnUIRemoveService()
{

}

Void CGameObjectEditor::SetGameObjectPrototype( CGameObjectPrototype& proto )
{
	m_GameObjectProto = &proto;
}

void CGameObjectEditor::OnUIOk()
{
	for( Int32 i = 0; i<m_ParameterNode->childCount(); i++ )
	{
		QTreeWidgetItem* item = m_ParameterNode->child( i );

		QComboBox* cb = (QComboBox*)m_ObjectTree->itemWidget( item, 1 );
//		m_GameObjectProto->AddProperty( item->text( 0 ).toLocal8Bit().constData(), (Sb::CGameProperty::EType) cb->currentIndex() );
	}	

	done( 1 );
}

void CGameObjectEditor::OnUICancel()
{
	done( 0 );
}

