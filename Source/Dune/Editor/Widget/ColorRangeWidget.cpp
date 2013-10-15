#include <Dune\Editor\Widget\ColorRangeWidget.h> 

#include <QtGui/QFormLayout>

using namespace Dune;

CColorRangeWidget::CColorRangeWidget( QWidget& parent, const Sb::CColorRange& value ) :
    QFrame( &parent )
{
	m_Value = value;

	m_Min = new CColorWidget( *this, value.GetLowValue() );
	m_Max = new CColorWidget( *this, value.GetHighValue() );

	QFormLayout* form = new QFormLayout();
	form->addRow( "Min", m_Min );
	form->addRow( "Max", m_Max );
	form->setContentsMargins( 0, 0, 0, 0 );
	m_SendOnValueChangedSignal = TRUE;

	SetValue( value );

	connect( m_Min, SIGNAL( OnValueChanged( double ) ), this, SLOT( OnUIMinValueChanged( double ) ) );
	connect( m_Max, SIGNAL( OnValueChanged( double ) ), this, SLOT( OnUIMaxValueChanged( double ) ) );

	setLayout( form );
}

CColorRangeWidget::~CColorRangeWidget()
{

}

Void CColorRangeWidget::SetValue( const Sb::CColorRange& range )
{
	m_SendOnValueChangedSignal = FALSE;

	m_Max->SetValue( range.GetHighValue() );
	m_Min->SetValue( range.GetLowValue() );

	m_Value = range;

	m_SendOnValueChangedSignal = TRUE;

	Q_EMIT OnValueChanged();
}

const Sb::CColorRange& CColorRangeWidget::GetValue() const
{
	return m_Value;
}


Bool CColorRangeWidget::OnUIMaxValueChanged( double d )
{
	m_Value.SetHighValue( m_Max->GetValue() );

	if( m_SendOnValueChangedSignal )
		Q_EMIT OnValueChanged();

	return TRUE;
}

Bool CColorRangeWidget::OnUIMinValueChanged( double d )
{
	m_Value.SetLowValue( m_Min->GetValue() );

	if( m_SendOnValueChangedSignal )
		Q_EMIT OnValueChanged();

	return TRUE;
}