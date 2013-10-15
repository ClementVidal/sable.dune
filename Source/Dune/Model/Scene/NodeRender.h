#ifndef _DUNE_MODEL_ELEMENT_SCENE_NODERENDER_
#define _DUNE_MODEL_ELEMENT_SCENE_NODERENDER_

#include <Dune\Model\Bank\Object.h>
#include <Dune\Model\Scene\NodeSpatial.h>

namespace Dune
{

/**
*/
class CNodeRender : public CNodeSpatial
{

    DEFINE_MANAGED_CLASS( CNodeRender )

public:

    /** @name Constructor/Destructor*/
    //@{
    CNodeRender();
    ~CNodeRender();
    //@}

    /** @name Accessors*/
    //@{
    CPropertyHandle&	GetModel() const;
    //@}

    /** @name Manipulator*/
    //@{
    Void PrepareBakeObjectSet( CBakeContext& context, CBakeObjectSet& group ) const;
    Void BakeObjectSet(CBakeContext& context, CBakeObjectSet& group );
    //@}

    /** @name Attributes names*/
    //@{
    static String PropModel;
    //@}

private:

    // Methods
    	
    // Attributes
    CPropertyHandle*    m_Model;

};

};

#endif