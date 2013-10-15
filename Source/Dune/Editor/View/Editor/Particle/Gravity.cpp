#include <Dune\Editor\View\Editor\Particle\Gravity.h>

#include <Dune\Editor\Widget\VectorWidget.h>

using namespace Dune;

CPSGravityPanel::CPSGravityPanel( QWidget& parent, Sb::CParticleOpGravity& op ) :
    CPSOperatorPanel( parent, op )
{
	m_Gravity = new CVector3Widget( *this, Sb::CVector3f::Zero );
	connect( m_Gravity, SIGNAL( OnValueChanged( ) ), this, SLOT( OnUIGravityChanged( ) ) );
	m_FormLayout->addRow( "Gravity", m_Gravity );

	SetOperator( op );
	setLayout( m_FormLayout );
}

CPSGravityPanel::~CPSGravityPanel()
{
}

Void CPSGravityPanel::SetOperator( Sb::CParticleOpGravity& emitter )
{
	m_Operator = &emitter;
	m_Gravity->SetValue( m_Operator->GetGravity() );
}

Bool CPSGravityPanel::OnUIGravityChanged( )
{
	m_Operator->SetGravity( m_Gravity->GetValue() );
	return TRUE;
}