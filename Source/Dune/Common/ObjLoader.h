#ifndef _DUNE_COMMON_OBJLOADER_
#define _DUNE_COMMON_OBJLOADER_

#include <Sable\Core\Common\DataTypes.h>
#include <Sable\Core\String\StaticString.h>
#include <Sable\Core\Common\IODevice.h>

namespace Dune
{

/** 
*/
class CObjLoader
{

public:

    /** @name DataTypes*/
    //@{
	struct SFace
	{
		UInt32 Id[3];
	};
	struct SVertex
	{
		Sb::CVector3f Pos;
	};
	typedef Sb::CArray<SVertex> SVertexTable;
	typedef Sb::CArray<SFace>	SFaceTable;
    //@}

	/** @name Constructor/Destructor*/
    //@{
	CObjLoader( );
	~CObjLoader();
    //@}

	/** @name Accessors*/
    //@{
	const SVertexTable& GetVertexTable() const;
	const SFaceTable& GetFaceTable() const;
    //@}

	/** @name Manipulator*/
	//@{
	Bool Load( const Sb::CFilePath& file );
	Bool Load( const Sb::CString& data );
	//@}

private:

    //Types

    //Methods
	Bool Load( Sb::CIODevice& device );
	Bool ParsePosition( String line, UInt32 index );
	Bool ParseFace( String line, UInt32 index );

    //Attributes
	SVertexTable	m_VertexTable;
	SFaceTable	m_FaceTable;

};


}

#endif