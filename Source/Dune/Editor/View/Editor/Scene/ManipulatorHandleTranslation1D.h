#ifndef _DUNE_EDITOR_VIEW_EDITOR_SCENE_MANIPULATORHANDLETRANSLATION_
#define _DUNE_EDITOR_VIEW_EDITOR_SCENE_MANIPULATORHANDLETRANSLATION_

#include <Dune\Editor\View\Editor\Scene\Handle.h>

namespace Dune
{

/**
*/
class CManipulatorHandleTranslation1D : public CManipulatorHandle
{

public:

    /** @name Constructor/Destructor*/
    //@{
    CManipulatorHandleTranslation1D( const Sb::CColor& color, EAxis supportAxis );
    ~CManipulatorHandleTranslation1D();
	//@}

    /** @name Manipulator*/
    //@{
    Float32 Intersect( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m );
    Void    StartDrag( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m  );
    Void    Drag( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m, Sb::CVector3f& deltaTranslation, Sb::CEulerAngle& deltaRotation );
    Void    EndDrag( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m );
    Void    Highlight( );
    Void    CancelDrag( );
    //@}

private:

    // Method
    Void    InternalDraw( const Sb::CRenderer& renderer, const Sb::CMatrix4x4f& m );

	// Attributes
	Sb::CColor					m_Color;
	Sb::CVector3f				m_StartDragPos;
	Sb::CArray<Sb::CTriangle>	m_ArrowMesh;
};

};

#endif