#ifndef _DUNE_EDITOR_VIEW_PROPERTYTYPE_
#define _DUNE_EDITOR_VIEW_PROPERTYTYPE_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/Math/Vector2f.h>
#include <Sable/Core/Math/Vector3f.h>
#include <Sable/Core/String/String.h>
#include <Dune/Model/Asset/Asset.h>
#include <Dune/Model/Property/Property.h> 
#include <Dune/Model/Property/PropertyArray.h> 
#include <Dune/Editor/Application/Application.h>
#include <Dune/Editor/Widget/ScalarWidget.h>
#include <Dune/Editor/Widget/VectorWidget.h>
#include <Dune/Editor/Widget/ColorWidget.h>
#include <Dune/Editor/Widget/MatrixWidget.h>

#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QComboBox>
#include <QtGui/QCheckBox>



namespace Dune
{

/**

*/
class CViewPropertyBase : public QWidget
{

	Q_OBJECT

public:

    // Constructor/Destructor
	CViewPropertyBase( CApplication* app, CPropertyInterface* p );
	~CViewPropertyBase();

	QTreeWidgetItem*		TreeItem;

    // Manipulators
    virtual  Void Initialize( QTreeWidget& tree, QTreeWidgetItem& root, QWidget* widgetToAdd = NULL );

    static CViewPropertyBase* CreateViewFromModel( CApplication* app, const CPropertyInterface& p );

protected:

	// Methods
	virtual Void	        OnModelPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue );
    Void                    Enable( Bool onOff );
	Q_SLOT virtual Bool     OnUILineEditChanged();

	// Attributes
	QHBoxLayout*		                    m_Layout;
	QLineEdit*			                    m_ValueLineEdit;
	CApplication*							m_App;
	CPropertyInterface*	                    m_Property;
	Sb::CEventSlot< CViewPropertyBase, const CPropertyInterface&, const Sb::CString& >	m_SlotPropertyChanged;
};

/**
String Property Wdg
*/
class CViewPropertyString : public CViewPropertyBase
{

	Q_OBJECT

public:

    CViewPropertyString( CApplication* app = NULL, CPropertyString* value = NULL );
    ~CViewPropertyString();

protected:

	Q_SLOT Bool OnUIBtnClicked();
	QPushButton* m_Button;
};

/**
Float32 Property Wdg
*/
class CViewPropertyFloat32 : public CViewPropertyBase
{

    Q_OBJECT 

public:

	CViewPropertyFloat32( CApplication* app = NULL, CPropertyFloat32* value = NULL );
    ~CViewPropertyFloat32();

private:

    Q_SLOT void OnUIValueChanged();
	CScalarWidget*			m_SpinBox;
};

/**
UInt32 Property Wdg
*/
class CViewPropertyInt32 : public CViewPropertyBase
{

    Q_OBJECT 

public:

    CViewPropertyInt32( CApplication* app = NULL, CPropertyInt32* value = NULL );
    ~CViewPropertyInt32();

private:

    Q_SLOT void OnUIValueChanged( );
	CScalarWidget*			m_SpinBox;
};

/**
Vector2 Property Wdg
*/
class CViewPropertyVector2i : public CViewPropertyBase
{

public:

	CViewPropertyVector2i( CApplication* app = NULL, CPropertyVector2i* value = NULL );
    ~CViewPropertyVector2i();
};

/**
Vector2 Property Wdg
*/
class CViewPropertyVector2f : public CViewPropertyBase
{

public:

	CViewPropertyVector2f( CApplication* app = NULL, CPropertyVector2f* value = NULL );
    ~CViewPropertyVector2f();
};

/**
Vector3 Property Wdg
*/
class CViewPropertyVector3f : public CViewPropertyBase
{

    Q_OBJECT 

public:

	CViewPropertyVector3f( CApplication* app = NULL, CPropertyVector3f* value = NULL );
    ~CViewPropertyVector3f();

private:

    Q_SLOT Bool         OnUIValueChanged();
    Void				OnModelPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue );
	CVector3Widget*	    m_Widget;
};

/**
Vector4 Property Wdg
*/
class CViewPropertyVector4f : public CViewPropertyBase
{

    Q_OBJECT 

public:

	CViewPropertyVector4f( CApplication* app = NULL, CPropertyVector4f* value = NULL );
    ~CViewPropertyVector4f();

private:

    Q_SLOT Bool         OnUIValueChanged();
    Void				OnModelPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue );
	CVector4Widget*	    m_Widget;
};

/**
Matrix4x4 Property Wdg
*/
class CViewPropertyMatrix4x4 : public CViewPropertyBase
{

    Q_OBJECT 

public:

	CViewPropertyMatrix4x4( CApplication* app = NULL, CPropertyMatrix4x4* value = NULL );
    ~CViewPropertyMatrix4x4();

private:

    Q_SLOT Bool         OnUIValueChanged();
    Void	OnModelPropertyChanged( const CPropertyInterface& p, const Sb::CString& oldValue  );
	CMatrix4x4Widget*	    m_Widget;
};

/**
Sb::CFilePath Property Wdg
*/
class CViewPropertyFilePath : public CViewPropertyBase
{

	Q_OBJECT 

public:

	CViewPropertyFilePath( CApplication* app = NULL, CPropertyFilePath* value = NULL );
	~CViewPropertyFilePath();

private:

	Q_SLOT Bool		OnUIBtnClicked();
	QPushButton*	m_Button;
};

/**
CAsset Property Wdg
*/
class CViewPropertyHandle : public CViewPropertyBase
{

	Q_OBJECT 

public:

    CViewPropertyHandle( CApplication* app = NULL, CPropertyHandle* value = NULL );
    ~CViewPropertyHandle();

private:

	Q_SLOT Bool		OnUIBtnClicked();
	Q_SLOT Bool		OnUIBindAsset( );
	Q_SLOT Bool		OnUIUnbindAsset( );
	Q_SLOT Bool		OnUIEditAsset( );
	Q_SLOT Bool		OnUIAddAsset();

	QPushButton*	m_Button;
    QLineEdit*      m_DisplayName;
};

/**
Color Property Wdg
*/
class CViewPropertyColor : public CViewPropertyBase
{

	Q_OBJECT

public:

    CViewPropertyColor( CApplication* app = NULL, CPropertyColor* value = NULL );
    ~CViewPropertyColor();

private:

	Q_SLOT			Bool OnUIValueChanged();
	CColorWidget*	m_Widget;
};

/**
bool Property Wdg
*/
class CViewPropertyBool : public CViewPropertyBase
{
	Q_OBJECT

public:

    CViewPropertyBool( CApplication* app = NULL, CPropertyBool* value = NULL );
    ~CViewPropertyBool();

private:

	Q_SLOT Bool OnUICheckBoxClicked( int state );
	QCheckBox*	m_CheckBox;
};

/**
Enum Property Wdg
*/
class CViewPropertyEnum : public CViewPropertyBase
{

	Q_OBJECT

public:

    CViewPropertyEnum( CApplication* app = NULL, CPropertyEnum* value = NULL );
    ~CViewPropertyEnum();

private:

	Q_SLOT Bool OnUIComboBoxChanged( int index );
	QComboBox*	m_Combo;
};


};

#endif