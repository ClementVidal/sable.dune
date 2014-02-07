#include <Dune/Editor/Widget/VectorWidget.h> 

using namespace Dune;

CVector3Widget::CVector3Widget( QWidget& parent, const Sb::CVector3f& value  ) :
    QWidget( &parent )
{
	m_CanTrigerSignal = TRUE;
	QHBoxLayout* layout = new QHBoxLayout();
	
	layout->setSpacing( 0 );
	layout->setContentsMargins( 0,0,0,0 );

	m_X = new CScalarWidget( *this,0.0f );
	m_Y = new CScalarWidget( *this,0.0f );
	m_Z = new CScalarWidget( *this,0.0f );

	m_X->setSizePolicy( QSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored ) );
	m_Y->setSizePolicy( QSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored ) );
	m_Z->setSizePolicy( QSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored ) );

	layout->addWidget( m_X );
	layout->addWidget( m_Y );
	layout->addWidget( m_Z );

	connect( m_X, SIGNAL( OnValueChanged( ) ), this, SLOT( OnUIValueChanged( ) ) );
	connect( m_Y, SIGNAL( OnValueChanged( ) ), this, SLOT( OnUIValueChanged( ) ) );
	connect( m_Z, SIGNAL( OnValueChanged( ) ), this, SLOT( OnUIValueChanged( ) ) );

	m_X->SetValue( value.GetX() );
	m_Y->SetValue( value.GetY() );
	m_Z->SetValue( value.GetZ() );

    m_X->SetDecimals( 5 );
    m_Y->SetDecimals( 5 );
    m_Z->SetDecimals( 5 );

	setLayout( layout );

	SetValue( value );
	
}

CVector3Widget::~CVector3Widget()
{

}

Void CVector3Widget::SetValue( const Sb::CVector3f& value, Bool trigerSignal )
{
	m_Value = value;
	m_CanTrigerSignal = FALSE;

	m_X->SetValue( m_Value.GetX() );
	m_Y->SetValue( m_Value.GetY() );
	m_Z->SetValue( m_Value.GetZ() );

	m_CanTrigerSignal = TRUE;

    if( trigerSignal )
	    emit OnValueChanged();
}

const Sb::CVector3f& CVector3Widget::GetValue() const
{
	return m_Value;
}

Bool CVector3Widget::OnUIValueChanged( )
{
	if( m_CanTrigerSignal == FALSE )
		return TRUE;

	Sb::CVector3f v = Sb::CVector3f( m_X->GetValueAsFloat(), m_Y->GetValueAsFloat(), m_Z->GetValueAsFloat() );
	if( m_Value != v )
	{
		m_Value = v;
		emit OnValueChanged();
	}
	return TRUE;
}


CVector4Widget::CVector4Widget( QWidget& parent, const Sb::CVector4f& value  ) :
QWidget( &parent )
{
	m_CanTrigerSignal = TRUE;
	QHBoxLayout* layout = new QHBoxLayout();

	layout->setSpacing( 0 );
	layout->setContentsMargins( 0,0,0,0 );

	m_X = new CScalarWidget( *this,0.0f );
	m_Y = new CScalarWidget( *this,0.0f );
	m_Z = new CScalarWidget( *this,0.0f );
	m_W = new CScalarWidget( *this,0.0f );

	m_X->setSizePolicy( QSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored ) );
	m_Y->setSizePolicy( QSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored ) );
	m_Z->setSizePolicy( QSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored ) );
	m_W->setSizePolicy( QSizePolicy( QSizePolicy::Ignored, QSizePolicy::Ignored ) );

	layout->addWidget( m_X );
	layout->addWidget( m_Y );
	layout->addWidget( m_Z );
	layout->addWidget( m_W );

	connect( m_X, SIGNAL( OnValueChanged( ) ), this, SLOT( OnUIValueChanged( ) ) );
	connect( m_Y, SIGNAL( OnValueChanged( ) ), this, SLOT( OnUIValueChanged( ) ) );
	connect( m_Z, SIGNAL( OnValueChanged( ) ), this, SLOT( OnUIValueChanged( ) ) );
	connect( m_W, SIGNAL( OnValueChanged( ) ), this, SLOT( OnUIValueChanged( ) ) );

	m_X->SetValue( value.GetX() );
	m_Y->SetValue( value.GetY() );
	m_Z->SetValue( value.GetZ() );
	m_Z->SetValue( value.GetW() );

	m_X->SetDecimals( 5 );
	m_Y->SetDecimals( 5 );
	m_Z->SetDecimals( 5 );
	m_W->SetDecimals( 5 );

	setLayout( layout );

	SetValue( value );

}

CVector4Widget::~CVector4Widget()
{

}

Void CVector4Widget::SetValue( const Sb::CVector4f& value, Bool trigerSignal )
{
	m_Value = value;
	m_CanTrigerSignal = FALSE;

	m_X->SetValue( m_Value.GetX() );
	m_Y->SetValue( m_Value.GetY() );
	m_Z->SetValue( m_Value.GetZ() );
	m_W->SetValue( m_Value.GetW() );

	m_CanTrigerSignal = TRUE;

	if( trigerSignal )
		emit OnValueChanged();
}

const Sb::CVector4f& CVector4Widget::GetValue() const
{
	return m_Value;
}

Bool CVector4Widget::OnUIValueChanged( )
{
	if( m_CanTrigerSignal == FALSE )
		return TRUE;

	Sb::CVector4f v = Sb::CVector4f( m_X->GetValueAsFloat(), m_Y->GetValueAsFloat(), m_Z->GetValueAsFloat(), m_W->GetValueAsFloat() );
	if( m_Value != v )
	{
		m_Value = v;
		emit OnValueChanged();
	}
	return TRUE;
}
