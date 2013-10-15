#ifndef _DUNE_PROJECT_SETTINGS_
#define _DUNE_PROJECT_SETTINGS_

#include <Sable\Core\File\FilePath.h>

namespace Sable
{
    class CPersistentArchive;
}

namespace Dune
{


/** 
\ingroup ModelProject
Project settings 
*/
class CProjectSettings 
{

public:

	/** @name DataTypes*/
	//@{
	typedef Sb::CList<Sb::CFilePath> PathList;
	//@}

    /** @name Constructor/Destructor*/
    //@{
    CProjectSettings( );
    virtual ~CProjectSettings();
    //@}

    /** @name Accessors*/
    //@{
	const Sb::CFilePath&	GetBakeOutputPath() const;
	Void					SetBakeOutputPath( const Sb::CFilePath& path );
	Void					SetName( String name );
    String					GetName() const;
    Void					SetDescription( String name );
    String					GetDescription() const;

	Void					AddAssetPath( const Sb::CFilePath& path );
	const PathList&			GetAssetPathList() const;
    //@}

    /** @name Manipulator*/
    //@{
    Bool Serialize( Sb::CPersistentArchive& ar );
    //@}

private:

    // Attributes
    Sb::CStaticString<128>				m_Name;
    Sb::CString							m_Description;
	Sb::CFilePath						m_BakeOutputPath;
	PathList							m_AssetPathList;
};

}

#endif
