#include <Dune/Editor/View/Editor/Particle/Emitter.h>

using namespace Dune;

CPSEmitterPanel::CPSEmitterPanel( QWidget& parent, Sb::CParticleOpSource& op ) :
    CPSOperatorPanel( parent, op )
{

	m_EmitRate = new QDoubleSpinBox( );
	connect( m_EmitRate, SIGNAL( valueChanged( double ) ), this, SLOT( OnUIEmitRateChanged( double ) ) );
	m_FormLayout->addRow( "Rate", m_EmitRate );
	
	m_Speed = new CFloat32RangeWidget( *this, 0.0f );
	connect( m_Speed, SIGNAL( OnValueChanged( ) ), this, SLOT( OnUISpeedChanged() ) );
	m_FormLayout->addRow( "Speed", m_Speed );

	m_Scale = new CFloat32RangeWidget( *this, 0.0f );
	connect( m_Scale, SIGNAL( OnValueChanged( ) ), this, SLOT( OnUIScaleChanged( ) ) );
	m_FormLayout->addRow( "Scale", m_Scale );

	m_Color = new CColorWidget( *this, Sb::CColor::White );
	m_Color->ShowLineEdit( FALSE );
	connect( m_Color, SIGNAL( OnValueChanged( ) ), this, SLOT( OnUIColorChanged() ) );
	m_FormLayout->addRow( "Color", m_Color );

	setLayout( m_FormLayout );

	SetEmitter( op );
}

CPSEmitterPanel::~CPSEmitterPanel()
{
}

Void CPSEmitterPanel::SetEmitter( Sb::CParticleOpSource& emitter )
{
	CPSOperatorPanel::SetOperator( emitter );
	m_OpSource = &emitter;
	m_EmitRate->setValue( emitter.GetEmitRate() );
	m_Speed->SetValue( emitter.GetParticleSpeed() );
	m_Scale->SetValue( emitter.GetParticleScale() );
	m_Color->SetValue( emitter.GetParticleColor() );
}

Bool CPSEmitterPanel::OnUIEmitRateChanged( double d )
{
	m_OpSource->SetEmitRate( m_EmitRate->value() );
	return TRUE;
}

Bool CPSEmitterPanel::OnUIEmissionTimeoutChanged()
{
	return TRUE;
}

Bool CPSEmitterPanel::OnUISpeedChanged()
{
	m_OpSource->SetParticleSpeed( m_Speed->GetValue() );
	return TRUE;
}

Bool CPSEmitterPanel::OnUIScaleChanged()
{
	m_OpSource->SetParticleScale( m_Scale->GetValue() );
	return TRUE;
}

Bool CPSEmitterPanel::OnUILifeTimeChanged()
{
	return TRUE;
}

Bool CPSEmitterPanel::OnUIColorChanged()
{
	m_OpSource->SetParticleColor( m_Color->GetValue() );
	return TRUE;
}