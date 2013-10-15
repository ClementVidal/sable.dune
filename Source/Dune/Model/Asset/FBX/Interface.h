#ifndef _DUNE_MODEL_ASSET_FBX_INTERFACE_
#define _DUNE_MODEL_ASSET_FBX_INTERFACE_

#include <Sable\Core\Common\DataTypes.h>
#include <Sable\Core\Event\Header.h>
#include <Dune\Model\Asset\Asset.h>
#include <Dune\Model\Asset\FBX\Header.h>



namespace Dune
{

class CFBXObject;
class CFBXDoc;
class CPropertyString;
class CPropertyFilePath;

/**
\ingroup ModelAsset

This class provide properties and functions to bind the asset to a particular object in
a FBX document.
*/
class CFBXDocInterface : public CAsset
{

    DEFINE_MANAGED_CLASS( CFBXDocInterface )

public:

    /** @name Constructor/Destructor*/
    //@{
    CFBXDocInterface();
    ~CFBXDocInterface();
    //@}

    /** @name Accessors*/
    //@{
	CPropertyFilePath&	GetFBXDocFilePath() const;
	CPropertyString&	GetFBXId() const;
	CFBXDoc*			GetFBXDoc();
	CFBXObject*			GetFBXObject();
	//@}

    /** @name Manipulator*/
    //@{
	Void	ReloadFBXIfNecessary();
	Bool    Serialize( Sb::CPersistentArchive& ar );
	Void	SetFBXBinding( CFBXObject* fbxObj );
	virtual Void OnSetFBXBinding();
	//@}

	/** @name Attributes names*/
	//@{
	static String	PropFBXPath;
	static String	PropFBXId;
	//@}

	/** @name Signal*/
	//@{
	Sb::CEventSignal< CFBXDocInterface& >       SignalFBXBindingChange;
	//@}

private:

    // Attributes
	CPropertyFilePath*			m_PropertyFBXPath;	
	CPropertyString*			m_PropertyFBXId;
	Sb::CRef<CFBXObject>		m_FBXObject;

	// Methods
	Void OnSetFBXBindingInternal();

};

};

#endif