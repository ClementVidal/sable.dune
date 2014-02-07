#ifndef _DUNE_EDITOR_WXVIEW_
#define _DUNE_EDITOR_WXVIEW_

#include <Sable/Core/Collection/Map.h>
#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/Event/Header.h>

#include <Dune/Editor/Ctrl/EventClient.h>

#include <QtGui/QTabWidget>


namespace Dune
{

class CViewEditor;
class CViewEditorUI;

/**
UI representation of a project
*/
class CViewEditorContainer : public QTabWidget, public CEventClient
{

public:

    // Data Types

    /** @name Constructor/Destructor*/
    //@{
    CViewEditorContainer( CViewMainFrame& parent );
    virtual ~CViewEditorContainer();
    //@}

    /** @name Accessors*/
    //@{
    Void			RemoveAllEditors();
    Void			AddEditor( CViewEditor& editor, String name );

    CViewEditor*	GetCurrentEditor( );
    Void			SetCurrentEditor( String name );

    Void			BakeObject( const CHandle& h );
	Void			BakeObject( );
    //@}

    /** @name Manipulator*/
    //@{
    Void Initialize();
    //@}

private:

    // Types
 
    // Methods
    Void			OnProjectCreated( CProject& repo );
	Void			OnProjectDestroyed( );

    CViewEditor*	GetEditor( String name );
    Void			SetCurrentEditor( CViewEditor* editor );

    // Attributes
    CViewEditor*									m_CurrentEditor;
    Sb::CMap<Sb::CString, CViewEditor*>		m_EditorTable;

};

}

#endif
