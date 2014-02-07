#ifndef _DUNE_MODEL_BANK_OBJECTACCESSOR_
#define _DUNE_MODEL_BANK_OBJECTACCESSOR_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/ManagedObject/ManagedObject.h>

namespace Dune
{

class CBankObject;
class CHandle;

/**
\ingroup ModelAsset
*/
class CBankObjectAccessor : public Sb::CManagedObject
{

public:

    /** @name Constructor/Destructor*/
    //@{
	CBankObjectAccessor() {};
	~CBankObjectAccessor() {};
    //@}

    /** @name Accessors*/
    //@{
	virtual CBankObject*	GetObj( const CHandle& h ) const = 0;
    //@}

    /** @name Manipulator*/
    //@{
    //@}

private:

    // Attributes

};

};

#endif
