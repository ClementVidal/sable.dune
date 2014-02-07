#ifndef _DUNE_MODEL_ELEMENT_SCENE_WORLD_
#define _DUNE_MODEL_ELEMENT_SCENE_WORLD_

#include <Dune/Model/Bank/Object.h>

namespace Dune
{

/**
*/
class CSceneWorld : public CBankObject
{

    DEFINE_MANAGED_CLASS( CSceneWorld )

public:

    /** @name Constructor/Destructor*/
    //@{
    CSceneWorld();
    ~CSceneWorld();
    //@}

    /** @name Accessors*/
    //@{
    //@}

    /** @name Manipulator*/
    //@{
    Void PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& group ) const;
    Void BakeObjectSet(CBakeContext& context, CBakeObjectSet& group );
    //@}

    /** @name Attributes names*/
    //@{
    //@}

private:

    // Attributes

};

};

#endif