#ifndef _DUNE_MODEL_ASSET_FBX_GRAPHCONVERTER_
#define _DUNE_MODEL_ASSET_FBX_GRAPHCONVERTER_

#include <Sable\Core\Common\DataTypes.h>

namespace Dune
{

/**
\ingroup ModelAsset

Helper class used to process a collada node graph
*/
class CFBXGraphConverter 
{

public:

    /** @name Constructor/Destructor*/
    //@{
    CFBXGraphConverter( /*domNode& daeNode, Bool fixYUp*/ );
    ~CFBXGraphConverter();
    //@}	

    /** @name Accessors*/
    //@{
	//@}

    /** @name Manipulator*/
    //@{
	Sb::CVector3f GetWorldPosition() const;
	//@}

private:

    // Attributes
	
	Bool			m_FixYUp;

	// Functions

};

};

#endif