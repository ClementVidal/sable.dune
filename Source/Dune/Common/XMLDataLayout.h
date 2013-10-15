#ifndef _DUNE_COMMON_XMLDATALAYOUT_
#define _DUNE_COMMON_XMLDATALAYOUT_

#include <Sable\Core\Common\DataTypes.h>
#include <Sable\Core\Persistent\DataLayout.h>
#include <Sable\Core\Collection\Stack.h>

#include <QtXml\QDomElement>
#include <QtXml\QDomDocument>

namespace Sable
{

class CPersistentArchive;

/** 
*/
class CXMLDataLayout : public CPersistentDataLayout
{


public:

	/** @name Constructor/Destructor*/
	//@{
	CXMLDataLayout( const Sb::CFilePath& path );
	~CXMLDataLayout();
	//@}

	/** @name Operator*/
	//@{
	//@}

	/** @name Accessor*/
	//@{
	//@}

	/** @name Manipulator*/
	//@{
    Bool Open( Sb::CPersistentArchive& ar );
	Void FlushData( const Bool IsStoring );

	Int16 BeginScope( WString name, Int16 version );
	Bool EndScope( );

	Void Store( UInt64 data, WString type, WString name );
	Void Store( UInt32 data, WString type, WString name );
	Void Store( UInt16 data, WString type, WString name );
	Void Store( UInt8 data, WString type, WString name );
	Void Store( Int64 data, WString type, WString name );
	Void Store( Int32 data, WString type, WString name );
	Void Store( Int16 data, WString type, WString name );
	Void Store( Int8 data, WString type, WString name );
	Void Store( Float32 data, WString type, WString name );
	Void Store( String data, UInt32 s, WString type, WString name );
	Void Store( WString data, UInt32 s, WString type, WString name );
	Void Store( Byte* data, UInt32 s, WString type, WString name );
	Void Store( const CString& data, WString type, WString name );

	Bool Load( UInt64& data, WString type, WString name );
	Bool Load( UInt32& data, WString type, WString name );
	Bool Load( UInt16& data, WString type, WString name );
	Bool Load( UInt8& data, WString type, WString name );
	Bool Load( Int64& data, WString type, WString name );
	Bool Load( Int32& data, WString type, WString name );
	Bool Load( Int16& data, WString type, WString name );
	Bool Load( Int8& data, WString type, WString name );
	Bool Load( Float32& data, WString type, WString name );
	Bool Load( Char* data, UInt32 s, WString type, WString name );
	Bool Load( WChar* data, UInt32 s, WString type, WString name );
	Bool Load( Byte* data, UInt32 s, WString type, WString name );
	Bool Load( CString& data, WString type, WString name );
	//@}

private:

    // DataTypes
    struct SEntry
    {
        SEntry() { LastChild = 0; }
		SEntry( QDomElement n, Int32 lastChild ) { Node = n; LastChild = lastChild; }//n.childNodes().count()-1; }
        QDomElement  Node;
        Int32      LastChild;
    };

    // Methods

    // Attributes
    Sb::CFilePath                       m_FilePath;
    QDomDocument                    m_Document;
    QDomElement				        m_RootNode;
    CStack<SEntry>                  m_CurrentNode;
    Sb::CPersistentArchive*      m_Archive;

};

}

#endif