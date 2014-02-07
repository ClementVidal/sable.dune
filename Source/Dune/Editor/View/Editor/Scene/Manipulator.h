#ifndef _DUNE_EDITOR_VIEW_EDITOR_MANIPULATOR_MANIPULATOR_
#define _DUNE_EDITOR_VIEW_EDITOR_MANIPULATOR_MANIPULATOR_

#include <Sable/Core/ManagedObject/ManagedObject.h>
#include <SAble/Core/Math/Matrix4x4f.h>
#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Input/Mouse/Event.h>

namespace Sable
{
    class CCamera;
}

namespace Dune
{

class CCtrlEditorScene;
class CManipulatorHandle;

/**
*/
class CManipulator : public Sb::CManagedObject
{

public:

	/** @name Constructor/Destructor*/
	//@{
	enum EType
	{
		nType_Rotation = 0,
		nType_Translation1D,
		nType_Translation2D,
		nType_Count,
		nType_None = -1
	};
    enum EMode
    {
        nMode_Translation = 0,
        nMode_Rotation,
        nMode_Count,
        nMode_None = -1
    };
	//@}

    /** @name Constructor/Destructor*/
    //@{
    CManipulator( CApplication& app );
    ~CManipulator();
	//@}

    /** @name Accessors*/
    //@{
    Bool                GetIsDragging() const;
	Void				SetAttachedNode( CHandle h );
    CHandle             GetAttachedNode() const;
    CManipulatorHandle* GetCurrentHandle() const;
    Void                SetMode( EMode mode );
	CApplication&		GetApp() const;
	CProject*			GetProject() const;
    //@}

    /** @name Manipulator*/
    //@{
    Void    Render( const Sb::CRenderer& renderer );
	Void	Initialize( Sb::CMouseInterface::Signal& mouseSignal, CCtrlEditorScene& ctrl );
    //@}

private:

    // DataTypes
    typedef Sb::CRef<CManipulatorHandle> HandleTable[nAxis_Count];

	// Members
	Void	OnPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue  );
	Void	OnMouseEvent( EMouseEventType type, EMouseEventData data, const Sb::CVector2f& pos );
	Void    StartDrag( const Sb::CVector2f& p );
	Void    Drag( const Sb::CVector2f& p );
	Void    EndDrag( const Sb::CVector2f& p );
	Void    Move( const Sb::CVector2f& pos );
	Void    CancelDrag();
    Void    Show( Bool onOff );
    Sb::CMatrix4x4f  GetTransformMatrix() const;

    // Attributes
	Sb::CVector2f			m_LastScreenPos;
	UInt32					m_DragState;
	EMouseEventData			m_DragBtn;

    Sb::CMatrix4x4f          m_Transformation;

    Sb::CMatrix4x4f          m_StartDragTransformation;

    Sb::CCamera*            m_Camera;
    CManipulatorHandle*     m_CurrentHandle;
    HandleTable             m_HandleTable[nType_Count];
	CHandle					m_AttachedNode;
	EMode					m_Mode;
    CCtrlEditorScene*       m_Ctrl;
	CApplication*			m_App;

	Sb::CEventSlot< CManipulator, const CPropertyInterface&, const Sb::CString& >	m_SlotPropertyChanged;
	Sb::CEventSlot< CManipulator, EMouseEventType, EMouseEventData, const Sb::CVector2f& > m_SlotMouseEvent;
};

};

#endif