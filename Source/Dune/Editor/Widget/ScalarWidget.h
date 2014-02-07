#ifndef _DUNE_EDITOR_WIDGET_SCALARWIDGET_
#define _DUNE_EDITOR_WIDGET_SCALARWIDGET_

#include <Sable/Core/Common/DataTypes.h>

#include <QtGUI/QLineEdit>

namespace Dune
{

/**
Scalar control
*/
class CScalarWidget : public QWidget
{
    
    Q_OBJECT

public:

    /** @name DataTypes*/
    //@{
	enum EType
	{
		nType_Float = 0,
		nType_Int,
		nType_Count,
		nType_None = -1
	};
    //@}

    /** @name Constructor/Destructor*/
    //@{
	CScalarWidget( QWidget& parent, Float32 v );
	CScalarWidget( QWidget& parent, Int32 v );
    virtual ~CScalarWidget();
    //@}

    /** @name Accesors*/
    //@{
	Void	SetValue( Float32 v, Bool trigerSignal = TRUE );
	Void	SetValue( Int32 v, Bool trigerSignal = TRUE );
	Float32 GetValueAsFloat() const;
	Int32	GetValueAsInt() const;

	Void	SetDecimals( Int32 decimal );
	Void	SetStep( Float32 step );
	Void	SetRange( Sb::CFloat32Range& range );
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
	Q_SLOT void OnUITextEdited( const QString& text );
	Q_SLOT void OnUIDecrement();
	Q_SLOT void OnUIIncrement();

	bool		eventFilter(QObject *obj, QEvent *event);
	Void		UpdateLineEdit();

	Void		ResetToDefault( );
	Void		Initialize();
	Void		ClampValue();
	Void		CreateIcons();

    // Attributes
	union
	{
		Float32	m_FloatValue;
		Int32	m_IntValue;
	};
	union
	{
		Float32	m_FloatDefaultValue;
		Int32	m_IntDefaultValue;
	};

	EType					m_Type;
	QLineEdit*				m_LineEdit;
	QPushButton*			m_IncButton;
	QPushButton*			m_DecButton;
	QIntValidator*			m_IntValidator;
	QDoubleValidator*		m_FloatValidator;
	Float32					m_Step;
	Float32					m_Decimals;
	Sb::CFloat32Range		m_FloatRange;

	static QIcon			m_IncIcon;
	static QIcon			m_DecIcon;
};

};

#endif