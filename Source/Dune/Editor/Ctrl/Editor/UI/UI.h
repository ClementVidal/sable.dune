#ifndef _DUNE_EDITOR_CTRL_EDITOR_UI_
#define _DUNE_EDITOR_CTRL_EDITOR_UI_

#include <Sable/Core/ManagedObject/Ref.h>
#include <Sable/Core/Event/Header.h>

#include <Dune/Editor/Sable/EditorRenderer.h>

#include <Dune/Editor/Ctrl/Editor/Editor.h>

namespace Sable
{
    class CWidget;
    class CDebugRenderPass;
    class CRect2Df;
}

namespace Dune
{

class CPropertyVector2;
class CViewEditorUI;
class CSableEditorRenderer;
class CCmdSetObjectProperty;

/** 
Controller class for USerInterfaceEditor
*/
class CCtrlEditorUI : public CCtrlEditor
{

public:

    /** @name Constructor/Destructor*/
    //@{
    CCtrlEditorUI( CViewEditorUI& view, const CHandle& h );
    virtual ~CCtrlEditorUI();
    //@}

    /** @name Accessor*/
    //@{
    const CHandle& GetRootObject() const;
    //@}

    /** @name Manipulator*/
    //@{
	UInt32		PickElement( const CHandle& root, const Sb::CVector2i& pos, UInt32 bufferSize, CHandle* buffer );

	Void		StopDraging( const Sb::CVector2i& pos );
	Void		StartDraging( const Sb::CVector2i& pos );
	Bool		IsDraging() const;

	Void		MoveElement( const Sb::CVector2i& pos );
	Void		Initialize();
	//@}

private:

	struct SDragedObject
	{
		CHandle                 Element;
		Sb::CVector2f	        StartDragPos;
        CCmdSetObjectProperty*  Command;
	};

    // DataType
	enum EPickingZone
	{
		nPickingZone_Move = 0,
		nPickingZone_Scale,
		nPickingZone_TextureMove,
		nPickingZone_TextureScale,
		nPickingZone_Count,
		nPickingZone_None = -1
	};

    // Attributes
	EPickingZone						m_DragingMode;
	Sb::CVector2f				        m_MouseStartDragPos;
	Sb::CArray<SDragedObject>			m_DragedObjectsList;
    CHandle                             m_RootElement;
    CSableEditorRenderer	            m_SableRenderer;
    Sb::CSceneView		                m_SableView;
	Sb::CSceneWorld						m_SableWorld;

    // Methods
	Void		OnProjectDestroyed();
	Void		OnEditElement( const CHandle& elem );
	Void		OnPropertyChanged( const CPropertyInterface& prop, const Sb::CString& oldValue );
	Void		OnRemoveObject( const CHandle& h );
	Void		OnChildIndexChanged( const CHandle& objtect, Int32 oldPos, Int32 newPos );
	void		AttachRootToEditor();

	CHandle			PickElement( const CHandle& root, const Sb::CVector2i& pos, UInt32 bufferSize, CHandle* buffer, UInt32& alreadyAdded );
	EPickingZone	FindPickingZone( const CHandle& elem, const Sb::CVector2i& pos );

};

}

#endif
