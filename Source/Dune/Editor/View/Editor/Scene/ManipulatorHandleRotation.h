#ifndef _DUNE_EDITOR_VIEW_EDITOR_SCENE_MANIPULATORHANDLEROTATION_
#define _DUNE_EDITOR_VIEW_EDITOR_SCENE_MANIPULATORHANDLEROTATION_

#include <Dune/Editor/View/Editor/Scene/Handle.h>

namespace Dune
{

/**
*/
class CManipulatorHandleRotation : public CManipulatorHandle
{

public:

    /** @name Constructor/Destructor*/
    //@{
    CManipulatorHandleRotation( const Sb::CColor& color, EAxis supportAxis );
    ~CManipulatorHandleRotation();
	//@}

    /** @name Manipulator*/
    //@{

    Float32 Intersect( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m );
    Void    StartDrag( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m  );
    Void    Drag( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m, Sb::CVector3f& deltaTranslation, Sb::CEulerAngle& deltaRotation );
    Void    EndDrag( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m  );
    Void    Highlight( );
    Void    CancelDrag( );
    //@}

private:

    // Method
    Void    InternalDraw( const Sb::CRenderer& renderer, const Sb::CMatrix4x4f& m );
	
	// Attributes
	Sb::CColor	    m_Color;
    Sb::CVector3f    m_StartDragPos;
    Sb::CVector3f    m_DragTangent;
    Sb::CVector3f    m_StartDragTangent;
    Bool        m_CanDisplayDirectionVector;
};

};

#endif