#include <Dune\Editor\View\PropertyGrid.h> 

#include <Dune\Model\Property\Property.h> 

using namespace Dune;

CViewPropertyGrid::CViewPropertyGrid( CViewProperty& view ) :
    QTreeWidget(  )
{
	m_App = &view.GetApp();
	m_ViewProperty = &view;
	m_EditedAsset = NULL;
	m_PropertyObject = NULL;
	CanChangeSelection = TRUE;
	QStringList header;
	header << "Name" << "Value";
	setHeaderLabels( header );

    setVerticalScrollMode( QAbstractItemView::ScrollPerPixel );
    setIndentation( 10 );
}

CViewPropertyGrid::~CViewPropertyGrid()
{
}

CApplication& CViewPropertyGrid::GetApp()
{
    return *m_App;
}

CBankObject* CViewPropertyGrid::GetPropertyObject() const
{
    return m_PropertyObject;
}

Void CViewPropertyGrid::SetPropertyObject( CBankObject* obj )
{
	m_SlotObjectPropertyAdded.DisconnectAll();
	m_SlotObjectPropertyRemoved.DisconnectAll();

    m_PropertyObject = obj;
    
	if( obj )
    {
		EVENT_CONNECT_SLOT( obj->SignalPropertyAdded, m_SlotObjectPropertyAdded, &CViewPropertyGrid::OnObjectPropertyAdded );
		EVENT_CONNECT_SLOT( obj->SignalPropertyRemoved, m_SlotObjectPropertyRemoved, &CViewPropertyGrid::OnObjectPropertyRemoved );
    }

	FillProperties();
}

Void CViewPropertyGrid::FillProperties()
{
	Sb::CMap<Sb::CString, QTreeWidgetItem*>::Iterator it;
	ForEachItem( it, m_GroupTable )
	{
		QTreeWidgetItem* item = (*it);
		delete item;
	}

	m_GroupTable.ClearAllItems();

	clear();

	if( m_PropertyObject )
	{
		CBankObject::PropertyTable::Iterator it;
		const CBankObject::PropertyTable& propTable = m_PropertyObject->GetPropertyTable();

		ForEachItem( it, propTable )
		{
			AddProperty( **it );
		}
	}
}

Void CViewPropertyGrid::OnObjectPropertyAdded( const CPropertyInterface& p )
{
	FillProperties();
}

Void CViewPropertyGrid::OnObjectPropertyRemoved( const CPropertyInterface& p )
{
	FillProperties();
}

Void CViewPropertyGrid::AddProperty( const CPropertyInterface& p )
{
    CApplication* app = &GetApp();

    if( !p.GetIsVisible() )
    {
        return;
    }

    CViewPropertyBase* wdg = CViewPropertyBase::CreateViewFromModel( app, p );

    if( wdg == NULL )
        return;

    Sb::CString groupName;
	QTreeWidgetItem* group = NULL;
	CViewPropertyGrid* grid = & GetApp().GetPropertyView().GetPropertyGrid();
    //Detect group name
    if( Sb::CString( p.GetGroup() ).IsEmpty() == FALSE )
    {
		groupName = p.GetGroup();
	}
	else
	{
		groupName = "Default";
	}

    // Retrieve group node
    if( m_GroupTable.HasItem( groupName ) )
    {
        group = m_GroupTable.GetItem( groupName );
    }
    else		
    {
        group = new QTreeWidgetItem (  );
		group->setText( 0, groupName.GetBuffer( ) );

		m_GroupTable.AddItem( groupName, group );

		addTopLevelItem( group );

		group->setExpanded( TRUE );
		group->setBackgroundColor( 0, QColor( 128, 128, 128 ) );
		group->setBackgroundColor( 1, QColor( 128, 128, 128 ) );
    }

    // Setup Widget
    wdg->Initialize( *this, *group );   

	// Assign bitmap to cell
	if( p.GetTypeInfo() == CPropertyHandle::GetStaticTypeInfo() )
	{
		CPropertyHandle* pH = (CPropertyHandle*)&p;
		const CPropertyHandle::FilterList& f = pH->GetFilterList();

		if( f.GetItemCount() )
		{
			if( f.GetFirstItem()->IsKindOf( CAsset::GetStaticTypeInfo() ) )
			{
				wdg->TreeItem->setIcon( 0, GetApp().GetAssetView().GetIcon( *f.GetFirstItem() ) );
			}
		}	
	}
} 