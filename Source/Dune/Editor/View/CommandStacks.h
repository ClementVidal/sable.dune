#ifndef _DUNE_EDITOR_VIEW_COMMANDSTACKS_
#define _DUNE_EDITOR_VIEW_COMMANDSTACKS_

#include <Sable\Core\Common\DataTypes.h>
#include <Sable\Core\Event\Header.h>

#include <Dune\Editor\Ctrl\EventClient.h>

#include <QtGui\QDockWidget>
#include <QtGui\QListWidget>



namespace Dune
{

/**
Command stack view
*/
class CViewCommandStacks : public QDockWidget, CEventClient
{

public:

    // Data Types

    /** @name Constructor/Destructor*/
    //@{
    CViewCommandStacks( CViewMainFrame& mainFrame );
    virtual ~CViewCommandStacks();
    //@}

    /** @name Accesors*/
    //@{
	//@}

    /** @name Manipulator*/
    //@{
    //@}

private:

    // friendship

    // Types

	// Methods
	Void	OnCommandStackChanged();
	Void	OnProjectDestroyed();

	// Attributes   
    CApplication*		m_App;
	QWidget*					m_Frame;
	QListWidget*			m_UndoStack;
	QListWidget*			m_RedoStack;
	Sb::CEventSlot< CViewCommandStacks >	m_CommandSlot;
};

};

#endif