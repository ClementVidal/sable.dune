#include <Dune\Editor\View\Editor\Particle\KillOld.h>

#include <Dune\Editor\Widget\VectorWidget.h>

using namespace Dune;

CPSKillOldPanel::CPSKillOldPanel( QWidget& parent, Sb::CParticleOpKillOld& op ) :
    CPSOperatorPanel( parent, op )
{
	m_AgeLimit = new CFloat32RangeWidget( *this, 0.0f );
	connect( m_AgeLimit, SIGNAL( OnValueChanged( ) ), this, SLOT( OnUIAgeLimitChanged() ) );
	m_FormLayout->addRow( "AgeLimit", m_AgeLimit );

	SetOperator( op );
	setLayout( m_FormLayout );
}

CPSKillOldPanel::~CPSKillOldPanel()
{
}

Void CPSKillOldPanel::SetOperator( Sb::CParticleOpKillOld& emitter  )
{
	m_Operator = &emitter;
	m_AgeLimit->SetValue( emitter.GetAgeLimit() );
}

Bool CPSKillOldPanel::OnUIAgeLimitChanged( )
{
	m_Operator->SetAgeLimit( m_AgeLimit->GetValue() );
	return TRUE;
}
