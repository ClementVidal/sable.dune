#ifndef _DUNE_EDITOR_VIEW_EDITOR_DISPLAYWINDOW_
#define _DUNE_EDITOR_VIEW_EDITOR_DISPLAYWINDOW_

#include <Sable\Core\String\String.h>
#include <Sable\Core\Event\Header.h>
#include <Sable\Application\Window.h>

#include <Dune\Editor\Sable\EditorRenderer.h>
#include <Dune\Editor\Sable\EditorWindow.h>

#include <QtGui\QFrame>
#include <QtGui\QWidget>
#include <QtCore\QTimer>

namespace Dune
{

class CApplication;
class CViewEditor;
class CCtrlEditor;

/**
*/
class CEditorDisplayWindow : public QFrame
{

	Q_OBJECT

public:

    // Constructor/Destructor
    CEditorDisplayWindow( QWidget& parent, const Sb::CSwapChainInfo& info );
    ~CEditorDisplayWindow();

    // Accessors
    CSableEditorWindow&					GetSableEditorWindow();

    Q_SIGNAL  void OnRender( Sb::CRenderer& renderer );
    Q_SIGNAL  void OnMouseLMBUp( const Sb::CVector2f& pos );
    Q_SIGNAL  void OnMouseLMBDown( const Sb::CVector2f& pos );
    Q_SIGNAL  void OnMouseRMBUp( const Sb::CVector2f& pos );
    Q_SIGNAL  void OnMouseRMBDown( const Sb::CVector2f& pos );
    Q_SIGNAL  void OnMouseMMBUp( const Sb::CVector2f& pos );
    Q_SIGNAL  void OnMouseMMBDown( const Sb::CVector2f& pos );
    Q_SIGNAL  void OnMouseMove( const Sb::CVector2f& pos );
    Q_SIGNAL  void OnMouseLDClick( const Sb::CVector2f& pos );
    Q_SIGNAL  void OnMouseRDClick( const Sb::CVector2f& pos );
    Q_SIGNAL  void OnKeyboardKeyDown( EKeyboardEventData data );
    Q_SIGNAL  void OnKeyboardKeyUp( EKeyboardEventData data );

private:

    // Methods
	Q_SLOT			Bool onIdle();

	QPaintEngine*	paintEngine() const;
	void			paintEvent(QPaintEvent *e);
	Void			mouseMoveEvent( QMouseEvent* event );
	Void			mousePressEvent( QMouseEvent* event );
	Void			mouseReleaseEvent( QMouseEvent* event );
	Void			mouseDoubleClickEvent( QMouseEvent* event );
    Void            keyPressEvent( QKeyEvent * event );
    Void            keyReleaseEvent( QKeyEvent * event );
	void            showEvent( QShowEvent * event );
	void			resizeEvent(  QResizeEvent * event  );

    // Attributes
	QSize							m_LastSize;
	QTimer							m_Timer;
    Sb::CSwapChainInfo				m_SwapChainInfo;
    Sb::CRef<CSableEditorWindow>    m_SableWindow;
};


};

#endif