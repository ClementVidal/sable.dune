#include <Dune\Editor\Widget\VectorRangeWidget.h> 

using namespace Dune;

CVector3RangeWidget::CVector3RangeWidget( QWidget& parent, const Sb::CVector3Range& value ) :
    QWidget( &parent )
{
	m_Value = value;
	QFormLayout* layout = new QFormLayout( );
	m_Min = new CVector3Widget( *this, value.GetLowValue() );
	m_Max = new CVector3Widget( *this, value.GetHighValue() );
	m_SendOnValueChangedSignal = TRUE;

    SetValue( value );

    connect( m_Min, SIGNAL( OnValueChanged( ) ), this, SLOT( OnUIMinValueChanged( ) ) );
    connect( m_Max, SIGNAL( OnValueChanged( ) ), this, SLOT( OnUIMaxValueChanged( ) ) );

    layout->addRow( "Min", m_Min );
    layout->addRow( "Max", m_Max );

	layout->setSpacing( 0 );
	layout->setContentsMargins( 0,0,0,0 );

	setLayout( layout );
}

CVector3RangeWidget::~CVector3RangeWidget()
{
	
}

Void CVector3RangeWidget::SetValue( const Sb::CVector3Range& range )
{
	m_SendOnValueChangedSignal = FALSE;

	m_Max->SetValue( range.GetHighValue() );
	m_Min->SetValue( range.GetLowValue() );

	m_Value = range;

	m_SendOnValueChangedSignal = TRUE;

    Q_EMIT OnValueChanged();
}

Bool CVector3RangeWidget::OnUIMaxValueChanged( )
{
	m_Value.SetHighValue( m_Max->GetValue() );

	if( m_SendOnValueChangedSignal )
		Q_EMIT OnValueChanged();

	return TRUE;
}

Bool CVector3RangeWidget::OnUIMinValueChanged( )
{
	m_Value.SetLowValue( m_Min->GetValue() );

	if( m_SendOnValueChangedSignal )
		Q_EMIT OnValueChanged();

	return TRUE;
}

const Sb::CVector3Range& CVector3RangeWidget::GetValue() const
{
	return m_Value;
}


CVector4RangeWidget::CVector4RangeWidget( QWidget& parent, const Sb::CVector4Range& value ) :
QWidget( &parent )
{
	m_Value = value;
	QFormLayout* layout = new QFormLayout( );
	m_Min = new CVector4Widget( *this, value.GetLowValue() );
	m_Max = new CVector4Widget( *this, value.GetHighValue() );
	m_SendOnValueChangedSignal = TRUE;

	SetValue( value );

	bool con;
	con = connect( m_Min, SIGNAL( OnValueChanged( ) ), this, SLOT( OnUIMinValueChanged( ) ) );
	con = connect( m_Max, SIGNAL( OnValueChanged( ) ), this, SLOT( OnUIMaxValueChanged( ) ) );

	layout->addRow( "Min", m_Min );
	layout->addRow( "Max", m_Max );

	layout->setSpacing( 0 );
	layout->setContentsMargins( 0,0,0,0 );

	setLayout( layout );
}

CVector4RangeWidget::~CVector4RangeWidget()
{

}

Void CVector4RangeWidget::SetValue( const Sb::CVector4Range& range )
{
	m_SendOnValueChangedSignal = FALSE;

	m_Max->SetValue( range.GetHighValue() );
	m_Min->SetValue( range.GetLowValue() );

	m_Value = range;

	m_SendOnValueChangedSignal = TRUE;

	Q_EMIT OnValueChanged();
}

Bool CVector4RangeWidget::OnUIMaxValueChanged( )
{
	m_Value.SetHighValue( m_Max->GetValue() );

	if( m_SendOnValueChangedSignal )
		Q_EMIT OnValueChanged();

	return TRUE;
}

Bool CVector4RangeWidget::OnUIMinValueChanged( )
{
	m_Value.SetLowValue( m_Min->GetValue() );

	if( m_SendOnValueChangedSignal )
		Q_EMIT OnValueChanged();

	return TRUE;
}

const Sb::CVector4Range& CVector4RangeWidget::GetValue() const
{
	return m_Value;
}