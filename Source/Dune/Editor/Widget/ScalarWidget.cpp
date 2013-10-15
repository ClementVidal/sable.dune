#include <Dune\Editor\Widget\ScalarWidget.h> 

using namespace Dune;

QIcon CScalarWidget::m_IncIcon;
QIcon CScalarWidget::m_DecIcon;

CScalarWidget::CScalarWidget( QWidget& parent, Float32 value  ) :
    QWidget( &parent )
{
	m_FloatDefaultValue = value;
	m_FloatValue = value;
	m_Type = nType_Float;
	Initialize();
}

CScalarWidget::CScalarWidget( QWidget& parent, Int32 value  ) :
	QWidget( &parent )
{
	m_IntDefaultValue = value;
	m_IntValue = value;
	m_Type = nType_Int;
	Initialize();
}

Void CScalarWidget::SetDecimals( Int32 decimal )
{
	m_Decimals = decimal;
}

Void CScalarWidget::SetStep( Float32 step )
{
	m_Step = step;
}

Void CScalarWidget::SetRange( Sb::CFloat32Range& range )
{
	m_FloatRange = range;
}

Void CScalarWidget::Initialize()
{
	m_FloatRange = Sb::CFloat32Range( FLOAT32_MIN_VAL, FLOAT32_MAX_VAL );
	m_Step = 1.0f;
	m_Decimals = 5;

	QHBoxLayout* hLayout = new QHBoxLayout();
	hLayout->setSpacing( 0 );
	hLayout->setContentsMargins( 0,0,0,0 );

	QVBoxLayout* vLayout = new QVBoxLayout();
	vLayout->setSpacing( 0 );
	vLayout->setContentsMargins( 0,0,0,0 );

	m_LineEdit = new QLineEdit();
	m_IncButton = new QPushButton( );
	m_DecButton = new QPushButton( );

	CreateIcons();
		
	m_IncButton->setIcon( m_IncIcon );
	m_DecButton->setIcon( m_DecIcon );

	m_IntValidator = NULL;
	m_FloatValidator = NULL;
	if( m_Type == nType_Float )
	{
		m_FloatValidator = new QDoubleValidator();
		m_LineEdit->setValidator( m_FloatValidator );
	}
	else if( m_Type == nType_Int )
	{
		m_IntValidator = new QIntValidator();
		m_LineEdit->setValidator( m_IntValidator );
	}

	m_LineEdit->setContextMenuPolicy( Qt::CustomContextMenu );

	m_IncButton->setContentsMargins( 0,0,0,0 );
	m_DecButton->setContentsMargins( 0,0,0,0 );
	m_IncButton->setMaximumWidth( 9 );
	m_DecButton->setMaximumWidth( 9 );
	m_IncButton->setMinimumWidth( 9 );
	m_DecButton->setMinimumWidth( 9 );
	m_IncButton->setAutoRepeat( TRUE );
	m_DecButton->setAutoRepeat( TRUE );

	connect( m_IncButton, SIGNAL( clicked() ), this, SLOT( OnUIIncrement() ) );
	connect( m_DecButton, SIGNAL( clicked() ), this, SLOT( OnUIDecrement() ) );
	connect( m_LineEdit, SIGNAL( textEdited( const QString & ) ), this, SLOT( OnUITextEdited( const QString & ) ) );
	m_LineEdit->installEventFilter( this );

	vLayout->addWidget( m_IncButton );
	vLayout->addWidget( m_DecButton );

	hLayout->addWidget( m_LineEdit );
	hLayout->addItem( vLayout );

	m_LineEdit->setMinimumHeight( 20 );

	setLayout( hLayout );
	UpdateLineEdit();
}


CScalarWidget::~CScalarWidget()
{

}

Void CScalarWidget::CreateIcons()
{
	if( !( m_IncIcon.isNull() && m_IncIcon.isNull() ) )
		return;

	QImage decImg( 3,3, QImage::Format_ARGB32 );
	decImg.setPixel( QPoint( 0, 0 ), 0x00000000 ); decImg.setPixel( QPoint( 1, 0 ), 0x00000000 ); decImg.setPixel( QPoint( 2, 0 ), 0x00000000 );
	decImg.setPixel( QPoint( 0, 1 ), 0xff000000 ); decImg.setPixel( QPoint( 1, 1 ), 0xff000000 ); decImg.setPixel( QPoint( 2, 1 ), 0xff000000 );
	decImg.setPixel( QPoint( 0, 2 ), 0x00000000 ); decImg.setPixel( QPoint( 1, 2 ), 0xff000000 ); decImg.setPixel( QPoint( 2, 2 ), 0x00000000 );

	m_DecIcon = QIcon( QPixmap::fromImage( decImg ) );

	QImage incImg( 3,3, QImage::Format_ARGB32 );
	incImg.setPixel( QPoint( 0, 0 ), 0x00000000 ); incImg.setPixel( QPoint( 1, 0 ), 0xff000000 ); incImg.setPixel( QPoint( 2, 0 ), 0x00000000 );
	incImg.setPixel( QPoint( 0, 1 ), 0xff000000 ); incImg.setPixel( QPoint( 1, 1 ), 0xff000000 ); incImg.setPixel( QPoint( 2, 1 ), 0xff000000 );
	incImg.setPixel( QPoint( 0, 2 ), 0x00000000 ); incImg.setPixel( QPoint( 1, 2 ), 0x00000000 ); incImg.setPixel( QPoint( 2, 2 ), 0x00000000 );

	m_IncIcon = QIcon( QPixmap::fromImage( incImg ) );

}

bool CScalarWidget::eventFilter(QObject *obj, QEvent *event)
{
	if (event->type() == QEvent::MouseButtonRelease) 
	{
		QMouseEvent* mouseEvent = (QMouseEvent*)event;
		if( mouseEvent->button() & Qt::RightButton )
			ResetToDefault();

		return true;
	} 
	else 
	{
		// standard event processing
		return QObject::eventFilter(obj, event);
	}
}

Void CScalarWidget::ResetToDefault( )
{
	if( m_Type == nType_Float )
		SetValue( m_FloatDefaultValue );
	else if( m_Type == nType_Int )
		SetValue( (Int32) m_IntDefaultValue );
}

void CScalarWidget::OnUIDecrement()
{
	if( m_Type == nType_Int )
	{
		m_IntValue = m_IntValue - (Int32) m_Step;
	}
	else if( m_Type == nType_Float )
	{
		m_FloatValue = m_FloatValue - (Float32) m_Step;
	}

	ClampValue();
	UpdateLineEdit();

	Q_EMIT OnValueChanged();
}

void CScalarWidget::OnUIIncrement()
{
	if( m_Type == nType_Int )
	{
		m_IntValue = m_IntValue + (Int32) m_Step;
	}
	else if( m_Type == nType_Float )
	{
		m_FloatValue = m_FloatValue + (Float32) m_Step;
	}

	ClampValue();
	UpdateLineEdit();

	Q_EMIT OnValueChanged();
}

Void CScalarWidget::ClampValue()
{
	if( m_FloatRange.GetHighValue() != FLOAT32_MAX_VAL || m_FloatRange.GetLowValue() != FLOAT32_MIN_VAL )
	{
		if( m_Type == nType_Int )
		{
			m_IntValue = (Int32)m_FloatRange.Clamp( m_IntValue );
		}
		else if( m_Type == nType_Float )
		{
			m_FloatValue = (Float32)m_FloatRange.Clamp( m_FloatValue );
		}
	}
}

Void CScalarWidget::SetValue( Int32 value, Bool trigerSignal )
{
	m_IntValue = value;
	ClampValue();
	UpdateLineEdit();

	if( trigerSignal )
		Q_EMIT OnValueChanged();
}

Void CScalarWidget::SetValue( Float32 value, Bool trigerSignal )
{
	m_FloatValue = value;
	ClampValue();
	UpdateLineEdit();

    if( trigerSignal )
	    Q_EMIT OnValueChanged();
}

Void CScalarWidget::UpdateLineEdit()
{
	QString str;
	if( m_Type == nType_Int )
	{
		str = QString::number( m_IntValue );
	}
	else if( m_Type == nType_Float )
	{
		str = QString::number( m_FloatValue );
	}
	m_LineEdit->setText( str );
}

Float32 CScalarWidget::GetValueAsFloat() const
{
	return m_FloatValue;
}

Int32 CScalarWidget::GetValueAsInt() const
{
	return m_IntValue;
}

void CScalarWidget::OnUITextEdited( const QString& text )
{
	if( m_Type == nType_Float )
	{
		Float32 floatValue = 0.0f;
		floatValue = text.toDouble();
		if( floatValue != m_FloatValue )
		{
			m_FloatValue = floatValue;
			Q_EMIT OnValueChanged();
		}
	}
	else if( m_Type == nType_Int )
	{	
		Int32 intValue = 0.0f;
		intValue = text.toInt();
		if( intValue != m_IntValue )
		{
			m_IntValue = intValue;
			Q_EMIT OnValueChanged();
		}
	}

}
