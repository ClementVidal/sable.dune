#include <Dune\Editor\View\UITree.h> 

#include <Dune\Editor\Application\Application.h>
#include <Dune\Editor\View\Editor\EditorContainer.h>
#include <Dune\Model\Project\Project.h>
#include <Dune\Editor\Ctrl\Cmd\AddObject.h>
#include <Dune\Editor\Ctrl\UI.h>

using namespace Dune;

CViewUITree::CViewUITree( CViewUI& uiView, CApplication& app ) :
    CViewBankObjectTree( uiView, app )
{	
	m_UIView = &uiView;
	m_CtrlUI = NEWOBJ( CCtrlUI, ( app ) );
}

CViewUITree::~CViewUITree()
{
}

Bool CViewUITree::OnUIEditElement( )
{
	if( selectedItems().count() == 0 )
		return FALSE;

	CTreeWidgetItem* item = (CTreeWidgetItem*) selectedItems().first();

    CHandle handle = item->BankObject;
    if( handle.IsValid() )
    {
        GetApp().GetProjectCtrl().OnEditElement( handle );
    }
	
	return TRUE;
}

QIcon CViewUITree::GetIcon( const Sb::CTypeInfo& type ) const
{
	return m_UIView->GetIcon( type );
}

Bool CViewUITree::HasIcon( const Sb::CTypeInfo& type ) const
{
	return TRUE;
}

Bool CViewUITree::CanDropOn( const CHandle& h ) const
{
	return  GetProject()->GetObj<CLayoutHBox>( h ) ||
			GetProject()->GetObj<CLayoutVBox>( h ) ||
			GetProject()->GetObj<CWidget>( h );
}

Void CViewUITree::contextMenuEvent(QContextMenuEvent * event )
{
	QMenu menu( this );
	QAction* action = NULL;

	if ( GetProject() == NULL )
		return;

	CTreeWidgetItem* id = (CTreeWidgetItem*)itemAt( event->pos() );

    if( id )
    {
		CTreeWidgetItem* selectedItem = (CTreeWidgetItem*)selectedItems().first();

        DebugAssert( selectedItem->BankObject.IsValid() );

        CWidget* uiRoot = GetProject()->GetObj<CWidget>( selectedItem->BankObject );
        if( uiRoot )
        {
            menu.addSeparator();
            action = menu.addAction( "Edit..."  );
            connect( action, SIGNAL( triggered() ), this, SLOT( OnUIEditElement() ) );
        }

        CBankObject* bankObj = GetProject()->GetObj<CBankObject>( selectedItem->BankObject );
        if( bankObj && bankObj->GetParent().IsValid() )
        {
		    action = menu.addAction( "Remove"  );
		    connect( action, SIGNAL( triggered() ), this, SLOT( OnUIRemoveElement() ) );
        }
        
	}
	else
	{
		m_UIView->ConfigureAddMenu( menu );		
	}

	menu.exec( event->globalPos() );
}

Void CViewUITree::OnProjectCreated( CProject& p )
{
    CViewBankObjectTree::OnProjectCreated( p );
}

Void CViewUITree::OnProjectLoaded( CProject& p )
{
    CViewBankObjectTree::OnProjectLoaded( p );
}

CHandle CViewUITree::GetRootObject( CProject& p )
{
    return p.GetRootUI();
}

Bool CViewUITree::OnUIRemoveElement( )
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

CCtrlUI& CViewUITree::GetCtrl()
{
	return *m_CtrlUI;
}
