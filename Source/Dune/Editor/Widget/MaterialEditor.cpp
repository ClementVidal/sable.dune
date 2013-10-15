#include <Dune\Editor\Widget\MaterialEditor.h> 

#include <Dune\Model\Bank\Object.h>
#include <Sable\Core\Collection\Map.h>

using namespace Dune;

CMaterialEditor::CMaterialEditor( CApplication& app, CAssetMaterial& obj ) :
    QDialog( & app.GetMainFrame(), Qt::Tool  )
{
	setWindowTitle( "Material Editor" );
    m_App = &app;
    m_Object = &obj;

    QVBoxLayout*	layout = new QVBoxLayout( this );
    QHBoxLayout*	bottomLayout = new QHBoxLayout( this );

    m_Table = new QTreeWidget( );
	QStringList header;
	header << "Name" << "Type";
	m_Table->setHeaderLabels( header );

	QPushButton* okBtn = new QPushButton( "OK" );
	connect( okBtn, SIGNAL( clicked() ), this, SLOT( OnUIOk() ) );
	bottomLayout->addWidget( okBtn );

	QPushButton* cancelBtn = new QPushButton( "Cancel" );
	connect( cancelBtn, SIGNAL( clicked() ), this, SLOT( OnUICancel() ) );
	bottomLayout->addWidget( cancelBtn );

	QPushButton* addBtn = new QPushButton( "+" );
	connect( addBtn, SIGNAL( clicked() ), this, SLOT( OnUIAddParameter() ) );
	bottomLayout->addWidget( addBtn );
	
	QPushButton* removeBtn = new QPushButton( "-" );
	connect( removeBtn, SIGNAL( clicked() ), this, SLOT( OnUIRemoveParameter() ) );
	bottomLayout->addWidget( removeBtn );

	layout->addWidget( m_Table );
    layout->addLayout( bottomLayout );
	
	m_Table->setColumnWidth( 0, 150 );
	m_Table->setSelectionMode( QAbstractItemView::ExtendedSelection );
    m_Table->setIndentation( 0 );

    layout->setContentsMargins( 0,0,0,0 );

	setLayout( layout );

	setMinimumSize( QSize( 300, 300 ) );
	resize( QSize( 300, 300 )  );

    Populate();
}

CMaterialEditor::~CMaterialEditor()
{

}

CApplication& CMaterialEditor::GetApp()
{
    return *m_App;
}

Bool CMaterialEditor::OnUIAddParameter()
{
	AddParameter( "NewParameter", m_TypeList[0] );
	return TRUE;
}

Bool CMaterialEditor::OnUIRemoveParameter()
{
	QList<QTreeWidgetItem *> items = m_Table->selectedItems();
	QTreeWidgetItem* item;
	Q_FOREACH( item, items )
	{
		delete item;
	}

	return TRUE;
}

Bool CMaterialEditor::OnUIOk()
{
	UpdateMaterial();
	done( 1 );
	return TRUE;
}

Bool CMaterialEditor::OnUICancel()
{
	done( 0 );
	return TRUE;
}


Void CMaterialEditor::Populate()
{
	CBankObject::PropertyTable::Iterator it;
	const CBankObject::PropertyTable& pTable = m_Object->GetPropertyTable();

	UInt32 i=0;
	m_TypeList.push_back( "Color" );
	m_TypeList.push_back( "Vector3" );
	m_TypeList.push_back( "Vector2" );
	m_TypeList.push_back( "Texture2D" );
	m_TypeList.push_back( "Texture3D" );
	m_TypeList.push_back( "Sampler" );

	i = 0;

	ForEachItem( it, pTable )
	{
		if( Sb::StringCompare(( *it)->GetGroup(), "Parameters" ) == 0 )
		{
			AddParameter( (*it)->GetName(), m_TypeList[ ConvertType( *(*it) ) ] );
		}
	}
}

Void CMaterialEditor::AddParameter( QString name, QString type )
{
	QTreeWidgetItem* root = m_Table->invisibleRootItem();
	QComboBox* comboBox;
	comboBox = new QComboBox( );
	comboBox->setInsertPolicy(  QComboBox::InsertAlphabetically );
	comboBox->addItems( m_TypeList );
	comboBox->setCurrentIndex( comboBox->findText( type ) );

	QTreeWidgetItem* item = new QTreeWidgetItem();
	item->setText( 0, name );
	item->setFlags( item->flags() | Qt::ItemIsEditable );
	root->addChild( item );

	m_Table->setItemWidget( item, 1, comboBox );
	m_Table->setColumnWidth( 0, 150 );
}

UInt32 CMaterialEditor::ConvertType( const CPropertyInterface& p )
{
	if( p.GetTypeInfo().IsKindOf( CPropertyColor::GetStaticTypeInfo() ) )
	{
		return 0;
	}
	else if( p.GetTypeInfo().IsKindOf( CPropertyVector3f::GetStaticTypeInfo() ) )
	{
		return 1;
	}
	else if( p.GetTypeInfo().IsKindOf( CPropertyVector2f::GetStaticTypeInfo() ) )
	{	
		return 2;
	}
	else if( p.GetTypeInfo().IsKindOf( CPropertyHandle::GetStaticTypeInfo() ) )
	{
		const CPropertyHandle& pH = (const CPropertyHandle&)p;

		if( pH.GetFilterList().GetItemCount() )
		{
			if( pH.GetFilterList().GetFirstItem()->IsA( CAssetTexture2D::GetStaticTypeInfo() ) )
			{
				return 3;
			}
			else if( pH.GetFilterList().GetFirstItem()->IsA( CAssetTexture3D::GetStaticTypeInfo() ) )
			{
				return 4;
			}
			else if( pH.GetFilterList().GetFirstItem()->IsA( CAssetStatesSampler::GetStaticTypeInfo() ) )
			{
				return 5;
			}
		}
	}
	return 1;
}


const Sb::CTypeInfo* CMaterialEditor::ConvertType( const QString& type )
{
	if( type == m_TypeList[0] )
	{	
		return &CPropertyColor::GetStaticTypeInfo();
	}
	else if( type == m_TypeList[1] )
	{	
		return &CPropertyVector3f::GetStaticTypeInfo();
	}
	else if( type == m_TypeList[2] )
	{
		return &CPropertyVector2f::GetStaticTypeInfo();
	}
	else if( type == m_TypeList[3] )
	{
		return &CPropertyHandle::GetStaticTypeInfo();
	}
	else if( type == m_TypeList[4] )
	{
		return &CPropertyHandle::GetStaticTypeInfo();
	}
	else if( type == m_TypeList[5] )
	{
		return &CPropertyHandle::GetStaticTypeInfo();
	}
	return NULL;
}

/**
Update parameters according to what is specified in the parameter grid

*/
Void CMaterialEditor::UpdateMaterial()
{	
	QTreeWidgetItem* root = m_Table->invisibleRootItem();
	Int32 i;
	QString name, type;
	QByteArray nameLocal8Bit, typeLocal8Bit;

	CCmdMeta* cmd = NEWOBJ( CCmdMeta, ( Sb::CString( "EditMaterialParameters: " ) + m_Object->GetName() ) );

	// Add any new parameters
	for(i=0;i<root->childCount();i++)
	{
		QTreeWidgetItem* item = root->child( i );
		name = item->text( 0 );
		type = ((QComboBox*)m_Table->itemWidget( item, 1 ))->currentText();
		
		nameLocal8Bit = name.toLocal8Bit();
		CPropertyInterface* p = m_Object->GetProperty( nameLocal8Bit.data() );
		if( p == NULL )
		{
			typeLocal8Bit = type.toLocal8Bit();
			const Sb::CTypeInfo* typeInfo = ConvertType( typeLocal8Bit );
			if( typeInfo )
			{
				p = (CPropertyInterface*)typeInfo->CreateObject( NULL );
				p->Initialize( nameLocal8Bit.data() );
				p->SetGroup( "Parameters" );
				p->SetIsAutoSerialized( FALSE );

				// If we add a texture param add a filter
				if( type == m_TypeList[3] )
				{
					CPropertyHandle* ph = (CPropertyHandle*)p;
					ph->AddFilter( CAssetTexture2D::GetStaticTypeInfo() );
					ph->SetSerializeFilters( TRUE );
				}
				else if( type == m_TypeList[4] )
				{
					CPropertyHandle* ph = (CPropertyHandle*)p;
					ph->AddFilter( CAssetTexture3D::GetStaticTypeInfo() );
					ph->SetSerializeFilters( TRUE );
				}
				else if( type == m_TypeList[5] )
				{
					CPropertyHandle* ph = (CPropertyHandle*)p;
					ph->AddFilter( CAssetStatesSampler::GetStaticTypeInfo() );
					ph->SetSerializeFilters( TRUE );
				}

				cmd->PushCommand( * NEWOBJ( CCmdAddObjectProperty, ( GetApp().GetProjectCtrl(), m_Object->GetHandle(), *p ) ) );
			}
		}
	}

	// Remove unused property
	CBankObject::PropertyTable::Iterator it;
	const CBankObject::PropertyTable& pTable = m_Object->GetPropertyTable();
	Sb::CStack<CPropertyInterface*> toRemove;
	Bool find;
	toRemove.SetCapacity( MathMax( root->childCount(), (Int32)pTable.GetItemCount() ) );

	ForEachItem( it, pTable )
	{
		if( Sb::StringCompare( (*it)->GetGroup(), "Parameters" ) == 0 )
		{
			find = FALSE;
			for(i=0;i<root->childCount();i++)
			{
				QTreeWidgetItem* item = root->child( i );
				name = item->text( 0 );

				nameLocal8Bit = name.toLocal8Bit();

				if( Sb::StringCompare( nameLocal8Bit.data(), (*it)->GetName() ) == 0 )
				{
					find = TRUE;
					break;
				}
			}
			// If we do not find the property in the table, it mean that we must delete it
			if( !find )
				toRemove.PushItem( (*it) );
		}
	}

	for( i=0;i< (Int32)toRemove.GetItemCount(); i++ )
	{
		cmd->PushCommand( * NEWOBJ( CCmdRemoveObjectProperty, ( GetApp().GetProjectCtrl(), m_Object->GetHandle(), *toRemove.GetItemAtIndex( i ) ) ) );
	}

	GetApp().GetCommandProcessor().Submit( *cmd );

}