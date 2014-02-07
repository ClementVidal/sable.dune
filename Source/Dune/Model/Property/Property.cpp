#include <Dune/Model/Property/Property.h> 

#include <Sable/Core/String/String.h>

#include <Dune/Common/LexicalCast.h>

using namespace Dune;


/**
Enum property
*/

IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CPropertyEnum, CPropertyInterface  );

CPropertyEnum::CDef::CDef( ) :
    m_EntryTable( NULL )
{
    m_EntryCount = 0;
}

CPropertyEnum::CDef::CDef( const SEntry def[] ) :
    m_EntryTable( def )
{
    m_EntryCount = 0;

    while( def[m_EntryCount].Name )
    {
        m_EntryCount++;
    }

    DebugAssert( m_EntryCount > 0 );
}

UInt32 CPropertyEnum::CDef::GetEntryCount() const
{
    return m_EntryCount;
}

String CPropertyEnum::CDef::GetEntryName( UInt32 i ) const
{
    DebugAssert( i < GetEntryCount() );
    return m_EntryTable[i].Name;
}

Int32 CPropertyEnum::CDef::GetEntryValue( UInt32 i ) const
{
    DebugAssert( i < GetEntryCount() );
    return m_EntryTable[i].Value;
}

CPropertyEnum::CPropertyEnum( ) :
    CPropertyConcrete<Int32>( )
{

}

CPropertyEnum::CPropertyEnum( String name, const CDef& def ) :
    m_Def( def ),
    CPropertyConcrete<Int32>( name, def.GetEntryValue( 0 ) )
{

}

const CPropertyEnum::CDef& CPropertyEnum::GetDefinition() const
{
    return m_Def;
}

Sb::CString CPropertyEnum::GetValueAsString() const
{
    UInt32 i;
    for( i=0;i<m_Def.GetEntryCount();i++)
    {
        if( m_Def.GetEntryValue( i ) == GetValue() )
            return m_Def.GetEntryName( i );
    }

    return "Invalid Value";
}


Void CPropertyEnum::SetValue( const Int32& value )
{
    if( IsValidValue( value ) )
    {
        CPropertyConcrete<Int32>::SetValue( value );
    }
}

Bool CPropertyEnum::IsValidValue( const Int32& value )
{
    UInt32 i;
    for( i=0;i<m_Def.GetEntryCount();i++)
    {
        if( m_Def.GetEntryValue( i ) == value )
            return TRUE;
    }

    return FALSE;
}

Bool CPropertyEnum::SetValueFromString( String v )
{
    UInt32 i;
    for( i=0;i<m_Def.GetEntryCount();i++)
    {
        if( Sb::StringCompare( m_Def.GetEntryName( i ), v ) == 0 )
        {
            CPropertyConcrete<Int32>::SetValue( m_Def.GetEntryValue( i ) );
            return TRUE;
        }
    }

	return FALSE;
}


/**
Int32 property
*/
IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CPropertyInt32, CPropertyInterface  );
CPropertyInt32::CPropertyInt32( ) :
    CPropertyConcrete<Int32>( )
{
}

CPropertyInt32::CPropertyInt32( String name ) :
    CPropertyConcrete<Int32>( name, 0 )
{
}

Sb::CString CPropertyInt32::GetValueAsString() const
{
    Sb::CString s;
    s.WriteFormattedText("%d", GetValue() );
    return s;
}

Bool CPropertyInt32::SetValueFromString( String v )
{
    SetValue( Sb::StringToInt32( v ) );
	return TRUE;
}

/**
Bool property
*/
IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CPropertyBool, CPropertyInterface );

CPropertyBool::CPropertyBool( ) :
    CPropertyConcrete<Bool>( )
{
}

CPropertyBool::CPropertyBool( String name ) :
    CPropertyConcrete<Bool>( name, FALSE )
{
}

Sb::CString CPropertyBool::GetValueAsString() const
{
	return CLexicalCast::ValueToString( m_Value );
}

Bool CPropertyBool::SetValueFromString( String v )
{
	Bool value;
	if( CLexicalCast::StringToValue( v, value ) )
	{
		SetValue( value );
		return TRUE;
	}
	return FALSE;
}

/**
Float32 property
*/
IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CPropertyFloat32, CPropertyInterface );

CPropertyFloat32::CPropertyFloat32( ) :
CPropertyConcrete<Float32>( )
{
}

CPropertyFloat32::CPropertyFloat32( String name ) :
    CPropertyConcrete<Float32>( name, 0.0f )
{
}

Sb::CString CPropertyFloat32::GetValueAsString() const
{
	return CLexicalCast::ValueToString( m_Value );
}

Bool CPropertyFloat32::SetValueFromString( String v )
{
	Float32 value;
	if( CLexicalCast::StringToValue( v, value ) )
	{
		SetValue( value );
		return TRUE;
	}
	return FALSE;
}

/**
Sb::CVector3f property
*/
IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CPropertyVector3f, CPropertyInterface );

CPropertyVector3f::CPropertyVector3f( ) :
    CPropertyConcrete<Sb::CVector3f>()
{
}

CPropertyVector3f::CPropertyVector3f( String name ) :
    CPropertyConcrete<Sb::CVector3f>( name, Sb::CVector3f() )
{
}

Sb::CString CPropertyVector3f::GetValueAsString() const
{
	return CLexicalCast::ValueToString( m_Value );
}

Bool CPropertyVector3f::SetValueFromString( String v )
{
	Sb::CVector3f value;
	if( CLexicalCast::StringToValue( v, value ) )
	{
		SetValue( value );
		return TRUE;
	}
	return FALSE;
}

Bool CPropertyVector3f::Serialize( Sb::CPersistentArchive& ar )
{
	return CPropertyConcrete<Sb::CVector3f>::Serialize( ar );
}

/**
Sb::CVector3f property
*/
IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CPropertyVector4f, CPropertyInterface );

CPropertyVector4f::CPropertyVector4f( ) :
    CPropertyConcrete<Sb::CVector4f>()
{
}

CPropertyVector4f::CPropertyVector4f( String name ) :
    CPropertyConcrete<Sb::CVector4f>( name, Sb::CVector4f() )
{
}

Sb::CString CPropertyVector4f::GetValueAsString() const
{
	return CLexicalCast::ValueToString( m_Value );
}

Bool CPropertyVector4f::SetValueFromString( String v )
{
	Sb::CVector4f value;
	if( CLexicalCast::StringToValue( v, value ) )
	{
		SetValue( value );
		return TRUE;
	}
	return FALSE;
}

Bool CPropertyVector4f::Serialize( Sb::CPersistentArchive& ar )
{
	return CPropertyConcrete<Sb::CVector4f>::Serialize( ar );
}

/**
Sb::CVector3f property
*/
IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CPropertyMatrix4x4, CPropertyInterface );

CPropertyMatrix4x4::CPropertyMatrix4x4( ) :
    CPropertyConcrete<Sb::CMatrix4x4f>( )
{
    Sb::CMatrix4x4f m;
    m.SetFromIdentity();
    SetValue( m );
}

CPropertyMatrix4x4::CPropertyMatrix4x4( String name ) :
    CPropertyConcrete<Sb::CMatrix4x4f>( name, Sb::CMatrix4x4f() )
{
    Sb::CMatrix4x4f m;
    m.SetFromIdentity();
    SetValue( m );
}

Sb::CString CPropertyMatrix4x4::GetValueAsString() const
{
	return CLexicalCast::ValueToString( m_Value );
}

Bool CPropertyMatrix4x4::SetValueFromString( String v )
{
	Sb::CMatrix4x4f value;
	if( CLexicalCast::StringToValue( v, value ) )
	{
		SetValue( value );
		return TRUE;
	}
	return FALSE;
}

Bool CPropertyMatrix4x4::Serialize( Sb::CPersistentArchive& ar )
{
	return CPropertyConcrete<Sb::CMatrix4x4f>::Serialize( ar );
}


/**
Sb::CColor property
*/
IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CPropertyColor, CPropertyInterface );

CPropertyColor::CPropertyColor( ) :
    CPropertyConcrete<Sb::CColor>( )
{
}

CPropertyColor::CPropertyColor( String name ) :
    CPropertyConcrete<Sb::CColor>( name, Sb::CColor() )
{
}

Sb::CString CPropertyColor::GetValueAsString() const
{
	return CLexicalCast::ValueToString( m_Value );
}

Bool CPropertyColor::SetValueFromString( String v )
{
	Sb::CColor value;
	if( CLexicalCast::StringToValue( v, value ) )
	{
		SetValue( value );
		return TRUE;
	}
	return FALSE;
}

/**
Sb::CVector2f property
*/
IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CPropertyVector2f, CPropertyInterface );

CPropertyVector2f::CPropertyVector2f() :
CPropertyConcrete<Sb::CVector2f>()
{
}

CPropertyVector2f::CPropertyVector2f( String name ) :
    CPropertyConcrete<Sb::CVector2f>( name, Sb::CVector2f() )
{
}

Sb::CString CPropertyVector2f::GetValueAsString() const
{
	return CLexicalCast::ValueToString( m_Value  );
}

Bool CPropertyVector2f::SetValueFromString( String v )
{
	Sb::CVector2f value;
	if( CLexicalCast::StringToValue( v, value ) )
	{
		SetValue( value );
		return TRUE;
	}
	return FALSE;
}

Bool CPropertyVector2f::Serialize( Sb::CPersistentArchive& ar )
{
	return 	CPropertyConcrete<Sb::CVector2f>::Serialize( ar );
}

/**
Sb::CVector2f property
*/
IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CPropertyVector2i, CPropertyInterface );

CPropertyVector2i::CPropertyVector2i() :
	CPropertyConcrete<Sb::CVector2i>()
{
}

CPropertyVector2i::CPropertyVector2i( String name ) :
    CPropertyConcrete<Sb::CVector2i>( name, Sb::CVector2i() )
{
}

Sb::CString CPropertyVector2i::GetValueAsString() const
{
	return CLexicalCast::ValueToString( m_Value  );
}

Bool CPropertyVector2i::SetValueFromString( String v )
{
	Sb::CVector2i value;
	if( CLexicalCast::StringToValue( v, value ) )
	{
		SetValue( value );
		return TRUE;
	}
	return FALSE;
}

Bool CPropertyVector2i::Serialize( Sb::CPersistentArchive& ar )
{
	return 	CPropertyConcrete<Sb::CVector2i>::Serialize( ar );
}

/**
Sb::CFilePath property
*/
IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CPropertyFilePath, CPropertyInterface );

CPropertyFilePath::CPropertyFilePath( ) :
CPropertyConcrete<Sb::CFilePath>( )
{
}

CPropertyFilePath::CPropertyFilePath( String name ) :
    CPropertyConcrete<Sb::CFilePath>( name, Sb::CFilePath() )
{
}

Bool CPropertyFilePath::Serialize( Sb::CPersistentArchive& ar )
{
    Char path[256];

    Sb::StringCopy( GetValue().GetPath(), path, 256 );
    ar.Serialize( path, 256, WT("Value") );

   // ar.Serialize( m_Wildcard, WT("Wildcard") );

    if( ar.IsLoading() )
    {
        m_Value = Sb::CFilePath( path );
    }
	return CPropertyInterface::Serialize( ar );
}

Void CPropertyFilePath::SetWildcard( String wildcard )
{
    m_Wildcard = wildcard;
}

String  CPropertyFilePath::GetWildcard() const
{
    return m_Wildcard;
}

Sb::CString CPropertyFilePath::GetValueAsString() const
{
	return CLexicalCast::ValueToString( m_Value );
}

Bool CPropertyFilePath::SetValueFromString( String v )
{
	Sb::CFilePath value;
	if( CLexicalCast::StringToValue( v, value ) )
	{
		SetValue( value );
		return TRUE;
	}
	return FALSE;
}

/**
Sb::CString property
*/
IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CPropertyString, CPropertyInterface );

CPropertyString::CPropertyString() :
    CPropertyConcrete< Sb::CString >( )
{
}

CPropertyString::CPropertyString( String name, EType type  ) :
    CPropertyConcrete< Sb::CString >( name, "" )
{
	m_Type = type;
}

CPropertyString::EType CPropertyString::GetType() const
{
	return m_Type;
}

Bool CPropertyString::Serialize( Sb::CPersistentArchive& ar )
{
    //ar.Serialize( (UInt32&)m_Type, WT("EditType") );
	return CPropertyConcrete< Sb::CString >::Serialize( ar );
}

Sb::CString CPropertyString::GetValueAsString() const
{
	return Sb::CString( m_Value );
}

Bool CPropertyString::SetValueFromString( String v )
{
	Sb::CString value;
	if( CLexicalCast::StringToValue( v, value ) )
	{
		SetValue( value.GetBuffer() );
		return TRUE;
	}
	return FALSE;
}

/**
CHandle property
*/
IMPLEMENT_MANAGED_CLASS1_ALLOC( Dune, CPropertyHandle, CPropertyInterface );

CPropertyHandle::CPropertyHandle() :
CPropertyConcrete<CHandle>( )
{
    m_AddDependency = TRUE;
	m_SerializeFilter = FALSE;
}

CPropertyHandle::CPropertyHandle( String name ) :
    CPropertyConcrete<CHandle>( name, CHandle() )
{
	m_AddDependency = TRUE;
	m_SerializeFilter = FALSE;
}

CPropertyHandle::~CPropertyHandle()
{
    // Remove any old dependency
    if( GetValue().IsValid() )
	{
        GetObj()->RemoveDependency( GetValue() );
	}
}

Void CPropertyHandle::AddDependencyToOwner( Bool onOff )
{
    m_AddDependency = onOff;
}

/**
Filter used to ensure type checking
*/
Void CPropertyHandle::AddFilter( const Sb::CTypeInfo& i )
{
    DebugAssert( i.IsKindOf( CBankObject::GetStaticTypeInfo() ) );

    m_FilterList.AddItemAtEnd( &i );
}

/**
Check if a given handle math the filtering type list
*/
Bool CPropertyHandle::TypeCheck( const CHandle& objHandle ) const
{
	CBankObject* obj = GetObjAccessor()->GetObj( objHandle );
    if( !obj )
        return TRUE;

    if( m_FilterList.GetItemCount() == 0 )
    {
        return TRUE;
    }
    else
    {
        FilterList::Iterator it;
        ForEachItem( it, m_FilterList )
        {
            if( obj->GetTypeInfo().IsKindOf( *(*it) ) )
                return TRUE;
        }
    }

    return FALSE;
}

Void CPropertyHandle::SetValue( const CHandle& value )
{
    // Remove any old dependency
    if( GetValue().IsValid() )
        GetObj()->RemoveDependency( GetValue() );

    // Set Value
    if( !value.IsValid() )
    {
        CPropertyConcrete<CHandle>::SetValue( value );
    }
    else 
    {
     //   DebugAssert( TypeCheck( value ) );
        CPropertyConcrete<CHandle>::SetValue( value );
        if( m_AddDependency )
            GetObj()->AddDependency( value );
    }
}

Bool CPropertyHandle::CopyFrom( const CPropertyInterface& p )
{
	m_AddDependency = ((const CPropertyHandle&)p).m_AddDependency;
	m_FilterList = ((const CPropertyHandle&)p).m_FilterList;
	m_SerializeFilter = ((const CPropertyHandle&)p).m_SerializeFilter;
	return CPropertyConcrete<CHandle>::CopyFrom( p );
}

const CPropertyHandle::FilterList& CPropertyHandle::GetFilterList() const
{
    return m_FilterList;
}

Bool CPropertyHandle::operator != ( const CPropertyHandle& other )
{
    return GetValue() != other.GetValue();
}

Sb::CString CPropertyHandle::GetValueAsString() const
{
	return CLexicalCast::ValueToString( m_Value );
}

Bool CPropertyHandle::SetValueFromString( String v )
{
	CHandle tmp( GetObj()->GetBank()->GetUuid() ) ;
	if( CLexicalCast::StringToValue( v, tmp ) )
	{
		SetValue( tmp );
		return TRUE;
	}

	return FALSE;
}

Void CPropertyHandle::BindToObject( CBankObject* obj )
{
	CPropertyInterface::BindToObject( obj );

	// Add a dependency to this object
	if( m_Value.IsValid() && m_AddDependency && GetObj() )
		GetObj()->AddDependency( m_Value );
}

Void CPropertyHandle::SetSerializeFilters( Bool onOff )
{
	m_SerializeFilter = onOff;
}

Bool CPropertyHandle::Serialize( Sb::CPersistentArchive& ar )
{
	Int16 version = 0;
	UInt32 i;
    WChar buffer[128];
    Sb::StringToWString( GetName(), buffer, 128 );

    if( ar.IsLoading() )
    {
        CHandle h;

		// Serialize value
        if( ! ar.Serialize( h, WT("Value") ) )
			return NULL;

        m_Value = h;

        // Add a dependency to this object
        if( m_Value.IsValid() && m_AddDependency && GetObj() )
		{
            GetObj()->AddDependency( m_Value );
		}

		if( m_SerializeFilter )
		{
			UInt32 filterCount = 0;
			m_FilterList.ClearAllItems();

			// Serialize filter count
			ar.Serialize( filterCount, WT("FilterCount") );

			// Serialize filters
			for(i=0;i<filterCount;i++)
			{
				if( ar.BeginScope( WT("Filters"), version ) == version )
				{
					Char tmp[128];
					ar.Serialize( buffer, 128, WT("Type") );
					Sb::WStringToString( buffer, tmp, 128 );
					const Sb::CTypeInfo& info = Sb::CManagedObject::GetStaticTypeInfo().GetDerivedClass( tmp );
					if( info == Sb::CTypeInfo::NullType )
					{
						CoreManager.GetLog().LogWarning( "Trying to deserialize proeprty handle filter with an invalid type: %s", tmp );
					}
					else
					{
						DebugAssert( info != Sb::CTypeInfo::NullType );
						m_FilterList.AddItemAtEnd( &info );
					}
					ar.EndScope();
				}
			}
		}
    }
    else
	{
		// Serialize value
        ar.Serialize( m_Value, WT("Value") );

		if( m_SerializeFilter )
		{
			UInt32 filterCount = m_FilterList.GetItemCount();
			// Serialize filter count
			ar.Serialize( filterCount, WT("FilterCount") );
			// Serialize filters
			FilterList::Iterator it;
			ForEachItem( it, m_FilterList )
			{
				if( ar.BeginScope( WT("Filters"), version ) == version )
				{
					const Sb::CTypeInfo* ti = (*it);
					Sb::StringToWString( ti->GetFullTypeName(), buffer, 128 );

					ar.Serialize( buffer, 128, WT("Type") );
				}
				ar.EndScope();
			}
		}
    }

	return CPropertyInterface::Serialize( ar );
}

