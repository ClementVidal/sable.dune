#ifndef _DUNE_EDITOR_VIEW_OBJECTPICKER_
#define _DUNE_EDITOR_VIEW_OBJECTPICKER_

#include <Sable\Core\Common\DataTypes.h>
#include <Dune\Editor\Application\Application.h>

#include <QtGui\QComboBox>
#include <QtGui\QTreeWidget>



namespace Dune
{

/**
Asset picker
*/
class CWidgetObjectPicker : public QDialog
{

	Q_OBJECT 

public:

    /** @name DataTypes*/
    //@{
    struct SConfig
    {
        Sb::CArray<const Sb::CTypeInfo*> Filter;
    };
    //@}

    /** @name Constructor/Destructor*/
    //@{
    CWidgetObjectPicker( CApplication& app, const CProject& prj, const SConfig* config );
    virtual ~CWidgetObjectPicker();
    //@}

    /** @name Accesors*/
    //@{
    const CHandle& GetAsset() const;
    CApplication& GetApp() ;
    //@}

    /** @name Manipulator*/
    //@{
    //@}

private:

    // Types
    struct CTreeItemData : public QTreeWidgetItem
    {
        CTreeItemData( const CHandle& elem );

        CHandle Asset;
    };

    // Methods
	QSize	sizeHint () const;
    Void PopulateTree( String typeName );
    Void PopulateFilter( const SConfig* config );
	Q_SLOT Bool OnUISelectionChanged( );
	Q_SLOT Bool OnUIItemDoubleClicked( QTreeWidgetItem* i , int c );
   	Q_SLOT Bool OnUIOk();
	Q_SLOT Bool OnUICancel();
    Q_SLOT Bool OnUIFilterChange( const QString & text );

    // Attributes
	QComboBox*              m_AssetTypeCombo;
	QToolBar*				m_ToolBar;
	QPushButton*			m_OkButton;
	QPushButton*			m_CancelButton;
	QTreeWidget*            m_TreeCtrl;
    CApplication*       m_App;
    CHandle                 m_Asset;
    const CProject*         m_Project;
};

};

#endif