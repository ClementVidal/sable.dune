#ifndef _DUNE_EDITOR_VIEW_PROPERTYARRAYTYPE_
#define _DUNE_EDITOR_VIEW_PROPERTYARRAYTYPE_

#include <Sable\Core\Common\DataTypes.h>

#include <Dune\Editor\View\PropertyType.h>

#include <QtGui\QHBoxLayout>
#include <QtGui\QLabel>
#include <QtGui\QLineEdit>
#include <QtGui\QPushButton>
#include <QtGui\QSpinBox>
#include <QtGui\QDoubleSpinBox>
#include <QtGui\QComboBox>
#include <QtGui\QCheckBox>



namespace Dune
{

/**

*/
class CViewPropertyArray : public CViewPropertyBase
{

public:

	CViewPropertyArray( CApplication* app, CPropertyArray* p );
	~CViewPropertyArray();

    Void Initialize( QTreeWidget& tree, QTreeWidgetItem& root, QWidget* widgetToAdd = NULL );

protected:

    // DataTypes
    class CPropertyProxy : public QWidget
    {
    public:

        CPropertyProxy( CViewPropertyBase& p );

        CViewPropertyBase* Property;
        QPushButton* RemoveButton;
    };

    // Methods

    // Attributes
    QPushButton*    m_AddButton;
    QPushButton*    m_ClearButton;
	QLabel*			m_CountLabel;
	QLineEdit*		m_CountLineEdit;
};

};

#endif