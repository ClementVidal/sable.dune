#ifndef _DUNE_MODEL_ASSET_FBX_MANAGER_
#define _DUNE_MODEL_ASSET_FBX_MANAGER_

#include <Sable\Core\Common\DataTypes.h>
#include <Sable\Core\ManagedObject\ManagedObject.h>

#include <fbxsdk.h>

namespace Dune
{

class CFBXDoc;

/**
\ingroup ModelAsset

Global manager for collada document
*/
class CFBXManager
{

public:

    /** @name Constructor/Destructor*/
    //@{
    CFBXManager( );
    virtual ~CFBXManager();
    //@}

	/** @name Accessors*/
	//@{
	FbxManager*		GetFBXSdkManager() const;
	FbxIOSettings*	GetFBXIOSettings() const;
	//@}

    /** @name Manipulator*/
    //@{
	CFBXDoc*	GetOrAdd( const Sb::CFilePath& path );
	CFBXDoc*	Add( const Sb::CFilePath& path );
	CFBXDoc*	Get( const Sb::CFilePath& path );
	Void		Remove( const Sb::CFilePath& path );
	//@}

private:

    // Types

    // Methods 

    // Attributes
	FbxManager*										m_SdkManager;
	FbxIOSettings*									m_IOSettings;
	Sb::CMap< Sb::CFilePath, Sb::CRef<CFBXDoc> >	m_Table;

};


};

#endif