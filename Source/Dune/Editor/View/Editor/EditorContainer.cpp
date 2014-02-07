#include <Dune/Editor/View/Editor/EditorContainer.h> 

#include <Dune/Editor/View/Editor/Editor.h> 
#include <Dune/Editor/Sable/Application.h>

using namespace Dune;

CViewEditorContainer::CViewEditorContainer( CViewMainFrame& parent ) :
    QTabWidget( &parent ),
	CEventClient( parent.GetApp() )
{
    m_CurrentEditor = NULL;
	setTabsClosable( TRUE );

}

CViewEditorContainer::~CViewEditorContainer()
{
}

Void CViewEditorContainer::OnProjectDestroyed( )
{
    RemoveAllEditors();
}

Void CViewEditorContainer::BakeObject( const CHandle& h )
{
    if( GetCurrentEditor() )
    {
        GetCurrentEditor()->GetCtrl().GetBakeContext().BakeObject( h, TRUE, FALSE );
    }
}

Void CViewEditorContainer::BakeObject( )
{
	if( GetCurrentEditor() )
	{
		CHandle h = GetCurrentEditor()->GetCtrl().GetRootObject();
        GetCurrentEditor()->GetCtrl().GetBakeContext().BakeObject( h, TRUE, FALSE );
	}
}

Void CViewEditorContainer::OnProjectCreated( CProject& repo )
{
    RemoveAllEditors();
}

Void CViewEditorContainer::AddEditor( CViewEditor& editor, String name )
{
    if( GetEditor( name ) == NULL )
    {
        m_EditorTable.AddItem( name, &editor );
        
        addTab( &editor , name );
		SetCurrentEditor( name );
    }
}

Void CViewEditorContainer::RemoveAllEditors()
{
	Int32 i;
    for( i=0; i<count(); i++ )
    {
		QWidget* w = widget( i );
        delete w;
    }
	m_EditorTable.ClearAllItems();
	clear();
	m_CurrentEditor = NULL;

}

Void CViewEditorContainer::Initialize()
{
}

CViewEditor* CViewEditorContainer::GetEditor( String name )
{
    if( !m_EditorTable.HasItem( name ) )
        return NULL;

    return m_EditorTable.GetItem( name );
}

CViewEditor* CViewEditorContainer::GetCurrentEditor( )
{
    return m_CurrentEditor;
}

Void CViewEditorContainer::SetCurrentEditor( CViewEditor* editor )
{
    m_CurrentEditor = editor;
}

Void CViewEditorContainer::SetCurrentEditor( String name )
{
    m_CurrentEditor = GetEditor( name );
}