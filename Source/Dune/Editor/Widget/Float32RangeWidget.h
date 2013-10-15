#ifndef _DUNE_EDITOR_WIDGET_FLOAT32RANGEWIDGET_
#define _DUNE_EDITOR_WIDGET_FLOAT32RANGEWIDGET_

#include <Sable\Core\Common\DataTypes.h>

#include <QtGui\QDoubleSpinBox> 



namespace Dune
{

/**
Input widget used to spécify a range of float 32.
Emit a OnValueChanged each time the value is modified
*/
class CFloat32RangeWidget : public QWidget
{

    Q_OBJECT

public:

    /** @name DataTypes*/
    //@{
    //@}

    /** @name Constructor/Destructor*/
    //@{
    CFloat32RangeWidget( QWidget& parent, const Sb::CFloat32Range& value );
    virtual ~CFloat32RangeWidget();
    //@}

    /** @name Accesors*/
    //@{
	const Sb::CFloat32Range&    GetValue() const;
	Void	                SetValue( const Sb::CFloat32Range& range );
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
	QDoubleSpinBox*		m_Min;
	QDoubleSpinBox*		m_Max;
	Sb::CFloat32Range	    m_Value;
	Bool				m_SendOnValueChangedSignal;
};

};

#endif