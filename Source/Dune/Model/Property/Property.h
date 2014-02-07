#ifndef _DUNE_PROPERTY_PROPERTY_
#define _DUNE_PROPERTY_PROPERTY_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/Common/Color.h>
#include <Sable/Core/File/FilePath.h>
#include <Sable/Core/Collection/Array.h>
#include <Sable/Core/String/StaticString.h>
#include <Sable/Core/Math/Vector3f.h>
#include <Sable/Core/Math/Vector2i.h>
#include <Sable/Core/Math/Vector2f.h>
#include <Sable/Core/Math/Matrix4x4f.h>

#include <Dune/Model/Property/Concrete.h>
#include <Dune/Model/Property/Macro.h>
#include <Dune/Model/Bank/Bank.h>


namespace Dune
{

class CBankObject;
class CPropertyHandle;

/**
Int32 property
*/
class CPropertyEnum : public CPropertyConcrete<Int32>
{

    DEFINE_MANAGED_CLASS( CPropertyEnum )

public:

    struct SEntry
    {
        String Name;
        Int32 Value;
    };

    class CDef
    {
    public:
        CDef( const SEntry def[] );
        CDef( );

        UInt32    GetEntryCount() const;
        String    GetEntryName( UInt32 i ) const;
        Int32     GetEntryValue( UInt32 i ) const;

    private:
        const SEntry* m_EntryTable;
        UInt32      m_EntryCount;
    };

    CPropertyEnum( );
    CPropertyEnum( String name, const CDef& def );

    const CDef&		GetDefinition() const;
    Sb::CString		GetValueAsString() const;
    Void			SetValue( const Int32& value );
    Bool			IsValidValue( const Int32& value );

private:

    Bool			SetValueFromString( String v );

    CDef			m_Def;
};

/**
Int32 property
*/
class CPropertyInt32 : public CPropertyConcrete<Int32>
{

    DEFINE_MANAGED_CLASS( CPropertyInt32 )

public:

    CPropertyInt32( );
    CPropertyInt32( String name );

    Sb::CString GetValueAsString() const;

private:

	Bool	SetValueFromString( String v );

};

/**
Int32 property
*/
class CPropertyBool : public CPropertyConcrete<Bool>
{

    DEFINE_MANAGED_CLASS( CPropertyBool )

public:

    CPropertyBool( );
    CPropertyBool( String name );
    Sb::CString GetValueAsString() const;

private:

	Bool	SetValueFromString( String v );
};
/**
Float32 property
*/
class CPropertyFloat32 : public CPropertyConcrete<Float32>
{

    DEFINE_MANAGED_CLASS( CPropertyFloat32 )

public:

    CPropertyFloat32( );
    CPropertyFloat32( String name );

    Sb::CString GetValueAsString() const;

private:

    Bool	SetValueFromString( String v );

};

/**
Sb::CMatrix4x4f property
*/
class CPropertyMatrix4x4 : public CPropertyConcrete<Sb::CMatrix4x4f>
{

    DEFINE_MANAGED_CLASS( CPropertyMatrix4x4 )

public:

    CPropertyMatrix4x4( );
    CPropertyMatrix4x4( String name );

    Sb::CString GetValueAsString() const;
	Bool	Serialize( Sb::CPersistentArchive& ar );

private:

    Bool	SetValueFromString( String v );

};

/**
Sb::CVector3f property
*/
class CPropertyVector3f : public CPropertyConcrete<Sb::CVector3f>
{

    DEFINE_MANAGED_CLASS( CPropertyVector3f )

public:

    CPropertyVector3f( );
    CPropertyVector3f( String name );

    Sb::CString GetValueAsString() const;
	Bool	Serialize( Sb::CPersistentArchive& ar );

private:

    Bool	SetValueFromString( String v );

};

/**
Sb::CVector3f property
*/
class CPropertyVector4f : public CPropertyConcrete<Sb::CVector4f>
{

    DEFINE_MANAGED_CLASS( CPropertyVector4f )

public:

    CPropertyVector4f( );
    CPropertyVector4f( String name );

    Sb::CString GetValueAsString() const;
	Bool	Serialize( Sb::CPersistentArchive& ar );

private:

    Bool	SetValueFromString( String v );

};
/**
Sb::CColor property
*/
class CPropertyColor : public CPropertyConcrete<Sb::CColor>
{

    DEFINE_MANAGED_CLASS( CPropertyColor )

public:

    CPropertyColor( );
    CPropertyColor( String name );

    Sb::CString GetValueAsString() const;

private:

    Bool	SetValueFromString( String v );

};

/**
Sb::CVector2f property
*/
class CPropertyVector2f : public CPropertyConcrete<Sb::CVector2f>
{

    DEFINE_MANAGED_CLASS( CPropertyVector2f )

public:

    CPropertyVector2f( );
    CPropertyVector2f( String name );

    Sb::CString	GetValueAsString() const;
	Bool	Serialize( Sb::CPersistentArchive& ar );

private:

	Bool	SetValueFromString( String v );

};


/**
Sb::CVector2i property
*/
class CPropertyVector2i : public CPropertyConcrete<Sb::CVector2i>
{

    DEFINE_MANAGED_CLASS( CPropertyVector2i )

public:

    CPropertyVector2i( );
    CPropertyVector2i( String name );

    Sb::CString	GetValueAsString() const;
	Bool		Serialize( Sb::CPersistentArchive& ar );

private:

	Bool	SetValueFromString( String v );

};


/**
Sb::CString property
*/
class CPropertyString : public CPropertyConcrete< Sb::CString >
{

    DEFINE_MANAGED_CLASS( CPropertyString )

public:

	enum EType
	{
		nType_SingleLine = 0,
		nType_MultiLine,
		nType_Script,
		nType_Shader,
		nType_Count,
		nType_None
	};

    CPropertyString( );
    CPropertyString( String name, EType type );

    Sb::CString GetValueAsString() const;
	EType		GetType() const;

private:

	Bool	SetValueFromString( String v );
	Bool	Serialize( Sb::CPersistentArchive& ar );

	EType	m_Type;

};

/**
CFileDesc property
*/
class CPropertyFilePath : public CPropertyConcrete<Sb::CFilePath>
{

    DEFINE_MANAGED_CLASS( CPropertyFilePath )

public:

    CPropertyFilePath( );
    CPropertyFilePath( String name );

    Sb::CString GetValueAsString() const;
    Bool	Serialize( Sb::CPersistentArchive& ar );
    Void    SetWildcard( String wildcard );
    String  GetWildcard() const;

private:

    Bool	SetValueFromString( String v );

    Sb::CStaticString<256>  m_Wildcard;
};

/**
CHandle property
*/
class CPropertyHandle : public CPropertyConcrete<CHandle>
{

    DEFINE_MANAGED_CLASS( CPropertyHandle )

public:

    typedef Sb::CList<const Sb::CTypeInfo*> FilterList;
    Bool operator != ( const CPropertyHandle& other );

    CPropertyHandle( );
    CPropertyHandle( String name );
    ~CPropertyHandle();

    Sb::CString			GetValueAsString() const;
    Bool                Serialize( Sb::CPersistentArchive& ar );
    Void                SetValue( const CHandle& value );
    Void                AddFilter( const Sb::CTypeInfo& i );
    Bool                TypeCheck( const CHandle& h ) const;
    const FilterList&   GetFilterList() const;
	Void                AddDependencyToOwner( Bool onOff );
	Bool				CopyFrom( const CPropertyInterface& p );
	Void				SetSerializeFilters( Bool onOff );

private:

    Bool                SetValueFromString( String v );
	Void				BindToObject( CBankObject* obj );

    FilterList			m_FilterList;
    Bool				m_AddDependency;
	Bool				m_SerializeFilter;
};

};

#endif
