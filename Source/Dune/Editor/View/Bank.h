#ifndef _DUNE_EDITOR_VIEW_BANK_
#define _DUNE_EDITOR_VIEW_BANK_

#include <Sable\Core\Common\DataTypes.h>

#include <Dune\Editor\Ctrl\EventClient.h>
#include <Dune\Editor\Utilities\IconTable.h>
#include <Dune\Model\Bank\Bank.h>

#include <QtGui\QDockWidget>

namespace Dune
{

class CControllerBank;
class CApplication;
class CProject;
class CBank;

/**
Bank view
*/
class CViewBank : public QDockWidget, public CEventClient
{

	Q_OBJECT 

public:

    /** @name Constructor/Destructor*/
    //@{
    CViewBank( CViewMainFrame& mainFrame );
    virtual ~CViewBank();
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
	Q_SLOT Bool     OnUIRenameBank();
    Q_SLOT Bool     OnUIAddBank();
    Q_SLOT Bool     OnUIRemoveBank();
    Q_SLOT Bool     OnUIAddNewBank();
    Q_SLOT Bool     OnUIAddExistingBank();
    Q_SLOT Bool     OnUICurrentBankChanged( int index);
	Void			OnProjectLoaded( CProject& p );
	Void			OnProjectCreated( CProject& p );
	Void			OnProjectDestroyed( );
    Void            OnBankAdded( CBank& b );
    Void            OnBankRemoved( CBank& b );
    Void            OnCurrentBankChanged( const CUuid& uuid );

    // Attributes   
    Sb::CEventSlot< CViewBank, CBank& >     m_SlotBankAdded;
    Sb::CEventSlot< CViewBank, CBank& >     m_SlotBankRemoved;

	CIconTable			                    m_IconTable;
    QComboBox*                              m_BankCombo;
    QLineEdit*                              m_PathTextCtrl;
    QLineEdit*                              m_BuidTextCtrl;
};

};

#endif