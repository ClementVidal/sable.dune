#ifndef _DUNE_GAME_OBJECT_
#define _DUNE_GAME_OBJECT_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/String/String.h>
#include <Sable/Core/Collection/Array.h>

#include <Dune/Model/Bank/Object.h>
#include <Dune/Model/Property/Property.h>


namespace Dune
{

class CBank;
class CHandle;
class CBakeObjectSet;

/**
\ingroup ModelAsset

Base class for any kind of Asset in the project
*/
class CGameObject : public CBankObject
{

    DEFINE_MANAGED_CLASS( CGameObject )

public:

    /** @name Constructor/Destructor*/
    //@{
	CGameObject( );
    ~CGameObject();
    //@}

    /** @name Accessors*/
    //@{
    Bool    GetIsEdited() const;
    Void    SetIsEdited( Bool onOff );
	//@}

    /** @name Manipulator*/
    //@{
	//@}

private:

    // Types

    // Methods 

    // Attributes
    Bool m_IsEdited;
};


};

#endif