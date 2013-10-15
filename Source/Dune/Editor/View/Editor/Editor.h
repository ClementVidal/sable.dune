#ifndef _DUNE_EDITOR_VIEW_EDITOR_EDITOR_
#define _DUNE_EDITOR_VIEW_EDITOR_EDITOR_

#include <Sable\Core\String\String.h>
#include <Sable\Core\Event\Header.h>

#include <QtGui\QFrame>
#include <QtGui\QWidget>
#include <QtCore\QTimer>

namespace Dune
{

class CApplication;
class CEditorDisplayWindow;
class CCtrlEditor;

/**
*/
class CViewEditor : public QFrame
{

public:

    /** @name Constructor/Destructor*/
    //@{
    CViewEditor( CApplication& app );
    ~CViewEditor ();
    //@}

    /** @name Constructor/Destructor*/
    //@{
    //@}

    /** @name Accessor*/
    //@{
    virtual CEditorDisplayWindow*   GetDisplayWindow() const = 0;
	virtual CCtrlEditor&		    GetCtrl() const = 0;
    CApplication&					GetApp() const;
	CProject*						GetProject() const;
    const Sb::CString&		        GetEditorName() const;
    //@}

    /** @name Manipulator*/
    //@{
	virtual Void OnMouseMove( const Sb::CVector2f& pos );
	virtual Void OnMouseLMBDown( const Sb::CVector2f& pos  );
	virtual Void OnMouseLMBUp( const Sb::CVector2f& pos );
    virtual Void OnMouseRMBDown( const Sb::CVector2f& pos  );
    virtual Void OnMouseRMBUp( const Sb::CVector2f& pos );
    virtual Void OnMouseMMBDown( const Sb::CVector2f& pos  );
    virtual Void OnMouseMMBUp( const Sb::CVector2f& pos );
	virtual Void OnMouseLDClick( const Sb::CVector2f& pos );
	virtual Void OnMouseRDClick( const Sb::CVector2f& pos );
    virtual Void OnKeyboardKeyDown( EKeyboardEventData data );
    virtual Void OnKeyboardKeyUp( EKeyboardEventData data );
    //@}

protected:

    // Attributes
    Sb::CString              m_EditorName;

    // Methods

private:

    // Types

    // Methods

    // Attributes
    CApplication*                                       m_App;
	Sb::CEventSlot<CViewEditor, Int32>	                m_FrameChangeSlot;
	Sb::CEventSlot<CViewEditor, CSableEditorRenderer&>   m_SlotEditorUpdate;
	Sb::CEventSlot<CViewEditor, CSableEditorRenderer&>   m_SlotEditorRender;

};

};

#endif