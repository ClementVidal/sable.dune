#ifndef _DUNE_EDITOR_WIDGET_MATRIXWIDGET_
#define _DUNE_EDITOR_WIDGET_MATRIXWIDGET_

#include <Sable\Core\Common\DataTypes.h>

namespace Dune
{

/**
Vector control
*/
class CMatrix4x4Widget : public QWidget
{
    
    Q_OBJECT

public:

    /** @name DataTypes*/
    //@{
    //@}

    /** @name Constructor/Destructor*/
    //@{
    CMatrix4x4Widget( QWidget& parent, const Sb::CMatrix4x4f& value  );
    virtual ~CMatrix4x4Widget();
    //@}

    /** @name Accesors*/
    //@{
	Void SetValue( const Sb::CMatrix4x4f& color, Bool trigerSignal = TRUE );
	const Sb::CMatrix4x4f& GetValue() const;
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
	Sb::CMatrix4x4f		m_Value;
	QLineEdit*		        m_LineEdits[4][4];
	Bool					m_CanTrigerSignal;
};


};

#endif