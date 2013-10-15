#include <Dune\Editor\View\Editor\DisplayWindow.h>

#include <Sable\Input\Common\Manager.h>

#include <Dune\Common\Qt\KeyCodeTranslator.h>
#include <Dune\Editor\Application\Application.h>
#include <Dune\Editor\Sable\Application.h>
#include <Dune\Editor\View\Input\Mouse.h>

using namespace Dune;

CEditorDisplayWindow::CEditorDisplayWindow( QWidget& parent, const Sb::CSwapChainInfo& info ) :
    QFrame( &parent )
{
    m_SwapChainInfo = info;

	setAttribute( Qt::WA_OpaquePaintEvent ); 
	setAttribute(Qt::WA_NativeWindow); 

	setAutoFillBackground(false); 

	QObject::connect( &m_Timer, SIGNAL( timeout() ), this, SLOT( onIdle() ) ) ;

	m_Timer.setParent( this );
	m_Timer.setInterval( 0 ) ;
	m_Timer.setSingleShot( false ) ; 

    setMouseTracking( TRUE );
    setFocusPolicy(Qt::StrongFocus);

}

CEditorDisplayWindow::~CEditorDisplayWindow()
{
}

CSableEditorWindow& CEditorDisplayWindow::GetSableEditorWindow()
{
    return *m_SableWindow;
}

void CEditorDisplayWindow::showEvent( QShowEvent * event )
{
    if( m_SableWindow == NULL )
    {
		m_SwapChainInfo.RenderTargetInfo.TextureInfo.Width = size().width();
		m_SwapChainInfo.RenderTargetInfo.TextureInfo.Height = size().height();

        m_SableWindow = NEWOBJ( CSableEditorWindow, (  ) );
        m_SableWindow->Initialize( m_SwapChainInfo, (UInt32) winId() );

        CViewInputKeyboard* kbd = NEWOBJ( CViewInputKeyboard, (*m_SableWindow) );
        CViewInputMouse* mouse = NEWOBJ( CViewInputMouse, (*m_SableWindow) );

        InputManager.SetMouse( mouse );
        InputManager.SetKeyboard( kbd );
    }

    QFrame::showEvent( event );
}

Void CEditorDisplayWindow::mouseMoveEvent( QMouseEvent* event )
{
    if( !InputManager.GetMouse() )
        return;

	Sb::CVector2f pos;
	Sb::CViewport& viewport = GetSableEditorWindow().GetSwapChain().GetViewport();
	Float32 ratio =  (Float32) viewport.GetWidth() /  (Float32) viewport.GetHeight();
	pos.SetX( (Float32) event->posF().x() / (Float32) viewport.GetWidth() );
	pos.SetY( (Float32) event->posF().y() / (Float32) viewport.GetHeight() );

    CViewInputMouse* mouse = ((CViewInputMouse*)InputManager.GetMouse());
    if( mouse )
    {
        mouse->OnMouseMove( Sb::CVector2f( (Float32) viewport.GetWidth(), (Float32)viewport.GetHeight() ), pos );
		Q_EMIT OnMouseMove( pos );
	}

}

Void CEditorDisplayWindow::keyPressEvent( QKeyEvent * event )
{
    if( event->isAutoRepeat() )
        return;

	if( !geometry().contains( mapFromGlobal( QCursor::pos() ) ) )
		return;

    EKeyboardEventData key = CKeyCodeTranslator::Translate( (Qt::Key) event->key() );
    if( key != nKeyboardEventData_None )
    {
        Q_EMIT OnKeyboardKeyDown( key );
    }
    else
    {
        QFrame::keyPressEvent( event );
    }
}

Void CEditorDisplayWindow::keyReleaseEvent( QKeyEvent * event )
{
    if( event->isAutoRepeat() )
        return;

    EKeyboardEventData key = CKeyCodeTranslator::Translate( (Qt::Key) event->key() );
    if( key != nKeyboardEventData_None )
    {
        Q_EMIT OnKeyboardKeyUp( key );
    }
    else
    {
       QFrame::keyPressEvent( event );
    }
}

Void CEditorDisplayWindow::mouseReleaseEvent( QMouseEvent* event )
{
    if( !InputManager.GetMouse() )
        return;

	Sb::CVector2f pos;
	Sb::CViewport& viewport = GetSableEditorWindow().GetSwapChain().GetViewport();
	Float32 ratio =  (Float32) viewport.GetWidth() /  (Float32) viewport.GetHeight();
	pos.SetX( (Float32) event->posF().x() / (Float32) viewport.GetWidth() );
	pos.SetY( (Float32) event->posF().y() / (Float32) viewport.GetHeight() );

	CViewInputMouse* mouse = ((CViewInputMouse*)InputManager.GetMouse());

	if( event->button() == Qt::LeftButton )
	{
		mouse->OnMouseLMBUp( pos );
		Q_EMIT OnMouseLMBUp( pos );
	} 
	else if( event->button() == Qt::RightButton )
	{
		mouse->OnMouseRMBUp( pos );
		Q_EMIT OnMouseRMBUp( pos );
	}
    else if( event->button() == Qt::MiddleButton )
    {
        mouse->OnMouseMMBUp( pos );
		Q_EMIT OnMouseMMBUp( pos );
    }

}

Void CEditorDisplayWindow::mousePressEvent( QMouseEvent* event )
{
    if( !InputManager.GetMouse() )
        return;

	Sb::CVector2f pos;
	Sb::CViewport& viewport = GetSableEditorWindow().GetSwapChain().GetViewport();
	Float32 ratio =  (Float32) viewport.GetWidth() /  (Float32) viewport.GetHeight();
	pos.SetX( (Float32) event->posF().x() / (Float32) viewport.GetWidth() );
	pos.SetY( (Float32) event->posF().y() / (Float32) viewport.GetHeight() );

	CViewInputMouse* mouse = ((CViewInputMouse*)InputManager.GetMouse());

	if( event->button() == Qt::LeftButton )
	{
		mouse->OnMouseLMBDown( pos );
		Q_EMIT OnMouseLMBDown( pos );
	} 
	else if( event->button() == Qt::RightButton )
	{
		mouse->OnMouseRMBDown( pos );
		Q_EMIT OnMouseRMBDown( pos );
	}
    else if( event->button() == Qt::MiddleButton )
    {
        mouse->OnMouseMMBDown( pos );
		Q_EMIT OnMouseMMBDown( pos );
    }
}

Void CEditorDisplayWindow::mouseDoubleClickEvent( QMouseEvent* event )
{
    if( !InputManager.GetMouse() )
        return;

	Sb::CVector2f pos;
	Sb::CViewport& viewport = GetSableEditorWindow().GetSwapChain().GetViewport();
	Float32 ratio =  (Float32) viewport.GetWidth() /  (Float32) viewport.GetHeight();
	pos.SetX( (Float32) event->posF().x() / (Float32) viewport.GetWidth() );
	pos.SetY( (Float32) event->posF().y() / (Float32) viewport.GetHeight() );

	CViewInputMouse* mouse = ((CViewInputMouse*)InputManager.GetMouse());
	if( event->button() == Qt::LeftButton )
	{
		Q_EMIT OnMouseLDClick( pos );
	}
	else if( event->button() == Qt::RightButton )
	{
		Q_EMIT OnMouseRDClick( pos );
	}
    else if( event->button() == Qt::MiddleButton )
    {

    }
}

QPaintEngine* CEditorDisplayWindow::paintEngine() const 
{ 
	return 0; 
} 

void CEditorDisplayWindow::paintEvent(QPaintEvent *e)
{
	if( m_Timer.isActive() == FALSE )
		m_Timer.start();
	QWidget::paintEvent(e);
}

void CEditorDisplayWindow::resizeEvent(  QResizeEvent * event  )
{
	if( m_SableWindow )
	{
		if( m_LastSize != size() )
		{
			Sb::CVector2i newSize( size().width(), size().height() );
			m_SableWindow->Resize( newSize );

			m_LastSize = size();

		}
	}
	else if( isVisible() )
	{
		DebugAssert( m_SableWindow == NULL )

		m_LastSize = size();

		m_SwapChainInfo.RenderTargetInfo.TextureInfo.Width = size().width();
		m_SwapChainInfo.RenderTargetInfo.TextureInfo.Height = size().height();

		m_SableWindow = NEWOBJ( CSableEditorWindow, (  ) );
		m_SableWindow->Initialize( m_SwapChainInfo, (UInt32) winId() );

		CViewInputKeyboard* kbd = NEWOBJ( CViewInputKeyboard, (*m_SableWindow) );
		CViewInputMouse* mouse = NEWOBJ( CViewInputMouse, (*m_SableWindow) );

		InputManager.SetMouse( mouse );
		InputManager.SetKeyboard( kbd );

	}
}

Bool CEditorDisplayWindow::onIdle()
{
    if( m_SableWindow )
    {
        if( m_SableWindow->GetRenderer() )
        {
            Q_EMIT OnRender( * m_SableWindow->GetRenderer() );
        }
        m_SableWindow->Update();   
    }

	return TRUE;
}
