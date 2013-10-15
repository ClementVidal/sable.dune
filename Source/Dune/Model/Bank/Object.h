#ifndef _DUNE_MODEL_BANK_OBJECT_
#define _DUNE_MODEL_BANK_OBJECT_

#include <Sable\Core\Common\DataTypes.h>
#include <Sable\Core\ManagedObject\ManagedObject.h>
#include <Sable\Core\Event\Header.h>
#include <Sable\Core\Collection\List.h>
#include <Sable\Core\Collection\Map.h>

#include <Dune\Model\Bank\Handle.h>
#include <Dune\Model\Bank\Bank.h>

namespace Sable
{
    class CPersistentArchive;
}

namespace Dune
{

class CPropertyInterface;
class CBakeContext;
class CBakeObjectSet;
class CPropertyArray;
class CPropertyHandle;
class CPropertyString;

/**
\ingroup ModelRepository

Base class for any kind of object that can be put and referenced using a database
*/
class CBankObject : public Sb::CManagedObject
{

    DEFINE_MANAGED_CLASS( CBankObject );

public:

    /** @name DataTypes */
    //@{
    typedef Sb::CMap<Sb::CString, Sb::CRef<CPropertyInterface>> PropertyTable;
    struct SDependency
    {
        SDependency();
        Bool operator == ( const SDependency& d );
        CHandle Handle;
        UInt32  Count;
    };
    typedef Sb::CList<SDependency>  DependencyList;
	//@}

    /** @name Constructor/Destructor*/
    //@{
    CBankObject( );
    ~CBankObject();
    //@}

    /** @name Accessors*/
    //@{
    UInt32                  GetPropertyCount() const;
    const PropertyTable&    GetPropertyTable() const;
    Bool                    AddProperty( CPropertyInterface& p );
    Void                    RemoveProperty( CPropertyInterface& p );
    CPropertyInterface*     GetProperty( String name ) const;

    const CHandle&          GetParent( ) const;
    Void                    SetParent( const CHandle& parent );
    const CHandle&		    GetChildAtIndex( UInt32 index ) const;
	UInt32				    GetChildCount() const;
	Int32				    GetChildIndex( const CHandle& ) const;
	Void					SetChildIndex( Int32 oldChildIndex, Int32 newChildIndex );

    const CHandle&          GetHandle( ) const;
	CBank*                  GetBank() const;
	CProject*				GetProject() const;
	CBankObjectAccessor*    GetObjAccessor() const;

    const DependencyList&   GetOutputDependencyList() const;
    const DependencyList&   GetInputDependencyList() const;

    Void                    SetName( String n ) const;
    String                  GetName() const;
    //@}

    /** @name Manipulator*/
    //@{
	CBankObject*		Clone();

    Void				AddChild( const CHandle& elem );
    Void				RemoveChild( const CHandle& elem );

    virtual Bool        Serialize( Sb::CPersistentArchive& ar );
    virtual Void        OnPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue );

    virtual Void		PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& out  ) const;
    virtual Void		BakeObjectSet(CBakeContext& context, CBakeObjectSet& out );
	virtual Void		InitializeObjectSet(CBakeContext& context, CBakeObjectSet& out );

	Void				FindChildByType( const Sb::CTypeInfo& type, Sb::CList<CHandle>& result ) const;
	//@}

    /** @name System*/
    //@{
    Void                AddDependency( const CHandle& o );
	Void                RemoveDependency( const CHandle& o );
    //@}

    /** @name Signal*/
    //@{
	Sb::CEventSignal< const CHandle&, Int32, Int32 >					SignalChildIndexChanged;
	Sb::CEventSignal< const CHandle& >									SignalChildAdded;
	Sb::CEventSignal< const CPropertyInterface&, const Sb::CString& >   SignalPropertyChanged;
    Sb::CEventSignal< const CPropertyInterface& >						SignalPropertyAdded;
	Sb::CEventSignal< const CPropertyInterface& >						SignalPropertyRemoved;
    //@}

    /** @name Attributes names*/
    //@{
    static String PropName;
    static String PropChilds;
    static String PropParent;
    //@}

private:

    // Friendship
    friend CBank;

    // Methods
	Void        SetHandle( const CHandle& handle ); // Only used by CBank 
	Void        SetBank( CBank& repo );
	Void		DetachFromParent( );
	Void		FindChildByType( const CHandle& root, const Sb::CTypeInfo& type, Sb::CList<CHandle>& result ) const;

    // DataType

    // Attributes
    CBank*                  m_Bank;
    CHandle                 m_Handle;
    PropertyTable           m_PropertyTable;
    DependencyList          m_InputDependency;
    DependencyList          m_OutputDependency;

    CPropertyArray*			m_Childs;
    CPropertyHandle*		m_Parent;
    CPropertyString*		m_Name;

	Sb::CEventSlot< CBankObject, const CPropertyInterface&, const Sb::CString& >	m_SlotOnPropertyChanged;


};

};

#endif