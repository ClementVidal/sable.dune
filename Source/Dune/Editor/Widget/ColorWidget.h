#ifndef _DUNE_EDITOR_WIDGET_COLORWIDGET_
#define _DUNE_EDITOR_WIDGET_COLORWIDGET_

#include <Sable\Core\Common\DataTypes.h>




namespace Dune
{

/**
Color picker widget
*/
class CColorWidget : public QWidget
{
    
	Q_OBJECT

public:

    /** @name DataTypes*/
    //@{
    //@}

    /** @name Constructor/Destructor*/
    //@{
    CColorWidget( QWidget& parent, const Sb::CColor& col );
    virtual ~CColorWidget();
    //@}

    /** @name Accesors*/
    //@{
	const Sb::CColor& GetValue() const;
	Void SetValue( const Sb::CColor& col );
	Void ShowLineEdit( Bool onOff );
	//@}

    /** @name Manipulator*/
    //@{
    //@}

	/** @name Signals*/
	//@{
	Q_SIGNAL Bool OnValueChanged();
	//@}

private:

    // Types

    // Methods
	Q_SLOT Bool		OnUILineEditChanged();
	Q_SLOT Bool		OnUIBtnClicked();

    // Attributes
	Sb::CColor					m_Value;
	QLineEdit*				m_LineEdit;
	QFrame*					m_ColorFrame;
	QPushButton*			m_ColorPickerBtn;
};

};

#endif