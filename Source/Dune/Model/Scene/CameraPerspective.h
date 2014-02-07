#ifndef _DUNE_MODEL_ELEMENT_SCENE_CAMERAPERSPECTIVE_
#define _DUNE_MODEL_ELEMENT_SCENE_CAMERAPERSPECTIVE_

#include <Dune/Model/Bank/Object.h>
#include <Dune/Model/Scene/NodeSpatial.h>

namespace Dune
{

/**
*/
class CCameraPerspective : public CNodeSpatial
{

    DEFINE_MANAGED_CLASS( CCameraPerspective )

public:

    /** @name Constructor/Destructor*/
    //@{
    CCameraPerspective();
    ~CCameraPerspective();
    //@}

    /** @name Accessors*/
    //@{
	CPropertyFloat32&	GetFieldOfView();
	CPropertyFloat32&	GetAspect();
	CPropertyFloat32&	GetNearPlane();
	CPropertyFloat32&	GetFarPlane();
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

    // Methods
    	
    // Attributes
	CPropertyFloat32*		m_FieldOfView;
	CPropertyFloat32*		m_Aspect;
	CPropertyFloat32*		m_NearPlane;
	CPropertyFloat32*		m_FarPlane;

};

};

#endif