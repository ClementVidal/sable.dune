#include <Dune\Editor\View\Editor\Editor.h>

#include <Sable\Input\Common\Manager.h>

#include <Dune\Common\Qt\KeyCodeTranslator.h>
#include <Dune\Editor\Application\Application.h>
#include <Dune\Editor\Sable\Application.h>
#include <Dune\Editor\View\Input\Mouse.h>

using namespace Dune;

CViewEditor::CViewEditor( CApplication& app ) :
    QFrame( &app.GetEditorContainerView() )
{
    m_App = &app;
    m_EditorName = "Editor";

}

CViewEditor::~CViewEditor()
{
}

CApplication& CViewEditor::GetApp() const
{
    return *m_App;
}

CProject* CViewEditor::GetProject() const
{
	return GetApp().GetProject();
}

Void CViewEditor::OnMouseMove( const Sb::CVector2f& pos )
{

}

Void CViewEditor::OnMouseRMBDown( const Sb::CVector2f& pos )
{
}

Void CViewEditor::OnMouseRMBUp( const Sb::CVector2f& pos  )
{
}

Void CViewEditor::OnMouseLMBDown( const Sb::CVector2f& pos )
{
}

Void CViewEditor::OnMouseLMBUp( const Sb::CVector2f& pos  )
{
}

Void CViewEditor::OnMouseMMBDown( const Sb::CVector2f& pos )
{
}

Void CViewEditor::OnMouseMMBUp( const Sb::CVector2f& pos  )
{
}

Void CViewEditor::OnMouseLDClick( const Sb::CVector2f& pos )
{

}

Void CViewEditor::OnMouseRDClick( const Sb::CVector2f& pos )
{

}

Void CViewEditor::OnKeyboardKeyDown( EKeyboardEventData data )
{

}

Void CViewEditor::OnKeyboardKeyUp( EKeyboardEventData data )
{

}

const Sb::CString& CViewEditor::GetEditorName() const
{
    return m_EditorName;
}

