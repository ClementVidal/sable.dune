#ifndef _DUNE_EDITOR_CTRL_EDITOR_SCENE_
#define _DUNE_EDITOR_CTRL_EDITOR_SCENE_

#include <Sable\Core\Geometry\AABoxTree.h>
#include <Sable\Core\ManagedObject\Ref.h>
#include <Sable\Core\Event\Header.h>
#include <Sable\Graphics\Camera\Perspective.h>
#include <Sable\Input\Common\CameraController.h>

#include <Dune\Editor\Sable\EditorRenderer.h>
#include <Dune\Editor\View\Editor\Scene\Manipulator.h>
#include <Dune\Editor\View\Editor\Scene\Gizmo.h>
#include <Dune\Editor\Ctrl\Editor\Editor.h>

namespace Sable
{
    class CDebugRenderPass;
    class CRect2Df;
}

namespace Dune
{

class CViewEditorScene;
class CCameraPerspective;
class CNodeSpatial;

/** 
Controller class for USerInterfaceEditor
*/
class CCtrlEditorScene : public CCtrlEditor
{

public:

    /** @name DataTypes*/
	//@{
	typedef Sb::CList<Sb::CAABoxTree> AABoxTreeList;
	typedef Sb::CMap< CHandle, AABoxTreeList > AABoxTreeTable;
    //@}

    /** @name Constructor/Destructor*/
    //@{
    CCtrlEditorScene( CViewEditorScene& view, const CHandle& h );
    ~CCtrlEditorScene();
    //@}

    /** @name Accessor*/
    //@{
    CManipulator&				GetManipulator();
    const AABoxTreeTable&		GetAABoxTreeTable() const;
    const CHandle&				GetRootObject() const;
    Sb::CCameraPerspective&     GetSableCamera() ;
	Sb::CCameraController&  GetCameraController();
    //@}

    /** @name Manipulator*/
    //@{
	Void		StopDraging( const Sb::CVector2i& pos );
	Void		StartDraging( const Sb::CVector2i& pos );
	Bool		IsDraging() const;

	Void		MoveElement( const Sb::CVector2i& pos );
	Void		Initialize();

    CHandle     Pick( const Sb::CVector2f& pos, Sb::CAABoxTree::SIntersection& inter, Sb::CRay& r  );

	Void		FitObject( const CNodeSpatial& obj );
	Void		FitScene();

	Void		Render( Sb::CRenderer& renderer );
	//@}

private:

    // DataType
	typedef Sb::CMap< CHandle, Sb::CRef<CGizmo> > GizmoTable;

    // Attributes
    AABoxTreeTable              m_AABoxTreeTable;
    CManipulator                m_Manipulator;
    CHandle                     m_PickedNode;   
    CHandle                     m_RootElement;
    CSableEditorRenderer	    m_SableRenderer;
    Sb::CSceneView		        m_SableView;
	Sb::CCameraPerspective      m_SableCamera;
	Sb::CCameraController		m_CamCtrl;
	GizmoTable					m_GizmoTable;

    // Methods
	Void		OnProjectDestroyed();
	Void		OnEditElement( const CHandle& elem );
	Void		AddToAABoxTree( const CHandle& h );
	Void		OnAddObject( const CHandle& parent, const CHandle& h );
	Void		OnPropertyChanged( const CPropertyInterface& prop, const Sb::CString& oldValue );
	Void		OnRemoveObject( const CHandle& current );
	Void		OnObjectSelected( const CHandle& current );
	Void		OnObjectDeSelected( const CHandle& current );
	Void		AddCameraGizmo( CProject& p );
	Void		AddCameraGizmo( CCameraPerspective& p );	
	
};

}

#endif
