#ifndef _DUNE_EDITOR_WIDGET_VECTORWIDGET_
#define _DUNE_EDITOR_WIDGET_VECTORWIDGET_

#include <Sable\Core\Common\DataTypes.h>

#include <Dune\Editor\Widget\ScalarWidget.h> 

namespace Dune
{

/**
Vector control
*/
class CVector3Widget : public QWidget
{
    
    Q_OBJECT

public:

    /** @name DataTypes*/
    //@{
    //@}

    /** @name Constructor/Destructor*/
    //@{
    CVector3Widget( QWidget& parent, const Sb::CVector3f& value  );
    virtual ~CVector3Widget();
    //@}

    /** @name Accesors*/
    //@{
	Void SetValue( const Sb::CVector3f& color, Bool trigerSignal = TRUE );
	const Sb::CVector3f& GetValue() const;
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
	Q_SLOT Bool OnUIValueChanged( );

    // Attributes
	Sb::CVector3f			m_Value;
	CScalarWidget*			m_X;
	CScalarWidget*			m_Y;
	CScalarWidget*			m_Z;
	Bool					m_CanTrigerSignal;
};


/**
Vector control
*/
class CVector4Widget : public QWidget
{
    
    Q_OBJECT

public:

    /** @name DataTypes*/
    //@{
    //@}

    /** @name Constructor/Destructor*/
    //@{
    CVector4Widget( QWidget& parent, const Sb::CVector4f& value  );
    virtual ~CVector4Widget();
    //@}

    /** @name Accesors*/
    //@{
	Void SetValue( const Sb::CVector4f& vector, Bool trigerSignal = TRUE );
	const Sb::CVector4f& GetValue() const;
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
	Q_SLOT Bool OnUIValueChanged( );

	// Attributes
	Sb::CVector4f			m_Value;
	CScalarWidget*			m_X;
	CScalarWidget*			m_Y;
	CScalarWidget*			m_Z;
	CScalarWidget*			m_W;
	Bool					m_CanTrigerSignal;
};

};

#endif