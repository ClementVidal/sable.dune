#include <Dune\Editor\View\Editor\Particle\EmitterPoint.h>

#include <QtGui\QFormLayout>

using namespace Dune;

CPSEmitterPointPanel::CPSEmitterPointPanel( QWidget& parent, Sb::CParticleOpSourcePoint& op ) :
    CPSEmitterPanel( parent, op )
{
	m_OpPoint = NULL;

	m_Origin = new CVector3Widget( *this, Sb::CVector3f::Zero  );
	connect( m_Origin, SIGNAL( OnValueChanged() ), this, SLOT( OnUIOriginChanged() ) );
	m_FormLayout->addRow( "Origin", m_Origin );

	m_SpreadAngle = new QDoubleSpinBox( );
	connect( m_SpreadAngle, SIGNAL( valueChanged( double ) ), this, SLOT( OnUISpreadAngleChanged() ) );
	m_FormLayout->addRow( "SpreadAngle", m_SpreadAngle );

	SetEmitter( op );
}

CPSEmitterPointPanel::~CPSEmitterPointPanel()
{
}

Void CPSEmitterPointPanel::SetEmitter( Sb::CParticleOpSourcePoint& emitter )
{
	CPSEmitterPanel::SetEmitter( emitter );
	m_OpPoint = &emitter;
	m_SpreadAngle->setValue( emitter.GetSpreadAngle() );
	m_Origin->SetValue( emitter.GetOrigin() );
}

Bool CPSEmitterPointPanel::OnUIOriginChanged()
{
	m_OpPoint->SetOrigin( m_Origin->GetValue() );
	return TRUE;
}

Bool CPSEmitterPointPanel::OnUISpreadAngleChanged()
{
	m_OpPoint->SetSpreadAngle( m_SpreadAngle->value() );
	return TRUE;
}