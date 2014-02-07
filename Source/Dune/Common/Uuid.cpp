#include <Dune/Common/Uuid.h>

#include <Sable/Core/Persistent/Archive.h>

#include <Objbase.h>
using namespace Dune;

CUuid::CUuid( )
{
    m_Data = "00000000-0000-0000-0000-000000000000";
}

CUuid::CUuid( const CUuid& other )
{
    m_Data = other.m_Data;
}

CUuid::CUuid( String data )
{
    DebugAssert( IsValidString( data ) );
    m_Data = data;
}

Bool CUuid::IsValidString( String s )
{
    WChar tmp[128];
    Sb::StringToWString( s, tmp, 128 );

    UUID uuid;
    RPC_STATUS status = ::UuidFromString( tmp, &uuid );

    return status == RPC_S_OK; 
}

Bool CUuid::IsValid() const
{
    return m_Data.IsEmpty() == FALSE;
}
 
CUuid::~CUuid()
{
}

CUuid::UuidString CUuid::ConvertToString( ) const
{
    return m_Data;
}

Bool CUuid::Serialize( Sb::CPersistentArchive& ar )
{
    if( ! ar.Serialize( m_Data, WT("Uuid") ) )
        return FALSE;

    return TRUE;
}

CUuid& CUuid::operator = ( const CUuid& other )
{
    m_Data = other.m_Data;
    return *this;
}

Bool CUuid::operator < ( const CUuid& other ) const
{
    return Sb::StringCompare( m_Data.GetBuffer(), other.m_Data.GetBuffer() ) < 0;
}

Bool CUuid::operator == ( const CUuid& other ) const
{
    return Sb::StringCompare( m_Data.GetBuffer(), other.m_Data.GetBuffer() ) == 0;
}

Bool CUuid::operator != ( const CUuid& other ) const
{
    return Sb::StringCompare( m_Data.GetBuffer(), other.m_Data.GetBuffer() ) != 0;
}

CUuid CUuid::Create()
{
    GUID win32Guid;
    CUuid uuid;
    RPC_STATUS status = ::CoCreateGuid( &win32Guid );
    DebugAssert( status == RPC_S_OK );

    Char tmp[64];
    RPC_WSTR str;
    status = ::UuidToString( (GUID*) &win32Guid, &str );
    DebugAssert( status == RPC_S_OK );
    Sb::WStringToString( str, tmp, 64 );
    status = RpcStringFree( &str );
    DebugAssert( status == RPC_S_OK );

    uuid.m_Data = tmp;
 
    return uuid;
}
