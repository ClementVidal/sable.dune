#include <Dune/Model/Bank/Handle.h>

#include <Dune/Common/Log.h>

using namespace Dune;

CHandle::CHandle()
{
    SetNull();
}

CHandle::CHandle( const CUuid& r )
{
    SetIndex( -1 );
    SetSpin( -1 );
    m_Bank = r;
}

CHandle::CHandle( const CUuid& r,  Int32 index, Int32 spin )
{
	SetIndex( index );
    SetSpin( spin );
    m_Bank = r;
}

Bool CHandle::IsValid() const
{
    return m_Index != -1 && m_Spin != -1 && m_Bank.IsValid();
}

Bool CHandle::Serialize( Sb::CPersistentArchive& ar  )
{
	Int32 i = -1;

	if( !ar.Serialize( m_Index, WT("Index") ) )
        return FALSE;

    if( !ar.Serialize( m_Spin, WT("Spin") ) )
        return FALSE;

    CUuid::UuidString string = m_Bank.ConvertToString();
    if( !ar.Serialize( string, WT("Buid") ) )
        return FALSE;
    m_Bank = CUuid( string );

	return TRUE;
}

Void CHandle::SetNull()
{
    SetIndex( -1 );
    SetSpin( -1 );
    m_Bank = CUuid();
}

Int32 CHandle::GetIndex() const
{
	return m_Index;
}

Int32 CHandle::GetSpin() const
{
	return m_Spin;
}

Void CHandle::SetIndex( Int32 i )
{
	m_Index = i;
}

Void CHandle::SetSpin( Int32 i )
{
	m_Spin = i;
}


Bool CHandle::operator< ( const CHandle& h ) const 
{
	if( GetBankUuid() < h.GetBankUuid() )
	{
		return TRUE;
	}
	else if( GetBankUuid() == h.GetBankUuid() )
	{
		if( GetIndex() < h.GetIndex() )
			return TRUE;
	}

    return FALSE;
}

const CUuid& CHandle::GetBankUuid() const
{
	return m_Bank;
}

Bool CHandle::operator == ( const CHandle& h ) const 
{
    if( GetIndex() == h.GetIndex() && m_Bank == h.m_Bank )
        return TRUE;

    return FALSE;
}

Bool CHandle::operator!= ( const CHandle& h ) const 
{
    if( GetIndex() != h.GetIndex() || m_Bank != h.m_Bank )
        return TRUE;

    return FALSE;
}
