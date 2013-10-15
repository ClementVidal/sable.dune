#ifndef _DUNE_COMMON_UUID_
#define _DUNE_COMMON_UUID_

#include <Sable\Core\Common\DataTypes.h>
#include <Sable\Core\String\StaticString.h>

#define _UUID_BYTE_SIZE_ 16
#define _UUID_STRING_SIZE_ 37

namespace Dune
{

/** 
*/
class CUuid
{

public:

    /** @name DataTypes*/
    //@{
    typedef Sb::CStaticString<_UUID_STRING_SIZE_> UuidString;
    //@}

	/** @name Constructor/Destructor*/
    //@{
	CUuid( );
    CUuid( String data );
	CUuid( const CUuid& other );
	~CUuid();
    //@}

    /** @name Operators*/
    //@{
    CUuid& operator = ( const CUuid& other );
    Bool operator == ( const CUuid& other ) const;
    Bool operator != ( const CUuid& other ) const;
    Bool operator < ( const CUuid& other ) const;
    //@}

	/** @name Accessors*/
    //@{
    UuidString      ConvertToString( ) const;
    Bool            IsValid() const;
    //@}

	/** @name Manipulator*/
	//@{
    Bool Serialize( Sb::CPersistentArchive& ar );
	//@}

    /** @name Static functions*/
    //@{
    static CUuid Create();
    static Bool IsValidString( String s );
    //@}

private:

    //Types

    //Methods

    //Attributes
    UuidString    m_Data;

};


}

#endif