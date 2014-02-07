#ifndef _DUNE_COMMON_LEXICALCAST_
#define _DUNE_COMMON_LEXICALCAST_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/String/String.h>
#include <Sable/Core/String/StaticString.h>
#include <Sable/Core/Collection/Array.h>

namespace Sable
{
	class CVector2i;
    class CVector2f;
    class CVector3f;
    class CVector4f;
    class CColor;
    class CFilePath;
    class CEulerAngle;
    class CMatrix4x4f;
}

namespace Dune
{

class CHandle;

/**
Convert string to value or value to string
*/
class CLexicalCast
{

public:

    /** @name Manipulator*/
    //@{
    static Sb::CString	ValueToString( const CHandle& v );
	static Sb::CString	ValueToString( const Int32& v );
    static Sb::CString	ValueToString( const UInt32& v );
	static Sb::CString	ValueToString( const Float32& v );
	static Sb::CString	ValueToString( const Sb::CVector2i& v );
    static Sb::CString	ValueToString( const Sb::CVector2f& v );
    static Sb::CString	ValueToString( const Sb::CVector3f& v );
    static Sb::CString	ValueToString( const Sb::CVector4f& v );
    static Sb::CString	ValueToString( const Sb::CMatrix4x4f& v );
    static Sb::CString	ValueToString( const Sb::CEulerAngle& v );
    static Sb::CString	ValueToString( const Sb::CColor& v );
	static Sb::CString	ValueToString( const Sb::CFilePath& );
	static Sb::CString	ValueToString( const Sb::CString& v );
	static Sb::CString	ValueToString( const Sb::CStaticString<256>& v );

	static Bool	StringToValue( const Sb::CString& str, CHandle& v );
	static Bool	StringToValue( const Sb::CString& str, Int32& v );
	static Bool	StringToValue( const Sb::CString& str, UInt32& v );
	static Bool	StringToValue( const Sb::CString& str, Float32& v );
	static Bool	StringToValue( const Sb::CString& str, Sb::CVector2i& v );
	static Bool	StringToValue( const Sb::CString& str, Sb::CVector2f& v );
	static Bool	StringToValue( const Sb::CString& str, Sb::CVector3f& v );
	static Bool	StringToValue( const Sb::CString& str, Sb::CVector4f& v );
	static Bool	StringToValue( const Sb::CString& str, Sb::CMatrix4x4f& v );
	static Bool	StringToValue( const Sb::CString& str, Sb::CEulerAngle& v );
	static Bool	StringToValue( const Sb::CString& str, Sb::CColor& v );
	static Bool	StringToValue( const Sb::CString& str, Sb::CFilePath& v );
	static Bool	StringToValue( const Sb::CString& str, Sb::CString& v );
	static Bool	StringToValue( const Sb::CString& str, Sb::CStaticString<256>& v );
    //@}

private:

    // Attributes

};


}


#endif
