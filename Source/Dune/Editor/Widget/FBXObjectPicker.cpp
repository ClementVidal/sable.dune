#include <Dune/Editor/Widget/FBXObjectPicker.h> 

#include <Dune/Editor/Application/Application.h>
#include <Dune/Model/Asset/Helper.h>
#include <Dune/Model/Asset/Material.h>

#include <QtGui/QPushButton.h>

using namespace Dune;

CFBXObjectPicker::STreeItemData::STreeItemData( CFBXObject* obj, const Sb::CTypeInfo& t )
{
	FBXObject = obj;
	Type = &t;
}

CFBXObjectPicker::CFBXObjectPicker( CApplication& app, CFBXDoc& dae ) :
    QDialog( &app.GetMainFrame(), Qt::Tool  ),
	m_FBXDoc( dae )
{
    m_App = &app;

    setWindowTitle( "FBX Object picker" );

	// Reload the FBX if necessary
	m_FBXDoc.ReloadIfNecessary();

    QVBoxLayout* layout = new QVBoxLayout();
    QHBoxLayout* bottomLayout = new QHBoxLayout( );

    m_TreeCtrl = new QTreeWidget( this );
	connect( m_TreeCtrl, SIGNAL( itemSelectionChanged() ), this, SLOT( OnUISelectionChanged() ) );
	QStringList header;
	header << "Name" << "ColladaId";
	m_TreeCtrl->setHeaderLabels( header );

	m_OkButton = new QPushButton( "OK" );
	connect( m_OkButton, SIGNAL( clicked() ), this, SLOT( OnUIOk() ) );
    bottomLayout->addWidget( m_OkButton );        

	QPushButton* cancelButton = new QPushButton( "Cancel" );
	connect( cancelButton, SIGNAL( clicked() ), this, SLOT( OnUICancel() ) );
	bottomLayout->addWidget( cancelButton );  

    layout->addWidget( m_TreeCtrl );
    layout->addLayout( bottomLayout );

    setLayout( layout );

	m_OkButton->setEnabled( FALSE );

	resize( 300, 300 );

	m_MeshTreeItem = new QTreeWidgetItem();
	m_MeshTreeItem->setText( 0, "Model" );
	m_TreeCtrl->invisibleRootItem()->addChild( m_MeshTreeItem );
	
	m_MeshTreeItem->setExpanded( true );
	PopulateTree( );

	m_TreeCtrl->resizeColumnToContents( 0 );
	m_TreeCtrl->resizeColumnToContents( 1 );
}

CFBXObjectPicker::~CFBXObjectPicker()
{

}

const Sb::CArray<CFBXObjectPicker::SResult>& CFBXObjectPicker::GetResult( ) const
{
	return m_Results;
}

Void CFBXObjectPicker::PopulateMesh( FbxNode* node )
{
	FbxMesh* mesh = (FbxMesh*) node->GetNodeAttribute ();

	CFBXObject* fbxObject = NEWOBJ( CFBXObject, ( m_FBXDoc, node ) );

	STreeItemData* item = new STreeItemData( fbxObject, CAssetModel::GetStaticTypeInfo() );
	item->setText( 0, node->GetName() );	
	item->setText( 1, QString::number( (UInt32)node->GetUniqueID() ) );
	m_MeshTreeItem->addChild( item );
}

Void CFBXObjectPicker::PopulateTree( FbxNode* node )
{
	FbxNodeAttribute::EType lAttributeType;
	if(node->GetNodeAttribute() != NULL)
	{
		lAttributeType = (node->GetNodeAttribute()->GetAttributeType());

		switch (lAttributeType)
		{
		case FbxNodeAttribute::eMarker:  
			break;

		case FbxNodeAttribute::eSkeleton:  
			break;

		case FbxNodeAttribute::eMesh:      
			PopulateMesh(node);
			break;

		case FbxNodeAttribute::eNurbs:  
			break;

		case FbxNodeAttribute::ePatch:     
			break;

		case FbxNodeAttribute::eCamera:   
			break;

		case FbxNodeAttribute::eLight:   
			break;

		case FbxNodeAttribute::eLODGroup:
			break;
		}   
	}

	for( Int32 i=0; i<node->GetChildCount(); i++ )
	{
		PopulateTree(node->GetChild(i));
	}
}

Void CFBXObjectPicker::PopulateTree( )
{
	FbxNode* root = m_FBXDoc.GetFBXScene()->GetRootNode();

	if( root == NULL )
		return;

	for( Int32 i=0; i<root->GetChildCount(); i++ )
	{
		PopulateTree(root->GetChild(i));
	}

	m_TreeCtrl->resizeColumnToContents( 0 );
}

CApplication& CFBXObjectPicker::GetApp()
{
    return *m_App;
}

Bool CFBXObjectPicker::OnUICancel( )
{
	done( 0 );
	return TRUE;
}

Bool CFBXObjectPicker::OnUIOk( )
{
	QList<QTreeWidgetItem*> items = m_TreeCtrl->selectedItems( );

	QTreeWidgetItem* item;
	UInt32 i = 0;
	m_Results.SetItemCount( items.size() );
	Q_FOREACH( item, items )
	{
		STreeItemData* itemData = (STreeItemData*)item;
		m_Results[i].FBXObject = itemData->FBXObject;
		m_Results[i].Type = itemData->Type;
		i++;
	}
	done( 1 );
	return TRUE;
}

Bool CFBXObjectPicker::OnUISelectionChanged( )
{
	m_OkButton->setEnabled( FALSE );
	if( m_TreeCtrl->selectedItems().size() )
	{
		m_OkButton->setEnabled( TRUE );
	}
	return TRUE;
}
