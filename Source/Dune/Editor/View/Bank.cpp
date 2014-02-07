#include <Dune/Editor/View/Bank.h> 

#include <Dune/Model/Project/Project.h>
#include <Dune/Editor/Application/Application.h>

#include <Dune/Resources/Icons/BankView.xpm>

#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QFormLayout>

using namespace Dune;

CViewBank::CViewBank( CViewMainFrame& mainFrame ) :
    QDockWidget( "Bank Manager", &mainFrame ),
	CEventClient( mainFrame.GetApp() ),
	m_IconTable( (const char** ) BankView_xpm )
{
    QVBoxLayout* layout = new QVBoxLayout();
    QFormLayout* layoutA = new QFormLayout();
    QHBoxLayout* layoutB = new QHBoxLayout();

    m_BankCombo = new QComboBox();
    connect( m_BankCombo, SIGNAL( activated( int ) ), this, SLOT( OnUICurrentBankChanged( int ) ) );

    m_PathTextCtrl = new QLineEdit();
    m_PathTextCtrl->setText("");
    m_PathTextCtrl->setReadOnly(TRUE);
    
    m_BuidTextCtrl = new QLineEdit();
    m_BuidTextCtrl->setText("");
    m_BuidTextCtrl->setReadOnly(TRUE);

    layoutA->addRow( "Curren bank:", m_BankCombo );
    layoutA->addRow( "Path:", m_PathTextCtrl );
    layoutA->addRow( "Buid:", m_BuidTextCtrl );
    layoutA->setContentsMargins( 0,4,0,0 );

	QPushButton* renameBtn = new QPushButton( "Rename..." );
	connect( renameBtn, SIGNAL( clicked() ), this, SLOT( OnUIRenameBank() ) );
    QPushButton* addbank = new QPushButton( "+" );
    addbank->setMaximumWidth( 25 );
    addbank->setMinimumWidth( 25 );
    connect( addbank, SIGNAL( clicked() ), this, SLOT( OnUIAddBank() ) );
    QPushButton* removeBank = new QPushButton( "-" );
    removeBank->setMaximumWidth( 25 );
    removeBank->setMinimumWidth( 25 );
    connect( removeBank, SIGNAL( clicked() ), this, SLOT( OnUIRemoveBank() ) );

    layoutB->addWidget( addbank );
    layoutB->addWidget( removeBank );
    layoutB->addWidget( renameBtn );
    layoutB->setContentsMargins( 0,4,0,0 );

    layout->addLayout( layoutA );
    layout->addLayout( layoutB );
    layout->addStretch();
    layout->setContentsMargins( 0,4,0,0 );

    QWidget* mainWidget = new QWidget();
    mainWidget->setLayout( layout );
    setWidget( mainWidget );
    setEnabled( FALSE );
	setMinimumWidth( 240 );
}

CViewBank::~CViewBank()
{

}

Bool CViewBank::OnUICurrentBankChanged( int index )
{   
    QString str = m_BankCombo->itemData( index ).toString();

    CUuid uuid( str.toLocal8Bit().constData() );

    return GetApp().GetProjectCtrl().SetCurrentBank( uuid );
}

Void CViewBank::OnProjectDestroyed( )
{
    setEnabled( FALSE );
}

Void CViewBank::OnProjectCreated( CProject& p )
{
    m_BankCombo->clear();
    m_BankCombo->clearEditText();
    m_PathTextCtrl->setText( "" );
    m_BuidTextCtrl->setText( "" );

    setEnabled( TRUE );
    EVENT_CONNECT_SLOT( p.SignalBankAdded, m_SlotBankAdded, &CViewBank::OnBankAdded );
    EVENT_CONNECT_SLOT( p.SignalBankRemoved, m_SlotBankRemoved, &CViewBank::OnBankRemoved );
}

Void CViewBank::OnBankAdded( Dune::CBank& b )
{
	m_PathTextCtrl->setText( GetProject()->GetBankFilePath( b.GetUuid() ).GetBuffer() );
	m_PathTextCtrl->setToolTip( m_PathTextCtrl->text() );
    m_BankCombo->addItem( b.GetName(), QVariant( QString( b.GetUuid().ConvertToString().GetBuffer() ) ) );
    GetApp().GetProjectCtrl().SetCurrentBank( b.GetUuid() );
}

Void CViewBank::OnBankRemoved( Dune::CBank& b )
{
  //  m_BankCombo->removeItem( b.GetName() );
}

Void CViewBank::OnCurrentBankChanged( const CUuid& uuid )
{
    m_PathTextCtrl->setText( GetProject()->GetBankFilePath( uuid ).GetBuffer() );
    m_PathTextCtrl->setToolTip( m_PathTextCtrl->text() );

    m_BuidTextCtrl->setText( uuid.ConvertToString().GetBuffer() );
    m_BuidTextCtrl->setToolTip( m_BuidTextCtrl->text() );

    int index = m_BankCombo->findData( QVariant( QString( uuid.ConvertToString().GetBuffer() ) ) );
    if( index != -1 )
        m_BankCombo->setCurrentIndex( index );

}

Void CViewBank::OnProjectLoaded( CProject& p )
{

}

Bool CViewBank::OnUIRenameBank()
{
	bool ok = TRUE;
	QString text;

	QString str = m_BankCombo->itemData( m_BankCombo->currentIndex() ).toString();
	CUuid uuid( str.toLocal8Bit().constData() );

	CBank* bank = GetApp().GetProject()->GetBank( uuid );

	text = QInputDialog::getText(this, "Rename existing bank", "Bank name:", QLineEdit::Normal, bank->GetName(), &ok );

	bank->SetName( text.toLocal8Bit().constData() );

	m_BankCombo->setItemText( m_BankCombo->currentIndex(), text );

	return TRUE;
}

Bool CViewBank::OnUIAddBank()
{
    QMenu menu;
    QAction* action = NULL;
    action = menu.addAction( "Add new bank..." );
    connect( action, SIGNAL( triggered() ), this, SLOT( OnUIAddNewBank() ) );
    action = menu.addAction( "Add existing bank..." );
    connect( action, SIGNAL( triggered() ), this, SLOT( OnUIAddExistingBank() ) );

    menu.exec(QCursor::pos());
    return TRUE;
}

Bool CViewBank::OnUIRemoveBank()
{
    setEnabled( GetProject()->GetBankCount() );
    return TRUE;
}

Bool CViewBank::OnUIAddNewBank()
{
    QString dir = GetProject()->GetProjectDirectory();
	QFileDialog fd( this, "Select a bank file", "s:", "Dune bank (*.dbnk)" );
	fd.setDefaultSuffix( "dbnk" );
	if( fd.exec() == FALSE )
		return FALSE;

	QString fileName = fd.selectedFiles().first();

    if( fileName.length() )
    {
        bool ok = TRUE;
        QString text;

        text = QInputDialog::getText(this, "Add new Bank",
            "Bank name:", QLineEdit::Normal,
            "Default", &ok );

        if( ok )
        {
		    Dune::CBank* bank = GetProject()->AddNewBank( Sb::CFilePath( fileName.toLocal8Bit().constData() ), text.toLocal8Bit().constData() );
		    if( !bank )
                return FALSE;
        }
    }

    return TRUE;
}

Bool CViewBank::OnUIAddExistingBank()
{
    QString dir = GetProject()->GetProjectDirectory();
    QString fileName = QFileDialog::getOpenFileName(this, "Select a file", dir, "Dune bank (*.dbnk)", NULL, QFileDialog::DontUseNativeDialog );

    if( fileName.length() )
    {
        GetProject()->AddExistingBank( Sb::CFilePath( fileName.toLocal8Bit().constData() ) );
    }

    return TRUE;
}