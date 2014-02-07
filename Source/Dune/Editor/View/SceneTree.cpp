#include <Dune/Editor/View/SceneTree.h> 

#include <Dune/Model/Scene/NodeRender.h>
#include <Dune/Model/Scene/World.h>
#include <Dune/Model/Scene/CameraPerspective.h>
#include <Dune/Editor/Application/Application.h>
#include <Dune/Editor/View/Editor/EditorContainer.h>
#include <Dune/Model/Project/Project.h>
#include <Dune/Editor/Ctrl/Cmd/AddObject.h>
#include <Dune/Editor/Ctrl/Scene.h>

#include <Sable/Core/Collection/HashTable.h>

using namespace Dune;

CViewSceneTree::CViewSceneTree( CViewScene& viewScene, CApplication& app ) :
    CViewBankObjectTree( viewScene, app )
{
	m_CtrlScene = NEWOBJ( CCtrlScene, ( app ) );

	Sb::CHashTable< Sb::CStringIdentifier, Sb::CVector3f> map( 32 );
	Sb::CVector3f v;

	map.AddItem( "test1", v );
	map.AddItem( "test2", v );
	map.AddItem( "test3", v );
	map.AddItem( "test4", v );

	const Sb::CVector3f& t = map.GetItem( "test1" );
}

CViewSceneTree::~CViewSceneTree()
{
}

CHandle CViewSceneTree::GetRootObject( CProject& p )
{
    return p.GetRootScene();
}

Bool CViewSceneTree::OnUIEditElement( )
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

Bool CViewSceneTree::HasIcon( const Sb::CTypeInfo& type ) const
{
    return TRUE;
}

QIcon CViewSceneTree::GetIcon( const Sb::CTypeInfo& type ) const
{
    return m_IconTable.GetIcon( GetIconIdFromType( type ) );
}

Bool CViewSceneTree::CanDropOn( const CHandle& h ) const
{
    return  GetProject()->GetObj<CBankObjectDirectory>( h ) || 
			GetProject()->GetObj<CSceneWorld>( h ) ||
			GetProject()->GetObj<CNodeRender>( h );
}

Void CViewSceneTree::contextMenuEvent(QContextMenuEvent * event )
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
        
        Dune::CSceneWorld* world = GetProject()->GetObj<Dune::CSceneWorld>( selectedItem->BankObject );
        if( world )
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
		ConfigureAddMenu( menu );
	}

    menu.exec( event->globalPos() );
  
}

Bool CViewSceneTree::ConfigureAddMenu( QMenu& subMenu )
{
    {
        const Sb::CTypeInfo& type = CBankObjectDirectory::GetStaticTypeInfo();
        QAction* action = subMenu.addAction( type.GetTypeName() );
        connect( action, SIGNAL( triggered() ), this, SLOT( OnUIAddElement() ) );
        action->setIcon( GetIcon( type ) );
    }
    subMenu.addSeparator();
    {
        const Sb::CTypeInfo& type = CSceneWorld::GetStaticTypeInfo();
        QAction* action = subMenu.addAction( type.GetTypeName() );
        connect( action, SIGNAL( triggered() ), this, SLOT( OnUIAddElement() ) );
        action->setIcon( GetIcon( type ) );
    }
    {
        const Sb::CTypeInfo& type = CNodeRender::GetStaticTypeInfo();
        QAction* action = subMenu.addAction( type.GetTypeName() );
        connect( action, SIGNAL( triggered() ), this, SLOT( OnUIAddElement() ) );
        action->setIcon( GetIcon( type ) );
    }
	{
		const Sb::CTypeInfo& type = CCameraPerspective::GetStaticTypeInfo();
		QAction* action = subMenu.addAction( type.GetTypeName() );
		connect( action, SIGNAL( triggered() ), this, SLOT( OnUIAddElement() ) );
		action->setIcon( GetIcon( type ) );
	}

    return TRUE;
}

Void CViewSceneTree::OnProjectCreated( CProject& p )
{
    CViewBankObjectTree::OnProjectCreated( p );
}

Void CViewSceneTree::OnProjectLoaded( CProject& p )
{
    CViewBankObjectTree::OnProjectLoaded( p );
}

Bool CViewSceneTree::OnUIAddElement()
{
	CBank* currentBank = GetApp().GetProjectCtrl().GetCurrentBank();
	if( !currentBank )
	{
		QMessageBox::warning( this, "Warning", "There is no active bank, please add one before" );
		return FALSE;
	}

    QAction* senderAction = (QAction*) sender();

	QString fullTypeName = "Dune::"+senderAction->text();
	QByteArray assetType = fullTypeName.toLocal8Bit();
    const Sb::CTypeInfo& assetTI = CBankObject::GetStaticTypeInfo().GetDerivedClass( assetType.data() );

    //COMMAND: AddObject
    CCmd* cmd = NEWOBJ( CCmdAddObject, ( GetProjectCtrl(), GetProject()->GetRootScene(), assetTI ) );
    GetApp().GetCommandProcessor().Submit( *cmd );

    return TRUE;
}

UInt32 CViewSceneTree::GetIconIdFromType( const Sb::CTypeInfo& info ) const
{
	UInt32 iconId = 0;
/*
	if( info == CWidgetFrame::GetStaticTypeInfo() )
	{
		iconId = 3;
	}
	else if( info == CLayoutVBox::GetStaticTypeInfo() )
	{
		iconId = 2;
	}
	else if( info == CLayoutHBox::GetStaticTypeInfo() )
	{
		iconId = 1;
	}
*/
	return iconId;
}

Bool CViewSceneTree::OnUIRemoveElement( )
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

CCtrlScene& CViewSceneTree::GetCtrl()
{
	return *m_CtrlScene;
}
