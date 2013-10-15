#ifndef _DUNE_EDITOR_VIEW_EDITOR_SCENE_MANIPULATORHANDLETRANSLATION2D_
#define _DUNE_EDITOR_VIEW_EDITOR_SCENE_MANIPULATORHANDLETRANSLATION2D_

#include <Dune\Editor\View\Editor\Scene\Handle.h>

namespace Dune
{

/**
*/
class CManipulatorHandleTranslation2D : public CManipulatorHandle
{

public:

    /** @name Constructor/Destructor*/
    //@{
    CManipulatorHandleTranslation2D( const Sb::CColor& color1,  const Sb::CColor& color2, EAxis supportAxis );
    ~CManipulatorHandleTranslation2D();
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
	Sb::CColor		m_Color1;
	Sb::CColor		m_Color2;
    Sb::CVector3f    m_StartDragPos;

};

};

#endif