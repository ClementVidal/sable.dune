#ifndef _DUNE_EDITOR_WIDGET_COLORRANGEWIDGET_
#define _DUNE_EDITOR_WIDGET_COLORRANGEWIDGET_

#include <Sable\Core\Common\DataTypes.h>

#include <Dune\Editor\Widget\ColorWidget.h> 



namespace Dune
{

/**
Range control
*/
class CColorRangeWidget : public QFrame
{

	Q_OBJECT

public:

    /** @name DataTypes*/
    //@{
    //@}

    /** @name Constructor/Destructor*/
    //@{
    CColorRangeWidget( QWidget& parent, const Sb::CColorRange& value );
    virtual ~CColorRangeWidget();
    //@}

    /** @name Accesors*/
    //@{
	Void SetValue( const Sb::CColorRange& range );
	const Sb::CColorRange& GetValue() const;
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
	Q_SLOT  Bool  OnUIMinValueChanged( double d );
	Q_SLOT  Bool  OnUIMaxValueChanged( double d );

    // Attributes
	CColorWidget*		m_Min;
	CColorWidget*		m_Max;
	Sb::CColorRange			m_Value;
	Bool				m_SendOnValueChangedSignal;
};

};

#endif