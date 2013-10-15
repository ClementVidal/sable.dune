#include <Dune\Model\Property\PropertyArray.h>

using namespace Dune;

IMPLEMENT_MANAGED_CLASS1( Dune, CPropertyArray, CPropertyInterface);

CPropertyArray::CPropertyArray( ) :
    CPropertyInterface( )
{
    m_Type = NULL;
}

CPropertyArray::CPropertyArray( String name, const Sb::CTypeInfo& type, UInt32 nb ) :
    CPropertyInterface( name )
{
	m_Type = & type;
    SetItemCount( nb );
}

CPropertyArray::~CPropertyArray()
{

}

Sb::CString CPropertyArray::GetValueAsString() const
{
    return "{}";
}

Bool CPropertyArray::SetValueFromString( String v )
{
    return TRUE;
}

const Sb::CTypeInfo& CPropertyArray::GetItemTypeInfo()const
{
	return *m_Type;
}

Void CPropertyArray::SetItemAtIndex( UInt32 i, CPropertyInterface& a ) 
{
	DebugAssert( i<GetItemCount() );

	m_Array[i] = &a;

	EVENT_CONNECT_SLOT( a.SignalPropertyChanged, m_SlotPropertyChanged, &CPropertyArray::OnChildPropertyChanged );

	SignalPropertyAdded( a );
}

CPropertyInterface& CPropertyArray::GetItemAtIndex( UInt32 i ) 
{
	return (CPropertyInterface&) *m_Array[i];
}

const CPropertyInterface& CPropertyArray::GetItemAtIndex( UInt32 i ) const
{
    return (const CPropertyInterface&) *m_Array[i];
}

Void CPropertyArray::AddItemAtEnd( CPropertyInterface& a )
{
    DebugAssertMsg( a.GetMemoryHeap(), "Property MUST be allocated within a heap, not directly on the local stack" );
    m_Array.AddItemAtEnd( &a );
	if( GetObj() )
	{
		a.BindToObject( GetObj() );
	}
	else
	{
		DebugError("Unsuported opeartion");
	}

    EVENT_CONNECT_SLOT( a.SignalPropertyChanged, m_SlotPropertyChanged, &CPropertyArray::OnChildPropertyChanged );

	SignalPropertyAdded( a );
}


Void CPropertyArray::AddItemAtIndex( CPropertyInterface& a, UInt32 index )
{
	DebugAssertMsg( a.GetMemoryHeap(), "Property MUST be allocated within a heap, not directly on the local stack" );
	m_Array.AddItemAtIndex( &a, index );
	if( GetObj() )
	{
		a.BindToObject( GetObj() );
	}
	else
	{
		DebugError("Unsuported opeartion");
	}

	EVENT_CONNECT_SLOT( a.SignalPropertyChanged, m_SlotPropertyChanged, &CPropertyArray::OnChildPropertyChanged );

	SignalPropertyAdded( a );
}

Void CPropertyArray::MoveItem( Int32 oldIndex, Int32 newIndex )
{
	newIndex = MathClamp( newIndex, 0, (Int32)m_Array.GetItemCount()-1 );
	oldIndex = MathClamp( oldIndex, 0, (Int32)m_Array.GetItemCount()-1 );
	m_Array.MoveItem( oldIndex, newIndex );

	SignalItemIndexChanged( oldIndex, newIndex );
}

CPropertyInterface* CPropertyArray::AddItemAtEnd( )
{
	CPropertyInterface* p = (CPropertyInterface*)m_Type->CreateObject( NULL );
    p->Initialize( GetName() );

	m_Array.AddItemAtEnd( p );
	
	if( GetObj() )
	{
		p->BindToObject( GetObj() );
	}
	else
	{
		DebugError("Unsuported opeartion");
	}

    EVENT_CONNECT_SLOT( p->SignalPropertyChanged, m_SlotPropertyChanged, &CPropertyArray::OnChildPropertyChanged );

	SignalPropertyAdded( *p );

	return p;
}

Void CPropertyArray::BindToObject( CBankObject* obj )
{
	CPropertyInterface::BindToObject( obj );
	UInt32 i;
	for(i=0;i<m_Array.GetItemCount();i++)
	{
		m_Array[i]->BindToObject( GetObj() );
	}
}

Void CPropertyArray::SetItemCount( UInt32 nb )
{
	m_Array.SetItemCount( nb );
	UInt32 i;
	for(i=0;i<m_Array.GetItemCount();i++)
	{
		m_Array[i] = (CPropertyInterface*)m_Type->CreateObject( NULL );
        m_Array[i]->Initialize( GetName() );
		m_Array[i]->BindToObject( GetObj() );

		SignalPropertyAdded( *m_Array[i] );
         
	    EVENT_CONNECT_SLOT( m_Array[i]->SignalPropertyChanged, m_SlotPropertyChanged, &CPropertyArray::OnChildPropertyChanged );
	}
}

Void CPropertyArray::OnChildPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue )
{
	// Notify that this property has changed
	SignalPropertyChanged( *this, "" );
}

Void CPropertyArray::RemoveItemAtIndex( UInt32 i )
{
    // Disconnect signal
	SignalPropertyRemoved( *m_Array[i] );

    m_SlotPropertyChanged.Disconnect( m_Array[i]->SignalPropertyChanged );
    m_Array.RemoveItemAtIndex( i );
}

Void CPropertyArray::RemoveItem( const CPropertyInterface& a )
{
    UInt32 i = 0;
    for( i=0;i<m_Array.GetItemCount(); i++ )
    {
        if( m_Array[i] == &a )
            break;
    }

    if( i < m_Array.GetItemCount() )
    {
        RemoveItemAtIndex( i );
    }
}

Void CPropertyArray::ClearAllItems( )
{ 
    // Disconnect signal
    UInt32 i = 0;
    for( i=0;i<m_Array.GetItemCount(); i++ )
    {
		SignalPropertyRemoved( *m_Array[i] );

		m_SlotPropertyChanged.Disconnect( m_Array[i]->SignalPropertyChanged );
    }
   
    m_Array.ClearAllItems( );
}

UInt32 CPropertyArray::GetItemCount() const
{
    return m_Array.GetItemCount();
}

Bool CPropertyArray::CopyFrom( const CPropertyInterface& p )
{
	const CPropertyArray& srcProp = (const CPropertyArray&)p;
	DebugAssert( p.GetTypeInfo().IsKindOf( GetTypeInfo() ) );

	SetItemCount( srcProp.GetItemCount() );

	for( UInt32 i=0;i<srcProp.GetItemCount();i++)
	{
		CPropertyInterface* dstItem = (CPropertyInterface*)m_Type->CreateObject( NULL );
		dstItem->CopyFrom( srcProp.GetItemAtIndex( i ) );
		SetItemAtIndex( i, *dstItem );
	}

	return TRUE;
}

Bool CPropertyArray::Serialize( Sb::CPersistentArchive& ar )
{
    UInt32 itemCount, i;
    Int16 version = 0;

    itemCount = m_Array.GetItemCount();

    ar.Serialize( itemCount, WT("ItemCount" ) );

    if( ar.IsLoading() )
    {
        DebugAssert( m_Array.GetItemCount() == 0 );

        m_Array.SetItemCount( itemCount );
        for( i=0;i<itemCount;i++ )
        {
            m_Array[i] = (CPropertyInterface*)m_Type->CreateObject( NULL );
            m_Array[i]->Initialize( GetName() );
			m_Array[i]->BindToObject( GetObj() );

            if( ar.BeginScope( WT("Entry" ), version ) == version )
            {
                m_Array[i]->Serialize( ar );
                ar.EndScope();
            }
            
        }
    }
    else
    {
        for( i=0;i<itemCount;i++ )
        {
            if( ar.BeginScope( WT("Entry" ), version ) == version )
            {
                m_Array[i]->Serialize( ar );
                ar.EndScope();
            }
        }
    }

    return TRUE;
}
