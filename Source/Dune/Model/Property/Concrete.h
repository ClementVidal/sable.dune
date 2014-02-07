#ifndef _DUNE_PROPERTY_CONRETE_
#define _DUNE_PROPERTY_CONRETE_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/String/String.h>

#include <Dune/Model/Property/Interface.h>

#include <Dune/Common/LexicalCast.h>


namespace Dune
{

/**
*/
template <typename DataType>
class CPropertyConcrete :  public CPropertyInterface
{

public:

    /** @name DataTypes*/
    //@{
    typedef DataType Type;
    //@}

    /** @name Constructor/Destructor*/
    //@{
    CPropertyConcrete( );
    CPropertyConcrete( String name, const DataType& value  );     
    ~CPropertyConcrete();
    //@}

    /** @name Accessors*/
    //@{
    virtual const DataType&     GetValue() const;
	virtual Void				SetValue( const DataType& value );
    //@}

    /** @name Manipulator*/
	//@{
	Bool					CopyFrom( const CPropertyInterface& p ) ;
    //@}

protected:

    // Types
 
    // Methods
    Void				NotifyChange( const DataType& oldValue );
    Bool				Serialize( Sb::CPersistentArchive& ar );


    // Attributes
    DataType    m_Value;
    Bool        m_CanSetValue;
    
};


#include <Dune/Model/Property/Concrete.hpp>

};

#endif
