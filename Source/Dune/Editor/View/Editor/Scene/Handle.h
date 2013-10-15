#ifndef _DUNE_EDITOR_VIEW_EDITOR_MANIPULATOR_HANDLE_
#define _DUNE_EDITOR_VIEW_EDITOR_MANIPULATOR_HANDLE_

#include <Sable\Core\ManagedObject\ManagedObject.h>
#include <Sable\Core\Common\DataTypes.h>

namespace Dune
{

/**
*/
class CManipulatorHandle : public Sb::CManagedObject
{

public:

    /** @name Constructor/Destructor*/
    //@{
    CManipulatorHandle(  EAxis supportAxis );
    ~CManipulatorHandle();
	//@}

    /** @name Manipulator*/
    //@{
    Void    Render( const Sb::CRenderer& renderer, const Sb::CMatrix4x4f& m );
    virtual Float32 Intersect( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m ) = 0;
    virtual Void    StartDrag( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m  ) = 0;
    virtual Void    Drag( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m , Sb::CVector3f& deltaTranslation, Sb::CEulerAngle& deltaRotation ) = 0;
    virtual Void    EndDrag( const Sb::CVector2f& p, const Sb::CCamera& cam, const Sb::CMatrix4x4f& m  ) = 0;
    virtual Void    CancelDrag( ) = 0;

    Bool    GetIsVisible() const;
    Void    SetIsVisible( Bool onOff );
    Void    SetIsHighlighted(Bool onOff );
    //@}

protected:

    // Methods
    virtual Void        InternalDraw( const Sb::CRenderer& renderer, const Sb::CMatrix4x4f& m ) = 0;
    EAxis               GetSupportAxis() const;
    Bool                GetIsHighlighted() const;

private:

    // Attributes
    Bool        m_IsVisible;
    EAxis       m_SupportAxis;
    Bool        m_IsHighlighted;

};

};

#endif