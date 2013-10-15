#ifndef _DUNE_EDITOR_VIEW_UI_
#define _DUNE_EDITOR_VIEW_UI_

#include <Sable\Core\Common\DataTypes.h>

#include <Dune\Editor\Ctrl\EventClient.h>
#include <Dune\Editor\Utilities\IconTable.h>
#include <Dune\Editor\View\UITree.h>

#include <QtGui\QDockWidget>

namespace Dune
{

/**
Bank view
*/
class CViewUI : public QDockWidget, public CEventClient
{

	Q_OBJECT 

public:

    /** @name Constructor/Destructor*/
    //@{
    CViewUI( CViewMainFrame& mainFrame );
    ~CViewUI();
    //@}

    /** @name Accesors*/
	//@{
	QIcon			GetIcon( const Sb::CTypeInfo& type ) const;
	Bool            HasIcon( const Sb::CTypeInfo& type ) const;
	//@}

    /** @name Manipulator*/
	//@{
	Void			ConfigureAddMenu( QMenu& subMenu );
    //@}

private:

    // friendship

    // Types

	// Methods
	Q_SLOT void		OnUIAddElement( );
	Q_SLOT void		OnUIShowAddMenu();
	Q_SLOT void		OnUISelectLibraryObject( CLibraryObject& obj );
	UInt32			GetIconIdFromType( const Sb::CTypeInfo& info ) const;

    // Attributes   
	CObjectLibrary*		m_ObjectLibrary;
	CViewUITree*		m_Tree;
	QToolBar*			m_ToolBar;
	CIconTable			m_IconToolsTable;
	CIconTable			m_IconTypeTable;
};

};

#endif