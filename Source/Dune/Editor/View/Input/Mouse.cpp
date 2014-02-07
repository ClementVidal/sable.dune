
#include <Dune/Editor/View/Input/Mouse.h>

using namespace Dune;

IMPLEMENT_MANAGED_CLASS1( Dune, CViewInputMouse, Sb::CMouseInterface );

CViewInputMouse::CViewInputMouse( Sb::CApplicationWindow& window ) :
	Sb::CMouseInterface( window )
{
	m_State.XPosition = (Float32) 0.0f;
	m_State.YPosition = (Float32) 0.0f;
	m_PosX = 0;
	m_PosY = 0;
}

CViewInputMouse::~CViewInputMouse()
{

}

Void CViewInputMouse::Update()
{
	m_State.XPosition = (Float32) m_PosX;
	m_State.YPosition = (Float32) m_PosY;
}

Void CViewInputMouse::OnMouseLMBDown(  const Sb::CVector2f& pos  )
{
    SignalEvent( nMouseEventType_KeyDown, nMouseEventData_LeftBtn, pos );
}

Void CViewInputMouse::OnMouseLMBUp(  const Sb::CVector2f& pos  )
{
    SignalEvent( nMouseEventType_KeyUp, nMouseEventData_LeftBtn, pos  );
}

Void CViewInputMouse::OnMouseRMBDown(  const Sb::CVector2f& pos  )
{
    SignalEvent( nMouseEventType_KeyDown, nMouseEventData_RightBtn, pos  );
}

Void CViewInputMouse::OnMouseRMBUp(  const Sb::CVector2f& pos  )
{
    SignalEvent( nMouseEventType_KeyUp, nMouseEventData_RightBtn, pos  );
}

Void CViewInputMouse::OnMouseMMBDown(  const Sb::CVector2f& pos  )
{
    SignalEvent( nMouseEventType_KeyDown, nMouseEventData_MiddleBtn, pos  );
}

Void CViewInputMouse::OnMouseMMBUp(  const Sb::CVector2f& pos  )
{
    SignalEvent( nMouseEventType_KeyUp, nMouseEventData_MiddleBtn, pos  );
}

Void CViewInputMouse::OnMouseMove( const Sb::CVector2f& size,  const Sb::CVector2f& pos  )
{
    m_PosX = pos.GetX();
    m_PosY = pos.GetY();

	m_State.XPosition = (Float32) m_PosX;
	m_State.YPosition = (Float32) m_PosY;

    SignalEvent( nMouseEventType_Move, nMouseEventData_None, pos );
}
