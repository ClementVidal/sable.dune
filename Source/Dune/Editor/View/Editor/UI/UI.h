#ifndef _DUNE_EDITOR_VIEW_EDITOR_UI_UI_
#define _DUNE_EDITOR_VIEW_EDITOR_UI_UI_

#include <Dune\Editor\View\Editor\Editor.h>

namespace Dune
{

class CBankObjectDirectory;
class CCtrlEditorUI;
class CEditorDisplayWindow;

/**
*/
class CViewEditorUI : public CViewEditor
{

	Q_OBJECT

public:

    /** @name Constructor/Destructor*/
    //@{
    CViewEditorUI( CApplication& app, const CHandle& h );
    virtual ~CViewEditorUI();
    //@}

    /** @name Accessor*/
    //@{
	CCtrlEditor&            GetCtrl() const;
    CEditorDisplayWindow*   GetDisplayWindow() const;
    //@}

    /** @name Manipulator*/
    //@{

    //@}

private:

    // Methods
	Void OutlineWidget( Sb::CRenderer& renderer, const Sb::CWidget& w, const Sb::CColor& col, UInt32 width );
	Void DrawRect( const Sb::CRect2Df& r, Sb::CDebugRenderPass& debugRP, Sb::CViewport& vp  );
	Void InitializeWidget();
    void showEvent( QShowEvent* event );
	void resizeEvent( QResizeEvent * event );

	Q_SLOT void OnMouseLMBUp( const Sb::CVector2f& pos );
	Q_SLOT void OnMouseLMBDown( const Sb::CVector2f& pos );
	Q_SLOT void OnMouseMove( const Sb::CVector2f& pos );
	Q_SLOT void OnMouseLDClick( const Sb::CVector2f& pos );
	Q_SLOT void OnMouseRDClick( const Sb::CVector2f& pos );
	Q_SLOT void OnRender( Sb::CRenderer& renderer );

    // Attributes
	UInt32						m_SelectableObjCount;
	CHandle						m_SelectableObj[32];
	Int32						m_SelectedObj;
    CHandle                     m_RootHandle;

    Sb::CRef<CCtrlEditorUI>	    m_Ctrl;
	QMenuBar*					m_MenuBar;
	QToolBar*					m_ToolBar;
	Bool						m_LMBDown;
    CEditorDisplayWindow*	    m_DisplayWindow;

    Sb::CVector2f                m_RendertargetSize;
};

};

#endif