#include <Dune\Model\Bank\Object.h>


template<typename DataType>
CPropertyConcrete<DataType>::CPropertyConcrete(  ) :
    CPropertyInterface( )
{
    m_CanSetValue = TRUE;
}


template<typename DataType>
CPropertyConcrete<DataType>::CPropertyConcrete( String name, const DataType& value  ) :
    CPropertyInterface( name )
{
    m_Value = value;
    m_CanSetValue = TRUE;
}


template<typename DataType>
CPropertyConcrete<DataType>::~CPropertyConcrete()
{
    
}

template<typename DataType>
const DataType& CPropertyConcrete<DataType>::GetValue() const
{
    return m_Value;
}

template<typename DataType>
Void CPropertyConcrete<DataType>::SetValue( const DataType& value )
{
    if( m_Value != value && m_CanSetValue)
    {
        m_CanSetValue = FALSE;
        DataType oldValue = m_Value;
        m_Value = value;
        NotifyChange( oldValue );
        m_CanSetValue = TRUE;
    }
}

template<typename DataType>
Bool CPropertyConcrete<DataType>::Serialize( Sb::CPersistentArchive& ar )
{
    if( !ar.Serialize( m_Value, WT("Value") ) )
        return FALSE;

    return CPropertyInterface::Serialize( ar );
}

/**
This method is called automaticly by any function that direcly alter it's content.
This method will then:
First: Call OnPropertyChanged() on it's owner object
Second: Fire the SignalPropertyChanged signal
*/
template<typename DataType>
Void CPropertyConcrete<DataType>::NotifyChange( const DataType& oldValue )
{
    Sb::CString oldValueStr = CLexicalCast::ValueToString( oldValue );

    // Notify that this property has change
	SignalPropertyChanged( *this, oldValueStr );
}

template<typename DataType>
Bool CPropertyConcrete<DataType>::CopyFrom( const CPropertyInterface& p )
{
	DebugAssert( p.GetTypeInfo().IsKindOf( GetTypeInfo() ) );
	SetValue( ((const CPropertyConcrete<DataType>&)p).GetValue() );
	return TRUE;
}