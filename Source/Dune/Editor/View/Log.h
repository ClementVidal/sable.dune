#ifndef _DUNE_EDITOR_VIEW_LOG_
#define _DUNE_EDITOR_VIEW_LOG_

#include <Sable/Core/Common/DataTypes.h>

#include <Dune/Editor/Ctrl/EventClient.h>

#include <QtGui/QListWidget>



namespace Dune
{

class CApplication;

/**
UI representation of a project
*/
class CViewLog : public QDockWidget, CEventClient
{

	Q_OBJECT

public:

    // Data Types

    /** @name Constructor/Destructor*/
    //@{
    CViewLog( CViewMainFrame& mainFrame );
    virtual ~CViewLog();
    //@}

    /** @name Manipulator*/
    //@{
    //@}

private:

    // Types

    // Methods
	Void			OnLog( UInt32 lvl, String header, String msg );
	Void			contextMenuEvent( QContextMenuEvent* event );
	Q_SLOT void		OnUIClear();

    // Attributes
	QListWidget*	m_TextOutput;
	Sb::CEventSlot< CViewLog, UInt32, String, String >	m_SlotLog;

};

};

#endif