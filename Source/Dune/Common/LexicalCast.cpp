#include <Dune/Common/LexicalCast.h>

#include <Dune/Model/Bank/Bank.h>
#include <Dune/Model/Project/Project.h>

#include <Sable/Core/Math/Vector2i.h>
#include <Sable/Core/Math/Vector2f.h>
#include <Sable/Core/Math/Vector3f.h>
#include <Sable/Core/Math/Vector4f.h>
#include <Sable/Core/Math/Matrix4x4f.h>
#include <Sable/Core/Common/Color.h>
#include <Sable/Core/File/FilePath.h>

using namespace Dune;

Sb::CString CLexicalCast::ValueToString( const CHandle& v )
{
	Sb::CString s;
	s.WriteFormattedText( "Handle(%d,%d,%s)", v.GetIndex(), v.GetSpin(), v.GetBankUuid().ConvertToString().GetBuffer() );
	return s;
}

Sb::CString CLexicalCast::ValueToString( const Int32& v )
{
	Sb::CString str;
	str.WriteFormattedText( "%d", v );
	return str;
}

Sb::CString CLexicalCast::ValueToString( const UInt32& v )
{
	Sb::CString str;
	str.WriteFormattedText( "%u", v );
	return str;
}

Sb::CString CLexicalCast::ValueToString( const Float32& v )
{
	Sb::CString str;
	str.WriteFormattedText( "%.7f", v );
	return str;
}

Sb::CString CLexicalCast::ValueToString( const Sb::CVector2f& v )
{
	Sb::CString str;
	str.WriteFormattedText( "(%.7f;%.7f)", v.GetX(), v.GetY() );
	return str;
}

Sb::CString CLexicalCast::ValueToString( const Sb::CVector2i& v )
{
	Sb::CString str;
	str.WriteFormattedText( "(%d;%d)", v.GetX(), v.GetY() );
	return str;
}

Sb::CString CLexicalCast::ValueToString( const Sb::CVector3f& v )
{
	Sb::CString str;
	str.WriteFormattedText( "(%.7f;%.7f;%.7f)", v.GetX(), v.GetY(), v.GetZ() );
	return str;
}

Sb::CString CLexicalCast::ValueToString( const Sb::CEulerAngle& v )
{
    Sb::CString str;
    str.WriteFormattedText( "(%.7f;%.7f;%.7f)", v.GetPitch(), v.GetYaw(), v.GetRoll() );
    return str;
}

Sb::CString CLexicalCast::ValueToString( const Sb::CVector4f& v )
{
	Sb::CString str;
	str.WriteFormattedText( "(%.7f;%.7f;%.7f;%.7f)", v.GetX(), v.GetY(), v.GetZ(), v.GetW() );
	return str;
}

Sb::CString CLexicalCast::ValueToString( const Sb::CColor& v )
{
	Sb::CString str;
	str.WriteFormattedText( "(%.7f;%.7f;%.7f;%.7f)", v.GetA(), v.GetR(), v.GetG(), v.GetB() );
	return str;
}

Sb::CString CLexicalCast::ValueToString( const Sb::CFilePath& v )
{
	Sb::CString str;
	str.WriteFormattedText( "%s", v.GetBuffer() );
	return str;
}

Sb::CString CLexicalCast::ValueToString( const Sb::CString& v )
{
	Sb::CString str;
	str.WriteFormattedText( "%s", v.GetBuffer() );
	return str;
}

Sb::CString CLexicalCast::ValueToString( const Sb::CStaticString<256>& v )
{
    Sb::CString str;
    str.WriteFormattedText( "%s", v.GetBuffer() );
    return str;
}

Sb::CString	CLexicalCast::ValueToString( const Sb::CMatrix4x4f& v )
{
    Sb::CString str;
    str.WriteFormattedText( "((%.7f;%.7f;%.7f;%.7f)(%.7f;%.7f;%.7f;%.7f)(%.7f;%.7f;%.7f;%.7f)(%.7f;%.7f;%.7f;%.7f))", 
        v._11, v._12, v._13, v._14,
        v._21, v._22, v._23, v._24,
        v._31, v._32, v._33, v._34,
        v._41, v._42, v._43, v._44 );

    return str;
}

Bool CLexicalCast::StringToValue( const Sb::CString& str, CHandle& v )
{
	Int64 x,y;
    Char bankName[256];
	if( sscanf( str.GetBuffer(), "Handle(%d,%d,%36s)", &x, &y, bankName ) == 3 )
	{
		v = CHandle( CUuid( bankName ), x, y );
		return TRUE;
	}
	return FALSE;
}

Bool CLexicalCast::StringToValue( const Sb::CString& str, Int32& v )
{
	v = Sb::StringToInt32( str.GetBuffer( ) );
	return TRUE;
}

Bool CLexicalCast::StringToValue( const Sb::CString& str, UInt32& v )
{
	v = Sb::StringToInt32( str.GetBuffer( ) );
	return TRUE;
}

Bool CLexicalCast::StringToValue( const Sb::CString& str, Float32& v )
{
    v = Sb::StringToFloat32( str.GetBuffer( ) );
	return TRUE;
}

Bool CLexicalCast::StringToValue( const Sb::CString& str, Sb::CVector2f& v )
{
	Float32 x,y;

	if( sscanf( str.GetBuffer(), "(%f;%f)", &x, &y ) == 2 )
	{
		v = Sb::CVector2f( x, y );
		return TRUE;
	}
	return FALSE;
}

Bool CLexicalCast::StringToValue( const Sb::CString& str, Sb::CVector2i& v )
{
	Int32 x,y;

	if( sscanf( str.GetBuffer(), "(%d;%d)", &x, &y ) == 2 )
	{
		v = Sb::CVector2i( x, y );
		return TRUE;
	}
	return FALSE;
}

Bool CLexicalCast::StringToValue( const Sb::CString& str, Sb::CVector3f& v )
{
	Float32 x,y,z;

	if( sscanf( str.GetBuffer(), "(%f;%f;%f)", &x, &y, &z ) == 3 )
	{
		v = Sb::CVector3f( x, y, z );
		return TRUE;
	}
	return FALSE;
}

Bool CLexicalCast::StringToValue( const Sb::CString& str, Sb::CEulerAngle& v )
{
    Float32 x,y,z;

    if( sscanf( str.GetBuffer(), "(%f;%f;%f)", &x, &y, &z ) == 3 )
    {
        v = Sb::CEulerAngle( x, y, z );
        return TRUE;
    }
    return FALSE;
}

Bool CLexicalCast::StringToValue( const Sb::CString& str, Sb::CVector4f& v )
{
	Float32 x,y,z,w;

	if( sscanf( str.GetBuffer(), "(%f;%f;%f;%f)", &x, &y, &z, &w ) == 4 )
	{
		v = Sb::CVector4f( x, y, z, w );
		return TRUE;
	}
	return FALSE;
}


Bool CLexicalCast::StringToValue( const Sb::CString& str, Sb::CMatrix4x4f& v )
{
    Float32 _11,_12,_13,_14, _21,_22,_23,_24 , _31,_32,_33,_34, _41,_42,_43,_44;

    if( sscanf( str.GetBuffer(), "((%f;%f;%f;%f)(%f;%f;%f;%f)(%f;%f;%f;%f)(%f;%f;%f;%f))", 
        &_11, &_12, &_13, &_14, &_21, &_22, &_23, &_24, &_31, &_32, &_33, &_34, &_41, &_42, &_43, &_44 ) == 16 )
    {
        v = Sb::CMatrix4x4f( _11, _12, _13, _14, _21, _22, _23, _24, _31, _32, _33, _34, _41, _42, _43, _44 );
        return TRUE;
    }
    return FALSE;
}

Bool CLexicalCast::StringToValue( const Sb::CString& str, Sb::CColor& v )
{
	Float32 x,y,z,w;

	if( sscanf( str.GetBuffer(), "(%f;%f;%f;%f)", &x, &y, &z, &w ) == 4 )
	{
		v = Sb::CColor( x, y, z, w );
		return TRUE;
	}
	return FALSE;
}

Bool CLexicalCast::StringToValue( const Sb::CString& str, Sb::CFilePath& v )
{
	v = Sb::CFilePath( str.GetBuffer() );
	return TRUE;
}

Bool CLexicalCast::StringToValue( const Sb::CString& str, Sb::CString& v )
{
	v = str.GetBuffer();
	return TRUE;
}

Bool CLexicalCast::StringToValue( const Sb::CString& str, Sb::CStaticString<256>& v )
{
    v = str.GetBuffer();
    return TRUE;
}
