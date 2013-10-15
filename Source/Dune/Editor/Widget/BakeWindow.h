#ifndef _DUNE_EDITOR_WIDGET_BAKEWINDOW_
#define _DUNE_EDITOR_WIDGET_BAKEWINDOW_

#include <Sable\Core\Common\DataTypes.h>
#include <Dune\Editor\Application\Application.h>
#include <Dune\Bake\Context\Export.h>
#include <Dune\Bake\Configuration.h>
#include <Dune\Bake\RequestDispatcher.h>

#include <QtGui\QProgressBar>
#include <QtGui\QDialog>
#include <QtGui\QPushButton>
#include <QtGui\QListWidget>
#include <QtGui\QCheckBox>
#include <QtGui\QLineEdit>



namespace Dune
{

class CProject;

/**
Dialog use to start baking operations;
*/
class CBakeWindow : public QDialog
{
    
	Q_OBJECT

public:

    /** @name DataTypes*/
    //@{
    //@}

    /** @name Constructor/Destructor*/
    //@{
    CBakeWindow( CApplication& app, CProject& prj );
    virtual ~CBakeWindow();
    //@}

    /** @name Accesors*/
    //@{
    //@}

    /** @name Manipulator*/
    //@{
    //@}

private:

    // Types    

    // Methods
    Q_SLOT Bool	    OnUIBake( );
    Q_SLOT Bool     OnUIClose( );
	Q_SLOT Bool	    OnUISelectOutputPath( );
	Q_SLOT Bool     OnTimerTimeout();
    Q_SLOT Bool     OutputLogMsg( int type, const QString& msg );
    Q_SIGNAL void   SignalOutputLogMsg( int type, const QString& msg );
    Bool            ExtractOutputPath();
    Void            OnLogMsg( CBakeContext::EType type, String msg );

    // Attributes
	QCheckBox*						m_OptimizeModels;
    QProgressBar*					m_ProgressGauge;
    QListWidget *					m_LogWindow;
    QPushButton*					m_BakeCloseBtn;
    QPushButton*					m_CancelBtn;
    QLineEdit *						m_OutputPath;
    CProject*						m_Project;
    Sb::CRef<CBakeContextExport>	m_BakeContext;
    SBakeConfiguration				m_BakeConfig;
    CBakeRequestDispatcher			m_BakeDispatcher;
    Bool							m_BakeStarted;
    QTimer							m_Timer;

    Sb::CEventSlot< CBakeWindow, CBakeContext::EType, String>   m_SlotLog;

};

};

#endif