#ifndef _DUNE_EDITOR_WIDGET_VECTORRANGEWIDGET_
#define _DUNE_EDITOR_WIDGET_VECTORRANGEWIDGET_

#include <Sable\Core\Common\DataTypes.h>

#include <Dune\Editor\Widget\VectorWidget.h> 



namespace Dune
{

/**
Input widget used to spécify a range of float 32.
Emit a OnValueChanged each time the value is modified
*/
class CVector3RangeWidget : public QWidget
{

    Q_OBJECT

public:

    /** @name DataTypes*/
    //@{
    //@}

    /** @name Constructor/Destructor*/
    //@{
    CVector3RangeWidget( QWidget& parent, const Sb::CVector3Range& value );
    virtual ~CVector3RangeWidget();
    //@}

    /** @name Accesors*/
    //@{
	const Sb::CVector3Range&    GetValue() const;
	Void	                SetValue( const Sb::CVector3Range& range );
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
    Q_SLOT  Bool  OnUIMinValueChanged( );
	Q_SLOT  Bool  OnUIMaxValueChanged( );

    // Attributes
	CVector3Widget*		m_Min;
	CVector3Widget*		m_Max;
	Sb::CVector3Range	    m_Value;
	Bool				m_SendOnValueChangedSignal;
};



class CVector4RangeWidget : public QWidget
{

	Q_OBJECT

public:

	/** @name DataTypes*/
	//@{
	//@}

	/** @name Constructor/Destructor*/
	//@{
	CVector4RangeWidget( QWidget& parent, const Sb::CVector4Range& value );
	virtual ~CVector4RangeWidget();
	//@}

	/** @name Accesors*/
	//@{
	const Sb::CVector4Range&    GetValue() const;
	Void	                SetValue( const Sb::CVector4Range& range );
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
	Q_SLOT  Bool  OnUIMinValueChanged( );
	Q_SLOT  Bool  OnUIMaxValueChanged( );

	// Attributes
	CVector4Widget*		m_Min;
	CVector4Widget*		m_Max;
	Sb::CVector4Range	    m_Value;
	Bool				m_SendOnValueChangedSignal;
};

};

#endif