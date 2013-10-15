#include <Dune\Editor\View\Editor\Scene\Gizmo.h>

#include <Dune\Editor\Application\Application.h>

using namespace Dune;

CGizmo::CGizmo( CApplication& app, const CHandle& objHandle )
{
	DebugAssert( app.GetProject() );
	m_App = &app;
	m_ObjectHandle = objHandle;
	m_IsVisible = TRUE;
	m_IsHighlighted = FALSE;

	CBankObject* obj = app.GetProject()->GetObj( objHandle );
	DebugAssert( obj );
	EVENT_CONNECT_SLOT( obj->SignalPropertyChanged, m_SlotPropertyChange, &CGizmo::OnPropertyChanged );
}

CGizmo::~CGizmo()
{
}

Void CGizmo::OnPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue  )
{

}

CApplication& CGizmo::GetApp() const
{
	return *m_App;
}

CProject* CGizmo::GetProject() const
{
	return m_App->GetProject();
}

Bool CGizmo::GetIsHighlighted() const
{
	return m_IsHighlighted;
}

const CHandle& CGizmo::GetObjectHandle() const
{
	return m_ObjectHandle;
}

Void CGizmo::SetIsHighlighted( Bool isHighlighted )
{
	m_IsHighlighted = isHighlighted;
}

Bool CGizmo::GetIsVisible() const
{
    return m_IsVisible;
}

Void CGizmo::SetIsVisible( Bool isVisible )
{
	m_IsVisible = isVisible;
}