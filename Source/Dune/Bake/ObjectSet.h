#ifndef _DUNE_BAKE_OBJECTSET_
#define _DUNE_BAKE_OBJECTSET_

#include <Sable\Core\Common\DataTypes.h>
#include <Sable\Core\Collection\Array.h>
#include <Sable\Core\String\StaticString.h>



namespace Dune
{

/**
\ingroup Bake
Represent a set of named sable object ready for beeing baked
*/
class CBakeObjectSet
{

public:

    /** @name Constructor/Destructor*/
    //@{
    CBakeObjectSet( );
    CBakeObjectSet( String name );
    CBakeObjectSet( const CBakeObjectSet& other );
	~CBakeObjectSet();
	//@}

    /** @name Operators*/
    //@{
    CBakeObjectSet&    operator=( const CBakeObjectSet& other );
    //@}

    /** @name Accessors*/
    //@{
	String			GetName() const;
	Bool			NeedInit() const;
	Void 			NeedInit( Bool onOff );
    UInt32          GetObjCount() const;
    template< typename DataType >
    DataType*       GetObj( UInt32 i ) const;
    template< typename DataType >
    DataType*       GetObj( ) const;
    //@}

    /** @name Manipulators*/
    //@{
	Void	Clear();
	Void    AddObject( Sb::CManagedObject& obj );
    //@}

private:

    // DataTypes

    // Attributes
    Sb::CArray< Sb::CRef<Sb::CManagedObject> >      m_Table;
    UInt32              m_Count;
    Sb::CStaticString<64>   m_Name;
	Bool				m_NeedInit;
};

template< typename DataType >
DataType* CBakeObjectSet::GetObj( UInt32 i ) const
{
    if( m_Table[i] && m_Table[i]->GetTypeInfo().IsKindOf( DataType::GetStaticTypeInfo() ) )
        return (DataType*) &*m_Table[i];
    
    return NULL;
}

template< typename DataType >
DataType* CBakeObjectSet::GetObj( ) const
{
    UInt32 i;

    for( i=0;i<m_Count;i++ )
    {
        if( m_Table[i]->GetTypeInfo().IsKindOf( DataType::GetStaticTypeInfo() ) )
            return (DataType*) &*m_Table[i];
    }

    return NULL;
}

};

#endif