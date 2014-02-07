#ifndef _DUNE_EDITOR_VIEW_GAMEOBJECT_
#define _DUNE_EDITOR_VIEW_GAMEOBJECT_

#include <Sable/Core/Common/DataTypes.h>

#include <Dune/Editor/Ctrl/EventClient.h>
#include <Dune/Editor/Utilities/IconTable.h>
#include <Dune/Editor/View/GameTree.h>
#include <Dune/Editor/Widget/ObjectLibrary.h>

#include <QtGui/QDockWidget>
#include <QtGui/QToolBar>

namespace Dune
{

/**
Bank view
*/
class CViewGame : public QDockWidget, public CEventClient
{

	Q_OBJECT 

public:

    /** @name Constructor/Destructor*/
    //@{
    CViewGame( CViewMainFrame& mainFrame );
    ~CViewGame();
    //@}

    /** @name Accesors*/
    //@{
	CViewGameTree&	GetTree();
	QIcon	GetIcon( const Sb::CTypeInfo& type ) const;
	//@}

    /** @name Manipulator*/
	//@{
	Void			ConfigureAddGameMenu( QMenu& subMenu );
    //@}

private:

    // friendship

    // Types

    // Methods
	Q_SLOT void		OnUISelectLibraryObject( CLibraryObject& obj );
	Q_SLOT void		OnUIShowAddGameMenu();
	Q_SLOT void		OnUIAddGame( );
	UInt32			GetIconIdFromType( const Sb::CTypeInfo& info ) const;

    // Attributes   
	CIconTable				m_IconTypeTable;
	CIconTable				m_IconToolsTable;
    CViewGameTree*			m_Tree;
	QToolBar*				m_ToolBar;
	CObjectLibrary*			m_ObjectLibrary;
};

};

#endif