#ifndef _DUNE_VIEW_INPUT_MOUSE_MOUSE_
#define _DUNE_VIEW_INPUT_MOUSE_MOUSE_

#include <Sable\Core\Common\DataTypes.h>
#include <Sable\Core\ManagedObject\Header.h>
#include <Sable\Input\Mouse\Interface.h>
#include <Sable\Application\Window.h>


namespace Dune
{

/** 
*/
class CViewInputMouse : public Sb::CMouseInterface
{

    DEFINE_MANAGED_CLASS( CViewInputMouse );

public:

    /** @name Constructor/Destructor*/
    //@{
    CViewInputMouse( Sb::CApplicationWindow& window );
    virtual ~CViewInputMouse();
    //@}

    /** @name Accessors*/
    //@{
    //@}

    /** @name Manipulator*/
    //@{
	Void OnMouseLMBDown( const Sb::CVector2f& pos );
    Void OnMouseLMBUp( const Sb::CVector2f& pos );
    Void OnMouseRMBDown( const Sb::CVector2f& pos );
    Void OnMouseRMBUp( const Sb::CVector2f& pos );
    Void OnMouseMMBDown( const Sb::CVector2f& pos );
    Void OnMouseMMBUp( const Sb::CVector2f& pos );
    Void OnMouseMove(  const Sb::CVector2f& size,  const Sb::CVector2f& pos );
    Void				Update();
    //@}

private:

    // Attributes
    Float32		m_PosX;
    Float32		m_PosY;

};

}

#endif


