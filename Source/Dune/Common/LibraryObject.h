#ifndef _DUNE_COMMON_LIBRARYOBJECT_
#define _DUNE_COMMON_LIBRARYOBJECT_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/Collection/Array.h>
#include <Sable/Core/ManagedObject/ManagedObject.h>

namespace Dune
{

/** 
*/
class CLibraryObject : public Sb::CManagedObject
{

public:

    /** @name DataTypes*/
    //@{
    //@}

	/** @name Constructor/Destructor*/
    //@{
	CLibraryObject( );
	CLibraryObject( CBankObject& object, const Sb::CString& description );
	~CLibraryObject();
    //@}

    /** @name Operators*/
    //@{
    //@}

	/** @name Accessors*/
	//@{
	const Sb::CFilePath&			GetFilePath() const;
	String							GetName() const;
	const Sb::CArray<Byte>&			GetThumbRawData() const;
	const Sb::CString&				GetDescription() const;
	Void							SetDescription( String desc );
	const Sb::CArray<Sb::CString>&	GetTags() const;
	CBankObject*					GetObject() const;
	CBank&							GetBank();
    //@}

	/** @name Manipulator*/
	//@{
	Bool	Load( const Sb::CFilePath& path );
	Bool	Save( const Sb::CFilePath& path );
	Bool	Save( );
	//@}

private:

    //Types

    //Methods
	Bool	Serialize( Sb::CPersistentArchive& ar );
	Void	LoadUnknownLibraryObjectIcon();

    //Attributes
	CBankObject*				m_Object;
	Sb::CString					m_Description;
	Sb::CArray<Sb::CString>		m_Tags;
	Sb::CArray<Byte>			m_ThumbRawData;
	Sb::CFilePath				m_FilePath;
	CBank						m_Bank;
};


}

#endif
