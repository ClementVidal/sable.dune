
#include <Dune/Editor/View/AssetTree.h>

#include <Dune/Model/Project/Project.h>
#include <Dune/Model/Property/Interface.h>
#include <Dune/Model/Asset/Header.h>
#include <Dune/Model/Bank/ObjectDirectory.h>
#include <Dune/Editor/Application/Application.h>
#include <Dune/Editor/Ctrl/Asset.h>
#include <Dune/Editor/Ctrl/Cmd/SetObjectProperty.h>

#include <QtGui/QContextMenuEvent>
#include <QtGui/QFileDialog>
#include <QtGui/QInputDialog>

using namespace Dune;

CViewAssetTree::CViewAssetTree( CViewAsset& viewAsset, CApplication& app ) :
    CViewBankObjectTree( viewAsset, app )
{
	m_AssetView = &viewAsset;
	m_Ctrl = NEWOBJ( CCtrlAsset, ( app ) );
}


CViewAssetTree::~CViewAssetTree()
{

}

Bool CViewAssetTree::IsDirectory( const QTreeWidgetItem& item )
{
	const CTreeWidgetItem& i = (const CTreeWidgetItem&) item;

	CBankObjectDirectory* asset = GetProject()->GetObj<CBankObjectDirectory>( i.BankObject );
	if( asset )
		return TRUE;

	return FALSE;
}

Void CViewAssetTree::contextMenuEvent(QContextMenuEvent * event )
{
	QMenu menu( this );

    if ( GetProject() == NULL )
        return;

    CTreeWidgetItem* id = (CTreeWidgetItem*)itemAt( event->pos() );
 
    QAction* action;

	// If we click on an asset
    if( id )
    {
		CBankObject* bankObj = GetProject()->GetObj<CBankObject>( id->BankObject );
		if( bankObj == NULL )
			return;

        AddEditEntry( *bankObj , menu );

		menu.addSeparator();
		action = menu.addAction( "Export to library..." );
		connect( action, SIGNAL( triggered() ), this, SLOT( OnUIExportToLibrary() ) );

		menu.addSeparator();
		action = menu.addAction( "Remove"  );
		connect( action, SIGNAL( triggered() ), this, SLOT( OnUIRemoveAsset() ) );

		if( GetProject()->GetIsSystemBank( bankObj->GetBank()->GetUuid() ) == FALSE )
		{
			menu.addSeparator();
			action = menu.addAction( "Use owner bank as current" );
			connect( action, SIGNAL( triggered() ), this, SLOT( OnUIUseOwnerBank() ) );
		}
		
    }
	// If we do not click on an asset
	else 
    {
        m_AssetView->ConfigureAddAssetMenu( menu );
    }

    menu.exec( event->globalPos() );
}

Void CViewAssetTree::AddEditEntry( CBankObject& asset, QMenu& subMenu )
{
	QAction* action = NULL;
    if( asset.GetTypeInfo().IsKindOf( CAssetFont::GetStaticTypeInfo() ) )
	{
		action = subMenu.addAction( "Edit..." );
		connect( action, SIGNAL( triggered() ), this, SLOT( OnUIEditFont() ) );
	}
	else if( asset.GetTypeInfo().IsKindOf( CAssetParticleSystem::GetStaticTypeInfo() ) )
	{
		action = subMenu.addAction( "Edit..." );
		connect( action, SIGNAL( triggered() ), this, SLOT( OnUIEditParticleSystem() ) );
	}
	else if( asset.GetTypeInfo().IsKindOf( CAssetShader::GetStaticTypeInfo() ) )
	{
		action = subMenu.addAction( "Edit..." );
		connect( action, SIGNAL( triggered() ), this, SLOT( OnUIEditShader() ) );
	}
	else if( asset.GetTypeInfo().IsKindOf( CAssetMaterial::GetStaticTypeInfo() ) )
	{
		action = subMenu.addAction( "Edit..." );
		connect( action, SIGNAL( triggered() ), this, SLOT( OnUIEditMaterial() ) );
	}
	else if( asset.GetTypeInfo().IsKindOf( CAssetModel::GetStaticTypeInfo() ) )
	{
		action = subMenu.addAction( "Display..." );

	}
	else if( asset.GetTypeInfo().IsKindOf( CAssetTexture2D::GetStaticTypeInfo() ) )
	{
		action = subMenu.addAction( "Display..." );
		connect( action, SIGNAL( triggered() ), this, SLOT( OnUIDisplayTexture() ) );
	}
}

QIcon CViewAssetTree::GetIcon( const Sb::CTypeInfo& type ) const
{
	return m_AssetView->GetIcon( type );
}

Bool CViewAssetTree::HasIcon( const Sb::CTypeInfo& type ) const
{
    return TRUE;
}

Bool CViewAssetTree::CanDropOn( const CHandle& h ) const
{
	return  GetProject()->GetObj<CBankObjectDirectory>( h ) != NULL;
}

Void CViewAssetTree::OnProjectCreated( CProject& p )
{
    CViewBankObjectTree::OnProjectCreated( p );
}

CHandle CViewAssetTree::GetRootObject( CProject& p )
{
    return p.GetRootAsset();
}

Bool CViewAssetTree::OnUIUseOwnerBank()
{
    // Add sub asset
    DebugAssert( selectedItems().count() );
    CTreeWidgetItem* selectedItem = (CTreeWidgetItem*)selectedItems().first();

    GetApp().GetProjectCtrl().SetCurrentBank( selectedItem->BankObject.GetBankUuid() );
    return TRUE;
}

void CViewAssetTree::OnUIExportToLibrary()
{
	QAction* senderAction = (QAction*) sender();

	// Add sub asset
	DebugAssert( selectedItems().count() );

	CTreeWidgetItem* selectedItem = (CTreeWidgetItem*)selectedItems().first();

	QString fileName = QFileDialog::getSaveFileName(this, "Select a file", "s:", "Dune library object (*.dlibobj)", NULL, QFileDialog::DontUseNativeDialog );

	if( !fileName.isEmpty() )
	{
		CBankObject* obj = GetProject()->GetObj( selectedItem->BankObject );
		m_Ctrl->ExportToLibrary( *obj, Sb::CFilePath( fileName.toLocal8Bit().constData() ) );
	}
}

Bool CViewAssetTree::OnUIEditAnimCurveSet()
{
	return TRUE;
}

Bool CViewAssetTree::OnUIEditMaterial()
{
	if( selectedItems().count() )
	{
		CTreeWidgetItem* selectedItem = (CTreeWidgetItem*)selectedItems().first();

		GetApp().GetProjectCtrl().OnEditElement( selectedItem->BankObject );
	}

	return TRUE;
}

Bool CViewAssetTree::OnUIEditParticleSystem()
{
	if( selectedItems().count() )
	{
		CTreeWidgetItem* selectedItem = (CTreeWidgetItem*)selectedItems().first();
	
		GetApp().GetProjectCtrl().OnEditElement( selectedItem->BankObject );
	}
	return TRUE;
}

Bool CViewAssetTree::OnUIDisplayTexture()
{
    if( selectedItems().count() )
    {
        CTreeWidgetItem* selectedItem = (CTreeWidgetItem*)selectedItems().first();

        GetApp().GetProjectCtrl().OnEditElement( selectedItem->BankObject );
    }

    return TRUE;
}

Bool CViewAssetTree::OnUIEditShader()
{
	if( selectedItems().count() )
	{
        CTreeWidgetItem* selectedItem = (CTreeWidgetItem*)selectedItems().first();

		GetApp().GetProjectCtrl().OnEditElement( selectedItem->BankObject );
	}
	return TRUE;
}

Bool CViewAssetTree::OnUIEditFont()
{
	if( selectedItems().count() )
	{
		CTreeWidgetItem* selectedItem = (CTreeWidgetItem*)selectedItems().first();

		GetApp().GetProjectCtrl().OnEditElement( selectedItem->BankObject );
	}
	return TRUE;

}


Bool CViewAssetTree::OnUIRemoveAsset( )
{
	if( selectedItems().count() )
    {
        CTreeWidgetItem* selectedItem = (CTreeWidgetItem*)selectedItems().first();

		//COMMAND: RemoveElement
		DebugAssert( selectedItem->BankObject.IsValid() );
		CCmd* cmd = NEWOBJ( CCmdRemoveObject, ( GetProjectCtrl(), selectedItem->BankObject ) );
		GetApp().GetCommandProcessor().Submit( *cmd );

	}	
	return TRUE;
}

CCtrlAsset&	CViewAssetTree::GetCtrl()
{
	return *m_Ctrl;
}
