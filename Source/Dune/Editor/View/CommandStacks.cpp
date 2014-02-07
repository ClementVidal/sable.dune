#include <Dune/Editor/View/CommandStacks.h> 

#include <QtGui/QHBoxLayout>

using namespace Dune;

CViewCommandStacks::CViewCommandStacks( CViewMainFrame& mainFrame ) :
    QDockWidget( "CommandStack", &mainFrame ),
	CEventClient( mainFrame.GetApp() )
{
	m_App = &mainFrame.GetApp();
	QHBoxLayout* layout = new QHBoxLayout( this );

	setMinimumHeight( 70 );
	m_Frame = new QWidget( this );
	m_RedoStack = new QListWidget( m_Frame  );
	m_UndoStack = new QListWidget( m_Frame );

    m_UndoStack->setSelectionMode( QAbstractItemView::NoSelection );
    m_UndoStack->setVerticalScrollMode( QAbstractItemView::ScrollPerPixel );
    m_RedoStack->setSelectionMode( QAbstractItemView::NoSelection );
    m_RedoStack->setVerticalScrollMode( QAbstractItemView::ScrollPerPixel );
	layout->setContentsMargins( 2, 2, 2, 2 );
	layout->addWidget( m_UndoStack );
	layout->addWidget( m_RedoStack );

	m_Frame->setLayout( layout );

	setWidget( m_Frame );

	EVENT_CONNECT_SLOT( m_App->GetCommandProcessor().SignalCmdStackChanged, m_CommandSlot, &CViewCommandStacks::OnCommandStackChanged );
}

CViewCommandStacks::~CViewCommandStacks()
{

}

Void CViewCommandStacks::OnCommandStackChanged()
{
	m_RedoStack->clear();
	m_UndoStack->clear();

	const CCommandProcessor::CmdStack& redoStack = m_App->GetCommandProcessor().GetRedoStack();
	const CCommandProcessor::CmdStack& undoStack = m_App->GetCommandProcessor().GetUndoStack();

	Int32 i=0;
	CCommandProcessor::CmdStack::Iterator it;
	ForEachItem( it, redoStack )
	{
		m_RedoStack->addItem( (*it)->GetTextualDesc().GetBuffer() );
	}

	ForEachItem( it, undoStack )
	{
		m_UndoStack->addItem( (*it)->GetTextualDesc().GetBuffer() );
	}

    m_UndoStack->scrollToBottom();
    m_RedoStack->scrollToBottom();
}

Void CViewCommandStacks::OnProjectDestroyed()
{
	m_UndoStack->clear();
	m_RedoStack->clear();
}
