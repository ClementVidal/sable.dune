#include <Dune/Editor/View/Log.h> 

using namespace Dune;

CViewLog::CViewLog( CViewMainFrame& mainFrame ) :
    QDockWidget( "Log", &mainFrame ),
	CEventClient( mainFrame.GetApp() )
{
	QHBoxLayout* layout = new QHBoxLayout( this );

	setMinimumHeight( 70 );
	QWidget* frame = new QWidget( this );
	m_TextOutput = new QListWidget( );
    m_TextOutput->setVerticalScrollMode( QAbstractItemView::ScrollPerPixel );
    m_TextOutput->setSelectionMode( QAbstractItemView::NoSelection );

	layout->setContentsMargins( 2, 2, 2, 2 );
	layout->addWidget( m_TextOutput );
	frame->setLayout( layout );
	setWidget( frame );

	setContextMenuPolicy( Qt::DefaultContextMenu );

	EVENT_CONNECT_SLOT( mainFrame.GetApp().GetSableApplication().GetLogHandler().SignalLog, m_SlotLog, &CViewLog::OnLog );
}

CViewLog::~CViewLog()
{
}

Void CViewLog::contextMenuEvent( QContextMenuEvent * event )
{
	QMenu menu( this );

	QAction* action = menu.addAction( "Clear log"  );
	connect( action, SIGNAL( triggered() ), this, SLOT( OnUIClear() ) );

	menu.exec( event->globalPos()  );
}

void CViewLog::OnUIClear()
{
	m_TextOutput->clear();
}

Void CViewLog::OnLog( UInt32 lvl, String header, String msg )
{
	Char out[1024];

	if( header )
		Sb::StringSetFormattedText(out, 1024, "%s - %s", header, msg );
	else
		Sb::StringSetFormattedText(out, 1024, "%s", msg );

	QListWidgetItem* item = new QListWidgetItem( out );

	if( lvl == Sb::CLogger::nLevel_Info )
	{
        item->setTextColor( QColor( 40,40,40 ) );
	}
	else if( lvl == Sb::CLogger::nLevel_Warning )
	{
		item->setTextColor( QColor( 255, 127, 0 ) );
	}
	else if( lvl == Sb::CLogger::nLevel_Error )
	{
		item->setTextColor( QColor( 255, 0, 0 ) );
	}
	
	m_TextOutput->addItem( item );
	m_TextOutput->scrollToItem( item );
}
