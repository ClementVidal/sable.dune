#ifndef _DUNE_PROPERTY_PROPERTYARRAY_
#define _DUNE_PROPERTY_PROPERTYARRAY_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/Collection/Array.h>
#include <Sable/Core/ManagedObject/TypeInfo.h>

#include <Dune/Model/Property/Concrete.h>
#include <Dune/Model/Property/Macro.h>

namespace Dune
{

class CBankObject;
class CPropertyHandle;


/**
CHandle array property
*/
class CPropertyArray : public CPropertyInterface
{

    DEFINE_MANAGED_CLASS( CPropertyArray );

public:

    // DataTypes
    typedef Sb::CArray< Sb::CRef< CPropertyInterface > > ArrayType;

    // Constructor / Destructor
    CPropertyArray( );
    CPropertyArray( String name, const Sb::CTypeInfo& type, UInt32 nb = 0 );
    ~CPropertyArray();

    // Accessors
	const Sb::CTypeInfo&		GetItemTypeInfo()const;
    Sb::CString					GetValueAsString() const;
	Bool						SetValueFromString( String v );
	Void                        SetItemCount( UInt32 nb );
	UInt32                      GetItemCount() const;
	Void						SetItemAtIndex( UInt32 i, CPropertyInterface& a );
	Void                        AddItemAtEnd( CPropertyInterface& a );
	const CPropertyInterface&   GetItemAtIndex( UInt32 i ) const ;
	CPropertyInterface&			GetItemAtIndex( UInt32 i ) ;

    // Manipulator
    Bool                        Serialize( Sb::CPersistentArchive& ar );
    CPropertyInterface*         AddItemAtEnd( );
	Void                        AddItemAtIndex( CPropertyInterface& a, UInt32 index );
    Void                        RemoveItemAtIndex( UInt32 i );
    Void                        RemoveItem( const CPropertyInterface& a );
    Void                        ClearAllItems( );
	Void						MoveItem( Int32 oldIndex, Int32 newIndex );
	Bool						CopyFrom( const CPropertyInterface& p );

	/** @name Signal*/
	//@{
	Sb::CEventSignal< const CPropertyInterface& >		SignalPropertyAdded;
	Sb::CEventSignal< const CPropertyInterface& >		SignalPropertyRemoved;
	Sb::CEventSignal< Int32, Int32 >					SignalItemIndexChanged;
	//@}

private:

    // Method
    Void            OnChildPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue );
	Void			BindToObject( CBankObject* obj );

    // Attributes
    ArrayType                   m_Array;
    const Sb::CTypeInfo*        m_Type;
    Sb::CEventSlot< CPropertyArray, const CPropertyInterface&, const Sb::CString& >	m_SlotPropertyChanged;

};


}

#endif