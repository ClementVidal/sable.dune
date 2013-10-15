#include <Dune\Model\Bank\Bank.h>

#include <Dune\Model\Bank\Handle.h>
#include <Dune\Common\Log.h>
#include <Dune\Bake\DependencySolver.h>

#include <Sable\Core\Collection\Pair.h>

IMPLEMENT_MANAGED_CLASS( Dune, CBank );

using namespace Dune;


CBank::CIterator::CIterator( )
{
    m_Index = 0;
    m_Bank = NULL;
}

CBank::CIterator::CIterator( const CBank* bank  )
{
    m_Index = 0;
    m_Bank = bank;
    while( m_Index < m_Bank->m_Table.GetItemCount() )
    {
        if(  m_Bank->m_Table[m_Index].Object != NULL )
            break;

        m_Index++;
    }
}

CBankObject* CBank::CIterator::operator -> () const
{
	return m_Bank->m_Table[m_Index].Object;
}

CBankObject& CBank::CIterator::operator * () const
{
    return *m_Bank->m_Table[m_Index].Object;
}

Bool CBank::CIterator::GetIsValid() const 
{
    return ( m_Index < m_Bank->m_Table.GetItemCount() ) && ( m_Bank->m_Table[m_Index].Object != NULL );
}

CBankObject& CBank::CIterator::GetData() const
{
    return *m_Bank->m_Table[m_Index].Object;
}

Void CBank::CIterator::Iterate()
{
    m_Index++;

    while( m_Index < m_Bank->m_Table.GetItemCount() )
    {
        if(  m_Bank->m_Table[m_Index].Object != NULL )
            break;

        m_Index++;
    }
}

CBank::SEntry& CBank::SEntry::operator=( const CBank::SEntry& other )
{
    Object = other.Object;
    Spin = other.Spin;
    NextFree = other.NextFree;

    return *this;
}

CBank::CBank( )
{
	m_Table.SetItemCount( 1024 );
	m_BindedObjectCount = 0;
	m_ReservedObjectCount = 0;
	m_Project = NULL;
	m_Uuid = CUuid::Create();
	m_Name = "";

	Clear();
}

CBank::CBank( CProject& project, String name )
{
    m_Table.SetItemCount( 1024 );
    m_BindedObjectCount = 0;
    m_ReservedObjectCount = 0;
    m_Project = &project;
    m_Uuid = CUuid::Create();
    m_Name = name;

    Clear();
}

CBank::~CBank()
{
    Clear();
}

Void CBank::StartIteration( CBank::Iterator& iterator ) const 
{
    iterator = Iterator( this );
}

CProject* CBank::GetProject() const
{
    return m_Project;
}

const CUuid& CBank::GetUuid() const
{
    return m_Uuid;
}

/**
This method should only be used by CProject in order to restore he right Uuid for the system bank.
*/
void CBank::SetUuid( const CUuid& uuid )
{
    if( m_Uuid == uuid )
        return;

    m_Uuid = uuid;

    Iterator it;
    ForEachItem( it, *this )
    {
        CHandle oldHandle = it->GetHandle();
        CHandle newHandle( GetUuid(), oldHandle.GetIndex(), oldHandle.GetSpin() ) ;

        it.GetData().SetHandle( newHandle );
    }
}

String CBank::GetName() const
{
    return m_Name;
}

Void CBank::SetName( String name )
{
	m_Name = name;
}

Bool CBank::Serialize( Sb::CPersistentArchive& ar )
{
    UInt32 i, index;
    Int16 version = 0;

    ar.Serialize( m_Name, WT("Name") );
    ar.Serialize( m_Uuid, WT("Buid") );
    ar.Serialize( m_FirstFree, WT("FirstFree") );
    ar.Serialize( m_BindedObjectCount, WT("BindedObjectCount") );

    if( ar.IsStoring() )
    {
        for( i=0; i<m_Table.GetItemCount(); i++ )
        {
            SEntry& entry = m_Table[i];

            if( entry.Object )
            {
                index = i;
                version = 0;
                if( ar.BeginScope( WT("Entry"), version ) == version )
                {
                    ar.Serialize( index, WT("Index") );
                    ar.Serialize( entry.Spin, WT("Spin") );
                    ar.Serialize( entry.Object, WT("Obj"), NULL );
                    ar.EndScope();
                }
            }
        }
    }
    else 
    {
        for( i=0; i<m_BindedObjectCount; i++ )
        {
            SEntry entry;
            Void* args[1];
            args[0] = this;

            index = i;
            version = 0;
            entry.NextFree = -1;
            if( ar.BeginScope( WT("Entry"), version ) == version )
            {
                ar.Serialize( entry.Spin, WT("Spin") );
                ar.Serialize( index, WT("Index") );
                ar.Serialize( entry.Object, WT("Obj"), NULL );
				if( entry.Object )
					entry.Object->SetBank( *this );
                ar.EndScope();
            }

            DebugAssert( m_Table[index].Object == NULL );
            m_Table[index] = entry;
        }

        Int32 firstFree = -1;
        for( i=0; i<m_Table.GetItemCount() ;i++ )
        {
            if( m_Table[i].NextFree != -1 )
            {
                if( firstFree == -1 )
                {
                    firstFree = i;
                    m_FirstFree = firstFree;
                }
                else
                {
                    m_Table[firstFree].NextFree = i;
                    firstFree = i;
                }
            }
        }
        m_Table[m_Table.GetItemCount()-1].NextFree = -1;
    }

    return TRUE;

}

Void CBank::Clear()
{
    UInt32 i;

    for( i=0;i<m_Table.GetItemCount();i++)
    {
        if( m_Table[i].Object )
        {
            // This should delete the object
            m_Table[i].Object = NULL;
        }
        m_Table[i].Spin = -1;
        m_Table[i].NextFree = i+1;
    }

    m_Table[m_Table.GetItemCount()-1].NextFree = -1;
    m_FirstFree = 0;
    m_BindedObjectCount = 0;
    m_ReservedObjectCount = 0;
}

/**

*/
Bool CBank::IsReserved( const CHandle& handle ) const
{
	if( handle.GetSpin() != -1 && handle.GetIndex() < (Int32)m_Table.GetItemCount() )
	{
		if( handle.GetSpin() == m_Table[handle.GetIndex()].Spin )
			return TRUE;
	}
	return FALSE;
}

/**
A handle is binded if it has been reserved and if an object is attached to this handle.
*/
Bool CBank::IsBinded( const CHandle& handle ) const
{
    return IsReserved( handle ) &&  m_Table[handle.GetIndex()].Object;
}

Bool CBank::RemoveObject( CHandle h, Bool freeHandle )
{
	if( h.IsValid() )
	{
        SignalObjectRemoved( *m_Table[h.GetIndex()].Object );

		// Delete the object
		m_Table[h.GetIndex()].Object->SetHandle( CHandle() );
		m_Table[h.GetIndex()].Object = NULL;
		m_BindedObjectCount --;

		if( freeHandle )
		{
			return FreeHandle( h );	
		}
	}

	return TRUE;
}

/**
Establish a link between an object and a given handle located in this bank
*/
Void CBank::BindObject( const CHandle& h , CBankObject& obj )
{
	DebugAssert( IsReserved( h ) );
	DebugAssert( m_Table[h.GetIndex()].Spin == h.GetSpin() );
	
	m_Table[h.GetIndex()].Object = &obj;
	obj.SetHandle( h );

	obj.SetBank( *this );

	m_BindedObjectCount++;

    SignalObjectAdded( obj );
}

/**
Reserve an Handle, this handle is not binded to any object, 
but it can't be re used in any subsequent handle allocation
*/
CHandle CBank::ReserveHandle()
{
	CHandle handle( GetUuid() );
	Int32 i = m_FirstFree;

	if( m_FirstFree == -1 )
	{
		return handle;
	}

	DebugAssert( m_Table[i].Object == NULL );

	m_Table[i].Object = NULL;
	m_Table[i].Spin++;
	m_FirstFree = m_Table[i].NextFree;
	m_Table[i].NextFree = -1;

	handle.SetSpin( m_Table[i].Spin );
	handle.SetIndex( i );

	return handle;
}

Bool CBank::FreeHandle( const CHandle& h )
{
	CBankObject* obj;

	if( !IsReserved( h ) )
		return FALSE;

	Int32 lastFirstFree = m_FirstFree;
	m_FirstFree = h.GetIndex();
	obj = m_Table[m_FirstFree].Object;
	// thie should delete the object
	m_Table[m_FirstFree].Object = NULL;
	m_Table[m_FirstFree].NextFree = lastFirstFree;

	return TRUE;

}

CBankObject* CBank::AddObject( const Sb::CTypeInfo& type, const CHandle& parent )
{
    CHandle handle( GetUuid() );

    if( type.HasAllocator() == FALSE )
    {
        Sb::DebugLogInfo("CBank::Add - Failled to add object of type \"%s\", No type allocator defined for it", type.GetTypeName() );
		return NULL;
    }

    CBankObject* newObj = (CBankObject*)type.CreateObject( NULL );

    if( newObj == NULL )
        return NULL;

	handle = ReserveHandle();

    // After the object's creation, and after the object is binded to a given parent, signal the system that it was created
    BindObject( handle, *newObj );

    if( parent.IsValid() )
    {
        // Attach to it's parent
        if( handle.IsValid() && parent.IsValid())
        {
			CBankObject* elem = GetObj( handle );
			if( m_Project )
				elem = m_Project->GetObj( handle );

			if( elem )
			{
				CBankObject* parentElem = GetObj( parent );
				if( m_Project )
					parentElem = m_Project->GetObj( parent );

				DebugAssert( parentElem );
				elem->SetParent( parent );
			}
        }
    }

    return newObj;

}

/**
Merge a given object AND ALL OF IT'S DEPENDENCY into this bank
*/
Bool CBank::Merge( const CBankObject& mergedObject, const CHandle& newRoot, Sb::CMap<CHandle,CHandle>& mappingTable  )
{
	Sb::CList<CHandle> handleList;

	CBankObjectAccessor* objAccessor = mergedObject.GetObjAccessor();
	if( !objAccessor )
		objAccessor = mergedObject.GetBank();

	DebugAssert( objAccessor );

	// Compute dependency
	handleList.AddItemAtEnd( mergedObject.GetHandle() );
	CBakeDependencySolver depsSolver( *objAccessor );
	depsSolver.Resolve( handleList );

	// Merge obj list
	return Merge( handleList, newRoot,*objAccessor,  mappingTable );
}


Void CBank::RemapHandleProperty( const CPropertyHandle& srcProp, CPropertyHandle& dstProp, const CHandle& newRoot, Sb::CMap<CHandle,CHandle>& mapTable ) const
{
	// 1. Copy the old property to the new one
	dstProp.CopyFrom( srcProp );

	if( !Sb::StringCompare( srcProp.GetName(), "Parent" ) )
	{
		dstProp.SetValue( newRoot );
	}
	else
	{
		// 2. Change the value using the mapped one
		if( mapTable.HasItem( srcProp.GetValue() ) )
			dstProp.SetValue( mapTable.GetItem( srcProp.GetValue() ) );
		else
			dstProp.SetValue( CHandle() );
	}
}

/**

*/
Bool CBank::Merge( const Sb::CList<CHandle>& srcObjList, const CHandle& newRoot, const CBankObjectAccessor& srcObjAccessor, Sb::CMap<CHandle,CHandle>& mappingTable )
{
	Sb::CMap<CHandle,CHandle>::Iterator it;

	Sb::CList<CHandle>::Iterator itSrcObj;

	// Add each object from the other bank to this one
	ForEachItem( itSrcObj, srcObjList )
	{
		CBankObject* srcObj = srcObjAccessor.GetObj( *itSrcObj );
		CBankObject* dstObj = AddObject( srcObj->GetTypeInfo(), CHandle() );
		mappingTable.AddItem( srcObj->GetHandle(), dstObj->GetHandle() );
	}

	// For each object copy properties value and correct the handle values
	ForEachItem( it, mappingTable )
	{
		CBankObject* srcObj = srcObjAccessor.GetObj( it.GetKey() );
		CBankObject* dstObj = GetObj( *it );

		CBankObject::PropertyTable::Iterator itProp;
		ForEachItem( itProp, srcObj->GetPropertyTable() )
		{
			CPropertyInterface* srcProp = *itProp;
			CPropertyInterface* dstProp = dstObj->GetProperty( srcProp->GetName() );

			// If property does not exist in target object, create it
			if( !dstProp )
			{
				dstProp = (CPropertyInterface*)srcProp->GetTypeInfo().CreateObject( NULL );
				dstProp->Initialize( srcProp->GetName() );
				dstObj->AddProperty( * dstProp );
			}
			
			// Translate PropertyHandle value
			if( srcProp->GetTypeInfo().IsKindOf( CPropertyHandle::GetStaticTypeInfo() ) )
			{				
				RemapHandleProperty( (CPropertyHandle&) *srcProp, (CPropertyHandle&) *dstProp, newRoot, mappingTable );
			}
			// Translate PropertyArray that containt PropertyHandle value
			else if( srcProp->GetTypeInfo().IsKindOf( CPropertyArray::GetStaticTypeInfo() ) )
			{
				CPropertyArray* arraySrcProp = (CPropertyArray*) srcProp;
				CPropertyArray* arrayDstProp = (CPropertyArray*) dstProp;

				// 1. Copy the old property to the new one
				arrayDstProp->CopyFrom( *arraySrcProp );
				// 2. Change the value using the mapped one
				if( arraySrcProp->GetItemTypeInfo().IsKindOf( CPropertyHandle::GetStaticTypeInfo() ) )
				{
					for( Int32 i=0; i< (Int32)arraySrcProp->GetItemCount(); i++ )
					{
						RemapHandleProperty( (CPropertyHandle&) arraySrcProp->GetItemAtIndex(i),  (CPropertyHandle&) arrayDstProp->GetItemAtIndex( i ), newRoot, mappingTable );
					}
				}
			}
			else
			{
				dstProp->CopyFrom( *srcProp );
			}
		}
	}

	return TRUE;
}

Bool CBank::Merge( const CBank& mergedBank, const CHandle& newRoot, Sb::CMap<CHandle,CHandle>& mappingTable )
{
	if( m_Project == NULL )
		return FALSE;

	Sb::CList<CHandle> objList;

	CBank::Iterator itSrcObj;

	// Add each object from the other bank to this one
	ForEachItem( itSrcObj, mergedBank )
	{
		objList.AddItemAtEnd( itSrcObj->GetHandle() );
	}

	return Merge( objList, newRoot, mergedBank, mappingTable );
}

CBankObject* CBank::GetObj( const CHandle& h ) const
{
	CBankObject* obj = NULL;
	
	// Try to find the obj in this bank
	if( IsBinded(h) )
	{
		obj = m_Table[h.GetIndex()].Object;
	}

	return obj;
}

/**
Execute a query over the repo based on a string expression.

Supported expression can be as follow:

- AttribName = Value :
    - AttribName: the name of a specific property in the object
    - Value: a string value used to compare.

- IsKindOf( TypeName ) :
    - TypeName: The name of a C++ class type ( like CWidgetFrame ... )

- * :
    - Grab every object of the database
*/
Bool CBank::IssueQuery( String text, Sb::CList<CHandle>& result ) const
{
	UInt32 j = 0;
    QString textToMatch( text );
    CPropertyInterface* p;

    // RegexOf type *
    if( Sb::StringCompare( text, "*" ) == 0)
    {
		CBank::Iterator it;
		ForEachItem( it, *this )
		{
			CBankObject& obj = it.GetData();
			DebugAssert( obj.GetHandle().IsValid() );	
			result.AddItemAtEnd( obj.GetHandle() );
		}
    }

    // RegexOf type AttribName = Value
    {
        QString prop, value;
        QRegExp regex("(([[:alnum:]])+)([[:blank:]])*=([[:blank:]])*(([[:alnum:] _-])+)");
        if ( regex.indexIn(textToMatch) != -1 )
        {
            prop = regex.cap( 1 );
            value = regex.cap( 5 );

			CBank::Iterator it;
			ForEachItem( it, *this )
			{
				CBankObject& obj = it.GetData();
				DebugAssert( obj.GetHandle().IsValid() );
				p = obj.GetProperty( prop.toLocal8Bit());
				if( p && QString( p->GetValueAsString() ) == value )
				{
					result.AddItemAtEnd( obj.GetHandle() );
				}
			}
        }
    }
    // RegexOf type IsKindOf( TypeName )
    {
        QString type;
        QRegExp regex("IsKindOf\\(\\s*(\\w*)\\s*\\)");
        if ( regex.indexIn(textToMatch) != -1 )
        {
			type = "Dune::"+regex.cap( 1 );

			CBank::Iterator it;
			ForEachItem( it, *this )
			{
				CBankObject& obj = it.GetData();
				DebugAssert( obj.GetHandle().IsValid() );	
				if( obj.GetTypeInfo().IsKindOf( type.toLocal8Bit() ) )
					result.AddItemAtEnd( obj.GetHandle() );
			}
		}
    }
	return TRUE;
}
