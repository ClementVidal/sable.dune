#ifndef _DUNE_PROPERTY_INTERFACE_
#define _DUNE_PROPERTY_INTERFACE_

#include <Sable\Core\Common\DataTypes.h>
#include <Sable\Core\String\String.h>
#include <Sable\Core\Collection\List.h>
#include <Sable\Core\Event\Signal.h>

#define CastProperty( prop, type ) ((type*)&prop)


namespace Dune
{

class CBankObject;
class CViewPropertyGrid;

/**
*/
class CPropertyInterface : public Sb::CManagedObject
{

	DEFINE_MANAGED_CLASS( CPropertyInterface );

public:

    /** @name DataTypes*/
    //@{
    //@}

    /** @name Constructor/Destructor*/
    //@{
    CPropertyInterface( );
    CPropertyInterface( String name );
    ~CPropertyInterface();
    //@}

    /** @name Accessors*/
    //@{
    Bool                            GetIsVisible() const;
    Bool                            GetIsEnabled() const;
	Bool							GetIsAutoSerialized( ) const;
    String                          GetGroup() const;

	Void							SetIsAutoSerialized( Bool onOff );
    Void                            SetIsVisible( Bool onOff );
    Void                            SetIsEnabled( Bool onOff );
    Void                            SetGroup( String group );

    String							GetName() const;
    CBankObject*                    GetObj() const;
	CBankObjectAccessor*			GetObjAccessor() const;

    virtual Sb::CString				GetValueAsString() const = 0;
	virtual Bool                    SetValueFromString( String v ) = 0;
	virtual Bool					CopyFrom( const CPropertyInterface& p ) = 0;
    //@}

    /** @name Manipulator*/
    //@{
	virtual Void Initialize( String name );
    virtual Bool Serialize(  Sb::CPersistentArchive& ar );
    //@}

	/** @name Signal*/
	//@{
	Sb::CEventSignal< const CPropertyInterface&, const Sb::CString& >		SignalPropertyChanged;
	//@}

protected:

	// Method
	virtual Void BindToObject( CBankObject* obj );

private:

    // Friendship
    friend class CBankObject;
    friend class CPropertyArray;

    // Attributes
    CBankObject*		        m_Object;
    Sb::CStaticString<64>       m_Name;

    Bool                        m_IsEnabled;
    Bool                        m_IsVisible;
	Bool						m_IsAutoSerialized;
    Sb::CStaticString<64>       m_GroupName;

    // Static attributes

    
};


};

#endif