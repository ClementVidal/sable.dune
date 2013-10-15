#ifndef _DUNE_MODEL_ASSET_FBX_DAE_
#define _DUNE_MODEL_ASSET_FBX_DAE_

#include <Sable\Core\Common\DataTypes.h>
#include <Sable\Core\ManagedObject\ManagedObject.h>

#include <fbxsdk.h>

#include <QtCore/QDateTime>

namespace Dune
{

class CFBXObject;

/**
\ingroup ModelAsset

Wrapper to access element in the collada dom
*/
class CFBXDoc : public Sb::CManagedObject
{

	DEFINE_MANAGED_CLASS( CFBXDoc );

public:

	/** @name Constructor/Destructor*/
    //@{
    CFBXDoc( CFBXManager& p );
    ~CFBXDoc();
    //@}

	/** @name Accessors*/
	//@{
	Bool					IsValid() const;
	const Sb::CFilePath&	GetPath() const;
	FbxScene*				GetFBXScene() const;
	CFBXObject*				GetObject( String id );
	//@}

    /** @name Manipulator*/
    //@{
	Bool			ReloadIfNecessary();
	Void			ReLoad();
	Bool			Load( const Sb::CFilePath& filePath );
	Void			UnLoad( );
	//@}

private:

    // Types

	// Methods 
	FbxNode*				GetObject( FbxNode* root, FbxUInt64 id );

	// Attributes
	FbxScene*				m_FBXScene;
	Sb::CFilePath			m_Path;
	QDateTime 				m_LastLoadingDate;
	CFBXManager*			m_FBXManager;

};

};

#endif