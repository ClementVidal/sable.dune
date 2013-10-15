#include <Dune\Model\Bank\Object.h>

using namespace Dune;

String CBankObject::PropName = "Name";
String CBankObject::PropChilds = "Childs";
String CBankObject::PropParent = "Parent";

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CBankObject, Sb::CManagedObject )


CBankObject::SDependency::SDependency()
{
    Count = 0;
}

Bool CBankObject::SDependency::operator == ( const CBankObject::SDependency& d )
{
    if( d.Handle == Handle )
        return TRUE;

    return FALSE;
}

CBankObject::CBankObject( )
{
    m_Bank = NULL;

    m_Name = NEWOBJ( CPropertyString, ( PropName, CPropertyString::nType_SingleLine ) );
    m_Name->SetGroup( "BankObject" );
    AddProperty( *m_Name );

    m_Childs = new CPropertyArray( PropChilds, CPropertyHandle::GetStaticTypeInfo() );
    m_Childs->SetIsEnabled( FALSE );
    m_Childs->SetIsAutoSerialized( FALSE );
    m_Childs->SetGroup( "BankObject" );
    AddProperty( *m_Childs );

    m_Parent = new CPropertyHandle( PropParent );
    m_Parent->SetIsEnabled( FALSE );
    m_Parent->AddDependencyToOwner( FALSE );
    m_Parent->SetGroup( "BankObject" );
    AddProperty( *m_Parent );
}

Void CBankObject::SetBank( CBank& repo )
{
    m_Bank = &repo;
}

CBankObject::~CBankObject()
{
    UInt32 i;

    CBankObject* e = GetObjAccessor()->GetObj( GetParent( ) );
    for( i=0;i<GetChildCount();i++)
    {
        CBankObject* bankObj = GetObjAccessor()->GetObj( GetChildAtIndex(i) );
        bankObj->GetBank()->RemoveObject( bankObj->GetHandle() );
    }

    if( e )
        e->RemoveChild( GetHandle() );

}

/**
Return the number of property that compose this object
*/
UInt32 CBankObject::GetPropertyCount() const
{
    return m_PropertyTable.GetItemCount();
}

/**
Return the property table
*/
const CBankObject::PropertyTable& CBankObject::GetPropertyTable() const
{
    return m_PropertyTable;
}

/**
Remove a given property.
If the new property is removed from  the object the SignalPropertyRemoved signal is fired
*/
Void CBankObject::RemoveProperty( CPropertyInterface& p )
{
    if( m_PropertyTable.HasItem( p.GetName() ) )
	{
		m_PropertyTable.RemoveItemAtKey( p.GetName() );
		SignalPropertyRemoved( p );
	}
}

/**
Add a new property to this object.
If the new property is added to the object the SignalPropertyAdded signal is fired
\return TRUE if the property have been succefully added, FALSE otherwise
*/
Bool CBankObject::AddProperty( CPropertyInterface& p )
{
    if( m_PropertyTable.HasItem( p.GetName() ) )
        return FALSE;

	p.BindToObject( this );
    m_PropertyTable.AddItem( p.GetName(), &p );

	SignalPropertyAdded( p );

	EVENT_CONNECT_SLOT( p.SignalPropertyChanged, m_SlotOnPropertyChanged, &CBankObject::OnPropertyChanged );

	if( p.GetTypeInfo().IsKindOf( CPropertyArray::GetStaticTypeInfo() ) )
	{
		CPropertyArray& pArray = (CPropertyArray&)p;
		EVENT_CONNECT_SIGNAL( pArray.SignalPropertyAdded, SignalPropertyAdded );
		EVENT_CONNECT_SIGNAL( pArray.SignalPropertyRemoved, SignalPropertyRemoved );
	}

    return TRUE;
}

/**
Return a specific property using a given property name
\return NULL is no property with this name exists
*/
CPropertyInterface* CBankObject::GetProperty( String name ) const
{
    if( ! m_PropertyTable.HasItem( name ) )
        return NULL;

    return m_PropertyTable.GetItem( name );
}

Void CBankObject::SetHandle( const CHandle& handle ) 
{
    m_Handle = handle;
}


const CHandle& CBankObject::GetHandle( ) const
{
    return m_Handle;
}

CBank* CBankObject::GetBank() const
{
    return m_Bank;
}

/**
Return the project associated with the binded bank
*/
CProject* CBankObject::GetProject() const
{
	if( m_Bank )
	{
		return m_Bank->GetProject();
	}

	return NULL;
}

/**
Return the object accessor used to retrieve other object
*/
CBankObjectAccessor* CBankObject::GetObjAccessor() const
{
	if( m_Bank )
	{
		if( m_Bank->GetProject() )
			return m_Bank->GetProject();
		else
			return m_Bank;
	}

	return NULL;
}

Bool CBankObject::Serialize( Sb::CPersistentArchive& ar )
{
    Char pName[256];
    Char pType[256];
    Int16 version = 0;

    // Serialize handle first ( it could be used right afterward )
    CHandle tmp;
    if( ar.IsLoading() )
        tmp = CHandle( m_Bank->GetUuid() );
    else
        tmp = m_Handle;

    if( !ar.Serialize( tmp, WT("Handle") ) )
        return FALSE;

    m_Handle = tmp;

	// Serialize Property table
	PropertyTable::Iterator it;
	const PropertyTable& pTable = GetPropertyTable();

	if( ar.IsStoring() )
	{
		ForEachItem( it, pTable )
		{
            if( (*it)->GetIsAutoSerialized() )
            {
			    Sb::StringCopy( (*it)->GetName(), pName, 256 );
			    Sb::StringCopy( (*it)->GetTypeInfo().GetTypeName(), pType, 256 );

			    if( ar.BeginScope( WT("Property"), version ) == version )
			    {
				    ar.Serialize( pName, 256, WT("Name") );
				    ar.Serialize( pType, 256, WT("Type") );
				    (*it)->Serialize( ar );
				    ar.EndScope();
			    }
            }
		}
	}
	else
	{
		ForEachItem( it, pTable )
		{
			if( (*it)->GetIsAutoSerialized() )
			{
				if( ar.BeginScope( WT("Property"), version ) == version )
				{
					ar.Serialize( pName, 256, WT("Name") );
					ar.Serialize( pType, 256, WT("Type") );
					CPropertyInterface* p =  GetProperty( pName );

					if( p )
					{
						if( Sb::StringCompare( p->GetTypeInfo().GetTypeName(), pType ) == 0 )
						{
							p->Serialize( ar );
						}
					}

					ar.EndScope();
				}
			}
		}
	}

    return Sb::CManagedObject::Serialize( ar );
}

String CBankObject::GetName() const
{
    CPropertyString* nameProp = (CPropertyString*)GetProperty( PropName );
    return nameProp->GetValue();
}

Void CBankObject::SetName( String n ) const
{
    CPropertyString* nameProp = (CPropertyString*)GetProperty( PropName );
    nameProp->SetValue( n );
}

Void CBankObject::SetChildIndex( Int32 oldChildIndex, Int32 newChildIndex )
{
	if( oldChildIndex == newChildIndex )
		return;

	m_Childs->MoveItem( oldChildIndex, newChildIndex );
	SignalChildIndexChanged( GetHandle(), oldChildIndex, newChildIndex );

}

CBankObject* CBankObject::Clone()
{
	CBankObject* clonedObject = m_Bank->AddObject( GetTypeInfo(), GetParent() );

	if( !clonedObject )
		return NULL;

	clonedObject->SetName( GetName() );

	PropertyTable::Iterator it;
	const PropertyTable& pTable = GetPropertyTable();

	ForEachItem( it, pTable )
	{
		clonedObject->GetProperty( (*it)->GetName() )->CopyFrom( **it );
	}

	return clonedObject;
}

Void CBankObject::AddDependency( const CHandle& objHandle )
{
    Sb::CList<SDependency>::Iterator it;
    Bool find;
	CBankObject* object = GetObjAccessor()->GetObj( objHandle );

	if( !object )
		return;

    // Add OutputDependency
    find = FALSE;
    ForEachItem( it, m_OutputDependency )
    {
        if( (*it).Handle == objHandle )
        {
            (*it).Count++;
            find = TRUE;
            break;
        }
    }

    if( !find )
    {
        SDependency d;

        d.Count = 1;
        d.Handle = objHandle;
        DebugAssert( d.Handle.IsValid() );
        m_OutputDependency.AddItemAtEnd( d );
    }

    // Add InputDependency
    find = FALSE;
    ForEachItem( it, object->m_InputDependency )
    {
        if( (*it).Handle == GetHandle() )
        {
            (*it).Count++;
            find = TRUE;
            break;
        }
    }

    if( !find )
    {
        SDependency d;

        d.Count = 1;
        d.Handle = GetHandle();
        DebugAssert( d.Handle.IsValid() );
        object->m_InputDependency.AddItemAtEnd( d );
    }

	//SignalObjectDependencyAdded( o );
}

Void CBankObject::RemoveDependency( const CHandle& objHandle )
{
    Sb::CList<SDependency>::Iterator it;
    Bool find;
    SDependency* d;
	CBankObject* obj = GetObjAccessor()->GetObj( objHandle );

	if( !obj )
		return;

    // Add OutputDependency
    find = FALSE;
    d = NULL;
    ForEachItem( it, m_OutputDependency )
    {
        if( (*it).Handle == objHandle )
        {
            (*it).Count--;
            d = &(*it);
            break;
        }
    }

    if( d && d->Count == 0 )
    {
        m_OutputDependency.RemoveItem( *d );
    }

    // Add InputDependency
    find = FALSE;
    d = NULL;
    ForEachItem( it, obj->m_InputDependency  )
    {
        if( (*it).Handle == GetHandle() )
        {
            (*it).Count--;
            d = &(*it);
            break;
        }
    }

    if( d && d->Count == 0 )
    {
        obj->m_InputDependency.RemoveItem( *d );
    }
}

const CBankObject::DependencyList& CBankObject::GetOutputDependencyList() const
{
    return m_OutputDependency;
}

const CBankObject::DependencyList& CBankObject::GetInputDependencyList() const
{
    return m_InputDependency;
}

Void CBankObject::PrepareBakeObjectSet(CBakeContext& context, CBakeObjectSet& out ) const
{
}

Void CBankObject::BakeObjectSet(CBakeContext& context, CBakeObjectSet& out )
{
}

Void CBankObject::InitializeObjectSet(CBakeContext& context, CBakeObjectSet& out )
{
}

const CHandle& CBankObject::GetParent( ) const
{
    return m_Parent->GetValue();
}

Void CBankObject::SetParent( const CHandle& newParent )
{
    if( newParent == m_Parent->GetValue() )
        return;

	CBankObject* oldParentAsset = GetObjAccessor()->GetObj( m_Parent->GetValue() );
	if( oldParentAsset )
	{
		oldParentAsset->RemoveChild( GetHandle() );
	}

	CBankObject* newParentAsset = GetObjAccessor()->GetObj( newParent );
	if( newParentAsset )
	{
		newParentAsset->AddChild( GetHandle() );
	}

    m_Parent->SetValue( newParent );
}

Void CBankObject::AddChild( const CHandle& elem )
{
    if( !elem.IsValid() )
        return;

    // Avoid to ad twice the same entry
    UInt32 i = 0;
    for( i=0; i< m_Childs->GetItemCount(); i++ )
    {
        CPropertyHandle* p = CastProperty( m_Childs->GetItemAtIndex( i ), CPropertyHandle );
        if( p && p->GetValue() == elem )
            return;
    }

	// Add a new property to store the reference
    CPropertyHandle* newProperty = NEWOBJ( CPropertyHandle, ( "" ) );
	m_Childs->AddItemAtEnd( *newProperty );
	newProperty->SetValue( elem );

	SignalChildAdded( elem );
}

/**
Return the 0 based index of a given child or -1 if the given handle is not a child
*/
Int32 CBankObject::GetChildIndex( const CHandle& childHandle ) const
{
	for( UInt32 i=0;i<m_Childs->GetItemCount();i++ )
	{
		if( ((const CPropertyHandle&)m_Childs->GetItemAtIndex(i)).GetValue() == childHandle )
			return i;
	}

	return -1;
}

const CHandle& CBankObject::GetChildAtIndex( UInt32 index ) const
{
    CPropertyHandle* p = CastProperty( m_Childs->GetItemAtIndex(index), CPropertyHandle );
    return p->GetValue();
}

UInt32  CBankObject::GetChildCount() const
{
    return m_Childs->GetItemCount();
}

Void CBankObject::DetachFromParent( )
{
    CBankObject* asset = GetObjAccessor()->GetObj( m_Parent->GetValue() );
    if( asset )
    {
        asset->RemoveChild( GetHandle() );
        m_Parent->SetValue( CHandle() );
    }
}
/**
This method is automaticly called when the value of a parameter have changed.
\param p The parameter that just change.
\note At this point, p already hold the new parameter value
*/
Void CBankObject::OnPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue )
{
	SignalPropertyChanged( p, oldValue );
}

Void CBankObject::RemoveChild( const CHandle& elem )
{
    UInt32 i;

    for( i=0; i<GetChildCount(); i++ )
    {
        if( GetChildAtIndex( i ) == elem )
            break;
    }

    // If we find the asset, remove it
    if( i<GetChildCount() )
    {
        m_Childs->RemoveItemAtIndex( i );
    }
}

Void CBankObject::FindChildByType( const Sb::CTypeInfo& type, Sb::CList<CHandle>& result ) const
{
	FindChildByType( GetHandle(), type, result );
}

Void CBankObject::FindChildByType( const CHandle& root, const Sb::CTypeInfo& type, Sb::CList<CHandle>& result ) const
{
	CBankObject* rootObj = GetObjAccessor()->GetObj( root );
	for( UInt32 i=0;i<rootObj->GetChildCount();i++ )
	{
		CBankObject* childObj = GetObjAccessor()->GetObj( rootObj->GetChildAtIndex( i ) );
		if( childObj->GetTypeInfo().IsKindOf( type ) )
		{
			result.AddItemAtEnd( childObj->GetHandle() );
		}

		FindChildByType( childObj->GetHandle(), type, result );
	}
}