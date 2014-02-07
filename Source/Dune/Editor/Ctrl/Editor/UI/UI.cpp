#include <Dune/Editor/Ctrl/Editor/UI/UI.h>

#include <Dune/Editor/Ctrl/Project.h>
#include <Dune/Model/Gui/Header.h>
#include <Dune/Editor/View/Editor/UI/UI.h>
#include <Dune/Editor/Sable/EditorRenderer.h>

#include <Sable/Input/Keyboard/Interface.h>
#include <Sable/Gui/Widget/Widget.h>
#include <Sable/Graphics/Debug/RenderPass.h>

using namespace Dune;

CCtrlEditorUI::CCtrlEditorUI( CViewEditorUI& view, const CHandle& h  ) :
    CCtrlEditor( view )
{
    Initialize();
	OnEditElement( h );
}

CCtrlEditorUI::~CCtrlEditorUI()
{   
}

Void CCtrlEditorUI::Initialize()
{
    Sb::CApplicationWindow* window = &GetView().GetDisplayWindow()->GetSableEditorWindow();

	window->SetRenderer( &m_SableRenderer );
	m_SableView.SetWorld( &m_SableWorld );
	m_SableRenderer.SetView( &m_SableView );

	m_SableView.SetCamera( NULL );

	m_SableRenderer.Initialize( window->GetSwapChain().GetInfo().RenderTargetInfo );
	m_SableView.Initialize( );

	if( m_SableRenderer.GetGuiRenderPass() )
		m_SableRenderer.GetGuiRenderPass()->ShowCursor( FALSE );
}

Void CCtrlEditorUI::OnProjectDestroyed()
{
    if( GetApp().GetEditorContainerView().GetCurrentEditor() )
        GetApp().GetEditorContainerView().GetCurrentEditor()->setVisible( FALSE );

    CCtrl::OnProjectDestroyed();
}

Void CCtrlEditorUI::MoveElement( const Sb::CVector2i& pos )
{
	UInt32 i;
	CPropertyVector2* changedProperty = NULL;
	Sb::CVector2i delta, lastPos, newValue;

	for(i=0;i<m_DragedObjectsList.GetItemCount();i++)
	{
		changedProperty = NULL;

		if( m_DragingMode != nPickingZone_None )
		{
			delta = m_MouseStartDragPos - pos;

			CWidget* widget = GetProject()->GetObj<CWidget>( m_DragedObjectsList[i].Element );
			CWidgetFrame* pane = GetProject()->GetObj<CWidgetFrame>( m_DragedObjectsList[i].Element );

			if( widget )
			{
				Sb::CVector2f finalPos = m_DragedObjectsList[i].StartDragPos - delta;

				if( m_DragingMode == nPickingZone_Move )
					widget->GetPosition().SetValue( Sb::CVector2i( finalPos.GetX(), finalPos.GetY() ) );
				else if( m_DragingMode == nPickingZone_Scale )
					widget->GetSize().SetValue( Sb::CVector2i( finalPos.GetX(), finalPos.GetY() ) );
				else if( pane )
				{
					finalPos = m_DragedObjectsList[i].StartDragPos + delta;

					if( m_DragingMode == nPickingZone_TextureMove )
						pane->GetTextureOrigin().SetValue( finalPos );
					else if( m_DragingMode == nPickingZone_TextureScale )
						pane->GetTextureSize().SetValue( finalPos );
				}
			}
		}
	}
}

Void CCtrlEditorUI::StartDraging( const Sb::CVector2i& pos )
{
	m_MouseStartDragPos = pos;
	m_DragingMode = nPickingZone_Move;

	CCtrlProject::ObjectList::Iterator it;
	const CCtrlProject::ObjectList& list = GetApp().GetProjectCtrl().GetSelectedObjects();
	m_DragedObjectsList.SetItemCount( list.GetItemCount() );

	ForEachItem( it, list )
	{
		m_DragingMode = FindPickingZone( (*it), pos );

		if( m_DragingMode != nPickingZone_None)
		{
			break;
		}
	}

	UInt32 i = 0;
	ForEachItem( it, list )
	{
		m_DragedObjectsList[i].Element = (*it);
		
		CWidget* widget = GetProject()->GetObj<CWidget>( (*it) );
		if( widget )
		{
			if( m_DragingMode == nPickingZone_Move )
            {
				Sb::CVector2i tmp = widget->GetPosition().GetValue();
				m_DragedObjectsList[i].StartDragPos = Sb::CVector2f( tmp.GetX(), tmp.GetY() );
                m_DragedObjectsList[i].Command = NEWOBJ( CCmdSetObjectProperty, ( GetApp().GetProjectCtrl(),  widget->GetPosition(), Sb::CVector2f::Zero ) );
            }
			else if( m_DragingMode == nPickingZone_Scale )
            {
				Sb::CVector2i tmp = widget->GetSize().GetValue();
				m_DragedObjectsList[i].StartDragPos = Sb::CVector2f( tmp.GetX(), tmp.GetY() );
                m_DragedObjectsList[i].Command = NEWOBJ( CCmdSetObjectProperty, ( GetApp().GetProjectCtrl(),  widget->GetSize(), Sb::CVector2f::Zero ) );
            }
		}
		CWidgetFrame* pane = GetProject()->GetObj<CWidgetFrame>( (*it) );
		if( pane )
		{
			if( m_DragingMode == nPickingZone_TextureMove )
            {
				m_DragedObjectsList[i].StartDragPos = pane->GetTextureOrigin().GetValue();
                m_DragedObjectsList[i].Command = NEWOBJ( CCmdSetObjectProperty, ( GetApp().GetProjectCtrl(),  pane->GetTextureOrigin(), Sb::CVector2f::Zero ) );
            }
			else if( m_DragingMode == nPickingZone_TextureScale )
            {
				m_DragedObjectsList[i].StartDragPos = pane->GetTextureSize().GetValue();
                m_DragedObjectsList[i].Command = NEWOBJ( CCmdSetObjectProperty, ( GetApp().GetProjectCtrl(),  pane->GetTextureSize(), Sb::CVector2f::Zero ) );
            }
		}
		i++;
	}
}

Bool CCtrlEditorUI::IsDraging() const
{
	return m_DragingMode != nPickingZone_None;
}

Void CCtrlEditorUI::StopDraging( const Sb::CVector2i& pos )
{
	Sb::CVector2i delta = m_MouseStartDragPos - pos;

    UInt32 i;
    for( i=0;i<m_DragedObjectsList.GetItemCount();i++ )
    {
        Sb::CVector2i startPos = m_DragedObjectsList[i].StartDragPos;
        Sb::CVector2i newPos = startPos - delta;
        m_DragedObjectsList[i].Command->SetNewValue( CLexicalCast::ValueToString( newPos ) );
        GetApp().GetCommandProcessor().Submit( *m_DragedObjectsList[i].Command );
    }
	m_DragedObjectsList.ClearAllItems();	
	m_DragingMode = nPickingZone_None;
}

Void CCtrlEditorUI::OnEditElement( const CHandle& h )
{
	m_RootElement = h;

	// Rebake everything starting from root element
	GetBakeContext().BakeObject( m_RootElement, TRUE, FALSE );

	AttachRootToEditor();
}

Void CCtrlEditorUI::OnPropertyChanged( const CPropertyInterface& prop, const Sb::CString& oldValue )
{
	CCtrlEditor::OnPropertyChanged( prop, oldValue );
	AttachRootToEditor();
}

Void CCtrlEditorUI::OnChildIndexChanged( const CHandle& objtect, Int32 oldPos, Int32 newPos )
{
	CCtrlEditor::OnChildIndexChanged( objtect, oldPos, newPos );
	AttachRootToEditor();
}

/**

*/
Void CCtrlEditorUI::OnRemoveObject( const CHandle& h )
{
	CCtrlEditor::OnRemoveObject( h );

	CBakeObjectSet* set = GetBakeObjectSet( h );
	if( !set )
		return;

	Sb::CWidget* widget = set->GetObj<Sb::CWidget>();
	if( !widget )
		return;

	// Detach the widget from it's parent
	widget->SetParent( NULL );

	AttachRootToEditor();
}

void CCtrlEditorUI::AttachRootToEditor()
{
	// Attach it as the root of the UI
	CBakeObjectSet* set = GetBakeObjectSet( m_RootElement );
	Sb::CWidget* widget = set->GetObj<Sb::CWidget>();
	
	DebugAssert( widget );
	DebugAssert( m_SableRenderer.GetGuiRenderPass() );
	widget->SetParent( &m_SableRenderer.GetView()->GetWorld()->GetGuiRootNode() );
}

const CHandle& CCtrlEditorUI::GetRootObject() const
{
    return m_RootElement;
}

CCtrlEditorUI::EPickingZone CCtrlEditorUI::FindPickingZone( const CHandle& elem, const Sb::CVector2i& pos )
{		
    CBakeObjectSet* set = GetBakeObjectSet( elem );
	if( set )
	{
		Sb::CWidget* w = (Sb::CWidget*)set->GetObj<Sb::CWidget>();
		if( w == NULL )
		{
			return nPickingZone_None;
		}

		Sb::CRect2Di moveZone = w->GetRect();
		Sb::CRect2Di scaleZone;
		scaleZone.SetSize( Sb::CVector2i( 16, 16 ) );
		scaleZone.SetPosition( moveZone.GetPosition() + moveZone.GetSize() / 2 );

		// Test Scale zone
		if( InputManager.GetKeyboard()->GetIsKeyDown( nKeyboardEventData_LeftShift ) )
		{
			if( scaleZone.IsInside( pos ) )
			{
				return nPickingZone_TextureScale;
			}
			else if( moveZone.IsInside( pos ) )
			{
				return nPickingZone_TextureMove;
			}
		}
		else
		{
			if( scaleZone.IsInside( pos ) )
			{
				return nPickingZone_Scale;
			}
			else if( moveZone.IsInside( pos ) )
			{
				return nPickingZone_Move;
			}
		}
	}

	return nPickingZone_None;
}

UInt32 CCtrlEditorUI::PickElement( const CHandle& root, const Sb::CVector2i& pos, UInt32 bufferSize, CHandle* buffer )
{
	UInt32 alreadyPicked = 0;
	CHandle h = PickElement( root, pos, bufferSize, buffer, alreadyPicked );
	if( h.IsValid() )
	{
		if( alreadyPicked < bufferSize )
			buffer[alreadyPicked++] = h;
	}
	return alreadyPicked;
}

CHandle CCtrlEditorUI::PickElement( const CHandle& root, const Sb::CVector2i& pos, UInt32 bufferSize, CHandle* buffer, UInt32& alreadyPickedCount )
{
	UInt32 i;

	CWidget* el = GetProject()->GetObj<CWidget>( root );
	if( el == NULL )
		return CHandle();

	for( i=0;i<el->GetChildCount();i++ )
	{
		CHandle h = PickElement( el->GetChildAtIndex( i ), pos, bufferSize, buffer, alreadyPickedCount );
		if( h.IsValid() )
		{
			if( alreadyPickedCount < bufferSize )
				buffer[alreadyPickedCount++] = h;
		}
	}

    CBakeObjectSet* set = GetBakeObjectSet( root );
	if( set )
	{
		Sb::CWidget* w = (Sb::CWidget*)set->GetObj<Sb::CWidget>();
		if( w && w->GetRect().IsInside( pos ) )
		{
			if( !GetProject()->GetObj( root )->GetTypeInfo().IsA( CBankObjectDirectory::GetStaticTypeInfo() ) ) 
				return root;
		}
	}
	
	return CHandle();
}

