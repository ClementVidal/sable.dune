#ifndef _DUNE_MODEL_ELEMENT_SCENE_NODESPATIAL_
#define _DUNE_MODEL_ELEMENT_SCENE_NODESPATIAL_

#include <Dune/Model/Bank/Object.h>

namespace Dune
{

/**
*/
class CNodeSpatial : public CBankObject
{

    DEFINE_MANAGED_CLASS( CNodeSpatial )

public:

    /** @name Constructor/Destructor*/
    //@{
    CNodeSpatial();
    ~CNodeSpatial();
    //@}

    /** @name Accessors*/
    //@{
	CPropertyVector3f&	GetPosition() const;
	CPropertyVector3f&	GetRotation() const;
	CPropertyMatrix4x4& GetTransformation() const;
    //@}

    /** @name Manipulator*/
    //@{
	Void BakeObjectSet(CBakeContext& context, CBakeObjectSet& objSet );
    //@}

    /** @name Attributes names*/
    //@{
    //@}

private:

    // Methods
    Void        OnPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue );
    	
    // Attributes
    CPropertyVector3f*		m_Position;
    CPropertyVector3f*		m_Rotation;
	CPropertyMatrix4x4*		m_Transformation;

};

};

#endif