#ifndef _DUNE_MODEL_ASSET_FBX_OBJECT_
#define _DUNE_MODEL_ASSET_FBX_OBJECT_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/ManagedObject/ManagedObject.h>

#include <fbxsdk.h>

namespace Dune
{

/**
\ingroup ModelAsset
*/
class CFBXObject : public Sb::CManagedObject
{

	DEFINE_MANAGED_CLASS( CFBXObject );

public:

	/** @name Constructor/Destructor*/
    //@{
    CFBXObject( CFBXDoc& doc, FbxObject* obj );
    ~CFBXObject();
    //@}

	/** @name Accessors*/
	//@{
	FbxObject*	GetFBXObject() const;
	CFBXDoc&	GetDoc() const;
	String		GetName() const;
	String		GetUniqueId() const;
	//@}

    /** @name Manipulator*/
    //@{
	//@}

private:

    // Types

    // Methods 

	// Attributes
	Sb::CRef<CFBXDoc>		m_Doc;
	FbxObject*				m_FBXObject;
	Sb::CStringIdentifier	m_Id;

};

};

#endif