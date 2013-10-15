#ifndef _DUNE_EDITOR_VIEW_EDITOR_SCENE_
#define _DUNE_EDITOR_VIEW_EDITOR_SCENE_

#include <Dune\Editor\View\Editor\Editor.h>
#include <Dune\Editor\View\Editor\Scene\Manipulator.h>
#include <Dune\Editor\Utilities\IconTable.h>

namespace Dune
{

class CBankObjectDirectory;
class CCtrlEditorScene;
class CEditorDisplayWindow;

/**
*/
class CViewEditorScene : public CViewEditor
{

    Q_OBJECT

public:

    /** @name Constructor/Destructor*/
    //@{
    CViewEditorScene( CApplication& app, const CHandle& h );
    virtual ~CViewEditorScene();
    //@}

    /** @name Accessor*/
    //@{
    CCtrlEditor&	        GetCtrl() const;
    CEditorDisplayWindow*   GetDisplayWindow() const;
    //@}

    /** @name Manipulator*/
    //@{
    //@}

private:

    // Methods
    Q_SLOT void OnUIEnableTranslationMode();
	Q_SLOT void OnUIEnableRotatationMode();
	Q_SLOT void OnUIFitSelectedObject();
	Q_SLOT void OnUIFitScene();
    Q_SLOT void OnRender( Sb::CRenderer& renderer );
    Q_SLOT void OnMouseLMBUp( const Sb::CVector2f& pos );
    Q_SLOT void OnMouseLMBDown( const Sb::CVector2f& pos );
    Q_SLOT void OnMouseRMBUp( const Sb::CVector2f& pos );
    Q_SLOT void OnMouseRMBDown( const Sb::CVector2f& pos );
    Q_SLOT void OnMouseMMBUp( const Sb::CVector2f& pos );
    Q_SLOT void OnMouseMMBDown( const Sb::CVector2f& pos );
    Q_SLOT void OnMouseMove( const Sb::CVector2f& pos );
    Q_SLOT void OnMouseLDClick( const Sb::CVector2f& pos );
    Q_SLOT void OnMouseRDClick( const Sb::CVector2f& pos );
    Q_SLOT void OnKeyboardKeyDown( EKeyboardEventData data );
    Q_SLOT void OnKeyboardKeyUp( EKeyboardEventData data );

	void resizeEvent( QResizeEvent * event );
	void showEvent( QShowEvent * event );
	Void InitializeWidget();

    // Attributes
	UInt32						m_SelectableObjCount;
	CHandle						m_SelectableObj[32];
	Int32						m_SelectedObj;

    Sb::CRef<CCtrlEditorScene>		m_Ctrl;
	QMenuBar*					    m_MenuBar;
	QToolBar*					    m_ToolBar;
	Bool						    m_LMBDown;

    Sb::CAABoxTree::SIntersection       m_TestBBox;
	Sb::CRay							m_TestRay;
	Sb::CVector2f						m_RendertargetSize;

    Sb::CMouseInterface::Signal         m_SignalMouseEventCamCtrl;
    Sb::CKeyboardInterface::Signal      m_SignalKeyboardEventCamCtrl;

	Sb::CMouseInterface::Signal         m_SignalMouseEventManipulator;

    CEditorDisplayWindow*	        m_DisplayWindow;
	CIconTable						m_IconTable;

    CHandle                         m_RootHandle;
};

};

#endif