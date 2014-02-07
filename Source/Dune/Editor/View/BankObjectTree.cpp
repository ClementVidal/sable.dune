#include <Dune/Editor/View/BankObjectTree.h>

#include <Dune/Model/Project/Project.h>
#include <Dune/Model/Property/Interface.h>
#include <Dune/Editor/Application/Application.h>
#include <Dune/Editor/Ctrl/Cmd/SetObjectProperty.h>
#include <Dune/Editor/Ctrl/Cmd/SetObjectHierarchy.h>

#include <QtGui/QContextMenuEvent>
#include <QtGui/QFileDialog>
#include <QtGui/QInputDialog>

using namespace Dune;

CViewBankObjectTree::CTreeWidgetItem::CTreeWidgetItem( const CHandle& bankObject )
{
	BankObject = bankObject;
}

CViewBankObjectTree::CTreeWidgetItem::CTreeWidgetItem( )
{
}

CViewBankObjectTree::CViewBankObjectTree(  QWidget& parent, CApplication& app ) :
    QTreeWidget( &parent ),
	CEventClient( app )
{
	QStringList header;
	header << "Name" << "Type" << "Bank" << "Handle";
	setHeaderLabels( header );
	setContextMenuPolicy( Qt::DefaultContextMenu );
	setAcceptDrops( TRUE );
	setDragEnabled( TRUE );
	setDropIndicatorShown( TRUE );
	setDragDropMode( QAbstractItemView::InternalMove );
	m_CanProcessSelChangedEvent = TRUE;

    connect( this, SIGNAL( itemDoubleClicked(QTreeWidgetItem *, int) ), this, SLOT( OnUILeftMouseButtonDClick(QTreeWidgetItem *, int) ) );
	connect( this, SIGNAL( itemSelectionChanged()  ), this, SLOT( OnUISelectionChanged() ) );

    setExpandsOnDoubleClick( FALSE ); 
    setIndentation( 10 );

}


CViewBankObjectTree::~CViewBankObjectTree()
{

}

Void CViewBankObjectTree::OnProjectDestroyed( )
{
	clear();
}

Void CViewBankObjectTree::OnProjectCreated( CProject& p )
{
    if( GetRootObject( p ).IsValid() == FALSE )
        return;

	Populate( p );
}

void CViewBankObjectTree::mousePressEvent( QMouseEvent * event )
{
	QTreeWidget::mousePressEvent( event );
    if( event->button() & Qt::LeftButton )
	    MakeDrag();
}

Bool CViewBankObjectTree::OnUILeftMouseButtonDClick( QTreeWidgetItem * item, int column )
{
	if ( GetProject() == NULL )
		return FALSE;

    // Rename bankObject
    bool ok;
    CHandle bankObject = ((CTreeWidgetItem*)item)->BankObject;

	CBankObject* clickedObj = GetProject()->GetObj( bankObject );
    QString text = QInputDialog::getText(this, "Rename object", "New name:", QLineEdit::Normal, clickedObj->GetName(), &ok );
    
    if( ok && text != clickedObj->GetName() )
    {
        CCmdSetObjectProperty* cmd = NEWOBJ( CCmdSetObjectProperty, ( GetApp().GetProjectCtrl(),  * clickedObj->GetProperty( "Name" ), Sb::CString( text.toAscii().constData() ) ) );
        GetApp().GetCommandProcessor().Submit( *cmd );
    }

	return TRUE;
}

void CViewBankObjectTree::dragMoveEvent(QDragMoveEvent *de)
{
	// The event needs to be accepted here
	de->accept();
}

void CViewBankObjectTree::dragEnterEvent(QDragEnterEvent *event)
{
	// Set the drop action to be the proposed action.
	event->acceptProposedAction();
}

void CViewBankObjectTree::dropEvent(QDropEvent *de)
{
	CDragAndDropMime* data = (CDragAndDropMime*) de->mimeData();
	Bool cloneObject = de->keyboardModifiers() & Qt::ShiftModifier;

	CTreeWidgetItem* itemUndexCursor = (CTreeWidgetItem*) itemAt( de->pos() );
	if( itemUndexCursor == NULL )
		return;

	CTreeWidgetItem* destItem = itemUndexCursor;
	CBankObject* destObject = GetProject()->GetObj<CBankObject>( destItem->BankObject );

	de->acceptProposedAction();

	Bool canDropOn = CanDropOn( destItem->BankObject );
	Bool sameParent = TRUE;
	QTreeWidgetItem* srcItem = NULL;
	CBankObject* srcBankObject = NULL;

	if( data->Items.size() != 1 )
		return;

	srcBankObject = GetProject()->GetObj<CBankObject>( ((CTreeWidgetItem*)data->Items.first())->BankObject );
	if( srcBankObject->GetHandle() != destObject->GetHandle() )
	{
		if( srcBankObject->GetParent() != destObject->GetParent() )
			sameParent = FALSE;
	}
	else
	{
		// Return if we drop on a draged item
		return;
	}

	QMenu menu( this );

	QAction* insertAbove = NULL;
	QAction* insertChild = NULL;
	QAction* insertBelow = NULL;
	CHandle newParent;
	Int32   newIndex=0;

	if( canDropOn && sameParent )
	{
		insertAbove = menu.addAction( "Move above"  );
		insertChild = menu.addAction( "Insert child"  );
		insertBelow = menu.addAction( "Move below"  );
	}
	else if( !canDropOn && sameParent )
	{
		insertAbove = menu.addAction( "Move above"  );
		insertBelow = menu.addAction( "Move below"  );
	}
	else if( canDropOn && !sameParent )
	{
		insertChild = menu.addAction( "Insert child"  );
	}

	QSize halfSize =  menu.frameSize() / 2;
	QAction* result = menu.exec( QCursor::pos() - QPoint( halfSize.width(), halfSize.height() ) );

	if( result == NULL )
		return;

	CBankObject* destParentObj = GetProject()->GetObj( destObject->GetParent() );

	if( result == insertChild )
	{
		newIndex = 0;
		newParent = destObject->GetHandle();
	}
	else if( result == insertBelow )
	{
		Int32 destIndex = destParentObj->GetChildIndex( destObject->GetHandle() );
		Int32 srcIndex = destParentObj->GetChildIndex( srcBankObject->GetHandle() );
		if( destIndex == destParentObj->GetChildCount()-1 )
			newIndex = destIndex;
		else if( newIndex > srcIndex )
			newIndex = destIndex ;
		else if( newIndex < srcIndex )
			newIndex = destIndex + 1;

		newParent = destObject->GetParent();
	}
	else if( result == insertAbove )
	{
		Int32 destIndex = destParentObj->GetChildIndex( destObject->GetHandle() );
		Int32 srcIndex = destParentObj->GetChildIndex( srcBankObject->GetHandle() );
		if( destIndex == 0 )
			newIndex = 0;
		else if( newIndex < srcIndex )
			newIndex = destIndex;
		else if( newIndex > srcIndex )
			newIndex = destIndex-1;

		newParent = destObject->GetParent();
	}

	if( cloneObject )
	{
		srcBankObject = srcBankObject->Clone();
	}

	// Send command to change parent
	CCmdSetObjectHierarchy* cmd = NEWOBJ( CCmdSetObjectHierarchy, ( GetApp().GetProjectCtrl(), srcBankObject->GetHandle(), newParent, newIndex ) );
	GetApp().GetCommandProcessor().Submit( *cmd );

}

void CViewBankObjectTree::MakeDrag()
{
	QDrag *dr = new QDrag(this);
	CDragAndDropMime *data = new CDragAndDropMime;
	data->Items = selectedItems();
	dr->setMimeData(data);
	dr->start();
}

Void CViewBankObjectTree::OnPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue )
{
    const CBankObject& bankObject = (CBankObject&) *p.GetObj();
    CTreeWidgetItem* item = GetTreeItem( bankObject.GetHandle() );

	if( item )
		item->setText( 0, bankObject.GetName() );

    // If we change the parent property
	if( Sb::StringCompare( p.GetName(), "Parent" ) == 0 )
	{
        Bool wasSelected = FALSE;

        if( item )
            wasSelected = item->isSelected();

        m_CanProcessSelChangedEvent = FALSE;
        if( wasSelected )
            item->setSelected( FALSE );

        if( item )
		    delete item;

        if(  p.GetObj()->GetHandle().IsValid() && bankObject.GetParent().IsValid() )
        {
		    QTreeWidgetItem* newParent = NULL;
			
			if( bankObject.GetParent() == GetRootObject( *GetProject() ) )
				newParent = invisibleRootItem();
			else
				newParent = GetTreeItem( bankObject.GetParent() );

			if( newParent )
			{
				CTreeWidgetItem* newItem = AppendItem( *newParent, *p.GetObj() ) ;
				if( wasSelected )
				{
					newParent->setSelected( FALSE );
					newItem->setSelected( TRUE );
				}
			}
        }
        m_CanProcessSelChangedEvent = TRUE;
	}
}

CViewBankObjectTree::CTreeWidgetItem* CViewBankObjectTree::RecursiveGetTreeItem( CTreeWidgetItem* root, const CHandle& bankObject )
{
	if( ! root )
		return NULL;

	if( root->BankObject == bankObject )
	{
		return root;
	}

	for( Int32 i=0;i<root->childCount();i++)
	{
		CTreeWidgetItem* child = (CTreeWidgetItem*) root->child( i );
		if( child->BankObject == bankObject )
		{
			return child;
		}
		if( child->childCount() )
		{
			CTreeWidgetItem* item = RecursiveGetTreeItem( (CTreeWidgetItem*) child, bankObject );
			if( item )
				return item;
		}
	}
	return NULL;
}

Void CViewBankObjectTree::Populate( CProject& p )
{
	QTreeWidgetItem* invisibleRoot = invisibleRootItem();
	while( invisibleRoot->childCount() )
	{
		delete invisibleRoot->child( 0 );
	}

	CBankObject* obj = p.GetObj( GetRootObject( p ) );

	DebugAssert( obj );

	// Recursivly add all of it's childs
	UInt32 i;
	for(i=0;i<obj->GetChildCount();i++)
	{
		CHandle h = obj->GetChildAtIndex( i );
		if( h.IsValid() )
			AppendItem( *invisibleRoot, *GetProject()->GetObj<CBankObject>( h ) );
	}

	resizeColumnToContents( 0 );
}

Void CViewBankObjectTree::OnProjectLoaded( CProject& p )
{
    if( GetRootObject( p ).IsValid() == FALSE )
        return;

	Populate( p );
}

CViewBankObjectTree::CTreeWidgetItem* CViewBankObjectTree::GetTreeItem( const CHandle& bankObject )
{
	if( GetProject() == NULL )
		return NULL;

    QTreeWidgetItem* root = invisibleRootItem();
	for(Int32 i=0;i<root->childCount();i++)
	{
		CTreeWidgetItem* item = RecursiveGetTreeItem( (CTreeWidgetItem*) root->child( i ), bankObject ); 
		if( item )
			return item;
	}

	return NULL;
}

Bool CViewBankObjectTree::HasIcon( const Sb::CTypeInfo& type ) const
{
    return FALSE;
}

Bool CViewBankObjectTree::CanDropOn( const CHandle& h ) const
{
    return  FALSE;
}

QIcon CViewBankObjectTree::GetIcon( const Sb::CTypeInfo& type ) const
{
    return QIcon();
}

CViewBankObjectTree::CTreeWidgetItem* CViewBankObjectTree::AppendItem( QTreeWidgetItem& parentId, const CBankObject& bankObject, Int32 index )
{
	CTreeWidgetItem* childNode = NULL;

	childNode = new CTreeWidgetItem( bankObject.GetHandle() );
	childNode->setText( 0, bankObject.GetName() );
	childNode->setText( 1, bankObject.GetTypeInfo().GetTypeName() );
	childNode->setText( 2, bankObject.GetBank()->GetName() );
    childNode->setText( 3, CLexicalCast::ValueToString( bankObject.GetHandle() ).GetBuffer() );
    if( HasIcon(  bankObject.GetTypeInfo() ) )
    {
	    childNode->setIcon( 0, GetIcon( bankObject.GetTypeInfo() ) );
    }
	if( index == -1 )
		parentId.addChild( childNode );
	else if( index>=0 && index<parentId.childCount() )
		parentId.insertChild( index, childNode );
	else
		parentId.addChild( childNode );

    parentId.setExpanded( TRUE );

    // Recursivly add all of it's childs
    UInt32 i;
    for(i=0;i<bankObject.GetChildCount();i++)
    {
        CHandle h = bankObject.GetChildAtIndex( i );
        if( h.IsValid() )
            AppendItem( *childNode, *GetProject()->GetObj<CBankObject>( h ) );
    }

	return childNode;
}

Void CViewBankObjectTree::RemoveItem( const CHandle& h )
{
    CTreeWidgetItem* itemID = GetTreeItem( h );

    if( m_LastSelectedItem == itemID )
        m_LastSelectedItem = NULL;

    delete itemID;
}

Void CViewBankObjectTree::OnRemoveObject( const CHandle& h )
{
	RemoveItem( h );
}

CCtrlProject& CViewBankObjectTree::GetProjectCtrl()
{
    return GetApp().GetProjectCtrl();
}

Void CViewBankObjectTree::OnAddObject( const CHandle& parent, const CHandle& h )
{
}

Void CViewBankObjectTree::OnChildIndexChanged( const CHandle& parent, Int32 oldIndex, Int32 newIndex )
{
	CTreeWidgetItem* parentItem = GetTreeItem( parent );
	if( !parentItem )
		return;

	m_CanProcessSelChangedEvent = FALSE;

	CTreeWidgetItem* oldItem = (CTreeWidgetItem*)parentItem->child( oldIndex );
	CHandle childHandle = oldItem->BankObject;
	DebugAssert( oldItem );
	delete oldItem;

	CBankObject* childObject = GetProject()->GetObj( childHandle );
	CTreeWidgetItem* newItem = AppendItem( *parentItem, *childObject, newIndex );
	/*if( wasSelected )
	{
		newParent->setSelected( FALSE );
		newItem->setSelected( TRUE );
	}*/

	m_CanProcessSelChangedEvent = TRUE;
}

Bool CViewBankObjectTree::OnUISelectionChanged( )
{
	if( selectedItems().count() == 0 || m_CanProcessSelChangedEvent == FALSE )
		return FALSE;

	CTreeWidgetItem* item = (CTreeWidgetItem*) selectedItems().first();

	if( item->BankObject.IsValid() )
	{
		CBankObject* currentObj = GetProject()->GetObj( item->BankObject );

		CCmdMeta* meta = NEWOBJ( CCmdMeta, ( ((Sb::CString)"SelectObject: ") + currentObj->GetName() ) );

        Sb::CArray<CHandle> objList( GetProjectCtrl().GetSelectedObjects() );
		meta->PushCommand( * NEWOBJ( CCmdSelectObject, ( GetProjectCtrl(), objList.GetBuffer(), objList.GetItemCount(), CCmdSelectObject::nAction_DeSelect) ) );
		meta->PushCommand( * NEWOBJ( CCmdSelectObject, ( GetProjectCtrl(), currentObj->GetHandle(), CCmdSelectObject::nAction_Select ) ) );
		GetApp().GetCommandProcessor().Submit( *meta );	
	}

	return TRUE;
}

Void CViewBankObjectTree::OnObjectDeSelected( const CHandle& current )
{
	CTreeWidgetItem* item = NULL;

	m_CanProcessSelChangedEvent = FALSE;

	if( current.IsValid() )
	{
		item = GetTreeItem( current );
		if( item == NULL )
		{
			m_CanProcessSelChangedEvent = TRUE;
			return;
		}

		item->setSelected( FALSE );
	}

	m_CanProcessSelChangedEvent = TRUE;
	m_LastSelectedItem = NULL;
}

Void CViewBankObjectTree::OnObjectSelected( const CHandle& current )
{
	m_CanProcessSelChangedEvent = FALSE;

	if( current.IsValid() )
	{
		m_LastSelectedItem = GetTreeItem( current );
	}
	else 
	{
		m_LastSelectedItem = NULL;
	}

	if( m_LastSelectedItem )
		m_LastSelectedItem->setSelected( TRUE );
	else
		clearSelection();

	m_CanProcessSelChangedEvent = TRUE;
}
