#include <Dune\Editor\Widget\TextEditor.h> 

using namespace Dune;

CTextEditor::CTextEditor( CApplication& app ) :
    QDialog( & app.GetMainFrame(), Qt::Tool  )
{
	setWindowTitle( "Text Editor" );
    m_App = &app;

    QVBoxLayout*	layout = new QVBoxLayout( this );
    QHBoxLayout*	bottomLayout = new QHBoxLayout( this );

	bottomLayout->addStretch( 1 );

	QPushButton* okBtn = new QPushButton( "OK" );
	connect( okBtn, SIGNAL( clicked() ), this, SLOT( OnUIOk() ) );
	bottomLayout->addWidget( okBtn );

	QPushButton* cancelBtn = new QPushButton( "Cancel" );
	connect( cancelBtn, SIGNAL( clicked() ), this, SLOT( OnUICancel() ) );
	bottomLayout->addWidget( cancelBtn );

	m_TextEdit = new QTextEdit();

	layout->addWidget( m_TextEdit );
	layout->addLayout( bottomLayout );

	setLayout( layout );

	layout->setContentsMargins( 0,0,0,0 );

	resize( 800, 400 );
}

CTextEditor::~CTextEditor()
{

}

CApplication& CTextEditor::GetApp()
{
    return *m_App;
}

Void CTextEditor::SetText( const Sb::CString& s )
{
	m_TextEdit->setPlainText( s.GetBuffer() );
}

Sb::CString CTextEditor::GetText() const
{
	QString q = m_TextEdit->toPlainText();

	return q.toLocal8Bit().constData();
}

void CTextEditor::OnUIOk()
{
	done( 1 );
}

void CTextEditor::OnUICancel()
{
	done( 0 );
}