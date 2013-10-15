
#include <Dune\Editor\View\GameTree.h>

#include <Dune\Model\Project\Project.h>
#include <Dune\Model\Property\Interface.h>
#include <Dune\Model\Game\Header.h>
#include <Dune\Model\Bank\ObjectDirectory.h>
#include <Dune\Editor\Application\Application.h>
#include <Dune\Editor\Ctrl\Game.h>
#include <Dune\Editor\Widget\FBXObjectPicker.h>
#include <Dune\Editor\Ctrl\Cmd\SetObjectProperty.h>
#include <Dune\Editor\Ctrl\Cmd\AddObjectFromFBX.h>

#include <QtGui\QContextMenuEvent>
#include <QtGui\QFileDialog>
#include <QtGui\QInputDialog>

using namespace Dune;

CViewGameTree::CViewGameTree( CViewGame& viewGame, CApplication& app ) :
    CViewBankObjectTree( viewGame, app )
{
	m_GameView = &viewGame;
	m_Ctrl = NEWOBJ( CCtrlGame, ( app ) );
}


CViewGameTree::~CViewGameTree()
{

}

Bool CViewGameTree::IsDirectory( const QTreeWidgetItem& item )
{
	const CTreeWidgetItem& i = (const CTreeWidgetItem&) item;

	CBankObjectDirectory* asset = GetProject()->GetObj<CBankObjectDirectory>( i.BankObject );
	if( asset )
		return TRUE;

	return FALSE;
}

Void CViewGameTree::OnAddObject( const CHandle& parent, const CHandle& h )
{
    CViewBankObjectTree::OnAddObject( parent, h );
}

Void CViewGameTree::OnFBXBindingChange( CFBXDocInterface& colladaInt )
{
	Sb::CFilePath file = colladaInt.GetFBXDocFilePath().GetValue();
	if( QFile::exists( file.GetPath() ) )
	{
		m_FileWatcher.addPath( file.GetPath() );
	}
}

Void CViewGameTree::contextMenuEvent(QContextMenuEvent * event )
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
		connect( action, SIGNAL( triggered() ), this, SLOT( OnUIRemoveGame() ) );

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
        m_GameView->ConfigureAddGameMenu( menu );
    }

    menu.exec( event->globalPos() );
}

Void CViewGameTree::AddEditEntry( CBankObject& asset, QMenu& subMenu )
{
	QAction* action = NULL;
	if( asset.GetTypeInfo().IsKindOf( CGameObjectPrototype::GetStaticTypeInfo() ) )
	{
		action = subMenu.addAction( "Edit..." );
		connect( action, SIGNAL( triggered() ), this, SLOT( OnUIEditGameObject() ) );
	}
}

QIcon CViewGameTree::GetIcon( const Sb::CTypeInfo& type ) const
{
	return m_GameView->GetIcon( type );
}

Bool CViewGameTree::HasIcon( const Sb::CTypeInfo& type ) const
{
    return TRUE;
}

Bool CViewGameTree::CanDropOn( const CHandle& h ) const
{
	return  GetProject()->GetObj<CBankObjectDirectory>( h ) != NULL;
}

Void CViewGameTree::OnProjectCreated( CProject& p )
{
    CViewBankObjectTree::OnProjectCreated( p );
}

Void CViewGameTree::OnProjectLoaded( CProject& p )
{
	Sb::CList<CHandle> list;
	Sb::CList<CHandle>::Iterator it;
	p.IssueQuery( "IsKindOf( CFBXDocInterface )", list );
	
	ForEachItem( it, list )
	{
		CFBXDocInterface* obj = GetProject()->GetObj<CFBXDocInterface>( it.GetData() );
		if( obj )
		{
			OnFBXBindingChange( *obj );
		}
	}

    CViewBankObjectTree::OnProjectLoaded( p );
}

CHandle CViewGameTree::GetRootObject( CProject& p )
{
    return p.GetRootGame();
}

void CViewGameTree::OnUIUseOwnerBank()
{
    // Add sub asset
    DebugAssert( selectedItems().count() );
    CTreeWidgetItem* selectedItem = (CTreeWidgetItem*)selectedItems().first();

    GetApp().GetProjectCtrl().SetCurrentBank( selectedItem->BankObject.GetBankUuid() );
}

void CViewGameTree::OnUIEditGameObject()
{
	if( selectedItems().count() )
	{
		CTreeWidgetItem* selectedItem = (CTreeWidgetItem*)selectedItems().first();

		GetApp().GetProjectCtrl().OnEditElement( selectedItem->BankObject );
	}
}

void CViewGameTree::OnUIExportToLibrary()
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

void CViewGameTree::OnUIRemoveGame( )
{
	if( selectedItems().count() )
    {
        CTreeWidgetItem* selectedItem = (CTreeWidgetItem*)selectedItems().first();

		//COMMAND: RemoveElement
		DebugAssert( selectedItem->BankObject.IsValid() );
		CCmd* cmd = NEWOBJ( CCmdRemoveObject, ( GetProjectCtrl(), selectedItem->BankObject ) );
		GetApp().GetCommandProcessor().Submit( *cmd );

	}	
}

CCtrlGame&	CViewGameTree::GetCtrl()
{
	return *m_Ctrl;
}
