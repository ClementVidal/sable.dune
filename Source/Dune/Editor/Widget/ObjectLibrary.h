#ifndef _DUNE_EDITOR_WIDGET_OBJECTLIBRARY_
#define _DUNE_EDITOR_WIDGET_OBJECTLIBRARY_

#include <Sable\Core\Common\DataTypes.h>
#include <Dune\Editor\Application\Application.h>
#include <Dune\Editor\Ctrl\EventClient.h>

#include <QtGui\QToolBar>
#include <QtGui\QTreeWidget>
#include <QtGui\QTreeWidgetItem>
#include <QtGui\QDockWidget>
#include <QtGui\QLineEdit>
#include <QtGui\QTextEdit>
#include <QtCore\QFileInfo>
#include <QtCore\QSignalMapper>
#include <QtCore\QFileSystemWatcher>

namespace Dune
{

class CApplication;
class CBankObject;

/**
Material custom edit dialog
*/
class CObjectLibrary : public QDockWidget, public CEventClient
{
    
	Q_OBJECT

public:

    /** @name DataTypes*/
    //@{
    //@}

    /** @name Constructor/Destructor*/
    //@{
	CObjectLibrary( CApplication& app, String name, const Sb::CTypeInfo& objType );
    ~CObjectLibrary();
    //@}

    /** @name Accesors*/
    //@{
    //@}

    /** @name Manipulator*/
    //@{
    //@}

	/** @name Signals*/
	//@{
	Q_SIGNAL void	OnObjectSelected( CLibraryObject& obj );
	//@}

private:

    // Types
	struct STreeItem : public QTreeWidgetItem
	{
		Sb::CFilePath	ItemPath;
		QTextEdit*		TextEditText;
	};

	// Methods
	Q_SLOT void OnUIAddObj( int id );
	Q_SLOT void OnUISaveObj( int id );
	Q_SLOT void OnPathContentChanged( const QString& dir );

	Void	AppendItem( const QFileInfo& file );
	Void	OnProjectLoaded( CProject& r );
	Void	Populate( CProject& p );

    // Attributes 
	QSignalMapper			m_SaveObjSignalMapper;	
	QSignalMapper			m_AddObjSignalMapper;	
	QToolBar*				m_ToolBar;
	QTreeWidget*			m_List;
	QFileSystemWatcher*		m_FileWatcher;
	const Sb::CTypeInfo*	m_ObjectType;

};

};

#endif