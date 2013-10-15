#include <Dune\Editor\Widget\Float32RangeWidget.h> 

using namespace Dune;

CFloat32RangeWidget::CFloat32RangeWidget( QWidget& parent, const Sb::CFloat32Range& value ) :
    QWidget( &parent )
{
	m_Value = value;
	QFormLayout* layout = new QFormLayout( );
	m_Min = new QDoubleSpinBox( );
	m_Max = new QDoubleSpinBox( );
	m_Max->setRange( FLOAT32_MIN_VAL, FLOAT32_MAX_VAL );
	m_Min->setRange( FLOAT32_MIN_VAL, FLOAT32_MAX_VAL );
	m_SendOnValueChangedSignal = TRUE;

    SetValue( value );

    connect( m_Min, SIGNAL( valueChanged( double ) ), this, SLOT( OnUIMinValueChanged( double ) ) );
    connect( m_Max, SIGNAL( valueChanged( double ) ), this, SLOT( OnUIMaxValueChanged( double ) ) );

    layout->addRow( "Min", m_Min );
    layout->addRow( "Max", m_Max );

	layout->setSpacing( 0 );
	layout->setContentsMargins( 0,0,0,0 );

	setLayout( layout );
}

CFloat32RangeWidget::~CFloat32RangeWidget()
{
	
}

Void CFloat32RangeWidget::SetValue( const Sb::CFloat32Range& range )
{
	m_SendOnValueChangedSignal = FALSE;

	m_Max->setValue( range.GetHighValue() );
	m_Min->setValue( range.GetLowValue() );

	m_Value = range;

	m_SendOnValueChangedSignal = TRUE;

    Q_EMIT OnValueChanged();
}

Bool CFloat32RangeWidget::OnUIMaxValueChanged( double d )
{
	m_Value.SetHighValue( m_Max->value() );

	if( m_SendOnValueChangedSignal )
		Q_EMIT OnValueChanged();

	return TRUE;
}

Bool CFloat32RangeWidget::OnUIMinValueChanged( double d )
{
	m_Value.SetLowValue( m_Min->value() );

	if( m_SendOnValueChangedSignal )
		Q_EMIT OnValueChanged();

	return TRUE;
}

const Sb::CFloat32Range& CFloat32RangeWidget::GetValue() const
{
	return m_Value;
}