#ifndef _DUNE_EDITOR_VIEW_EDITOR_SCENE_GIZMOCAMERA_
#define _DUNE_EDITOR_VIEW_EDITOR_SCENE_GIZMOCAMERA_

#include <Sable\Core\ManagedObject\ManagedObject.h>
#include <Sable\Core\Event\Slot.h>
#include <Dune\Editor\View\Editor\Scene\Gizmo.h>

namespace Dune
{

/**
*/
class CGizmoCamera : public CGizmo
{

public:

	/** @name DataTypes*/
	//@{
	//@}

    /** @name Constructor/Destructor*/
    //@{
    CGizmoCamera( CApplication& app, const CHandle& object );
    ~CGizmoCamera();
	//@}

    /** @name Accessors*/
    //@{
    //@}

    /** @name Manipulator*/
    //@{
    virtual Void    Render( const Sb::CRenderer& renderer );
    //@}

private:

	// Members
	Void	OnPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue  );
	Void	LoadMesh();

	// Attribute
	Sb::CArray<Sb::CTriangle>		m_CameraGizmoMesh;

};

};

#endif