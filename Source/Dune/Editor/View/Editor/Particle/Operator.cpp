#include <Dune\Editor\View\Editor\Particle\Operator.h>

using namespace Dune;


CPSOperatorPanel::CPSOperatorPanel( QWidget& parent, Sb::CParticleOperator& op ) :
    QWidget( &parent )
{
	m_FormLayout = new QFormLayout();

	m_StartTime = new CFloat32RangeWidget( *this, 0.0f );
	connect( m_StartTime, SIGNAL( OnValueChanged( ) ), this, SLOT( OnUIStartTimeChanged() ) );
	m_FormLayout->addRow( "Start time", m_StartTime );

	m_Duration = new CFloat32RangeWidget( *this, 1.0f );
	connect( m_Duration, SIGNAL( OnValueChanged( ) ), this, SLOT( OnUIDurationChanged() ) );
	m_FormLayout->addRow( "Duration", m_Duration );

	SetOperator( op );
	setLayout( m_FormLayout );	
}

CPSOperatorPanel::~CPSOperatorPanel()
{
}

Void CPSOperatorPanel::SetOperator( Sb::CParticleOperator& op )
{
	m_BasOperator = &op;
	m_StartTime->SetValue( m_BasOperator->GetStartTime() );
	m_Duration->SetValue( m_BasOperator->GetDuration() );
}


Bool CPSOperatorPanel::OnUIStartTimeChanged( )
{
	m_BasOperator->SetStartTime( m_StartTime->GetValue() );
	return TRUE;
}

Bool CPSOperatorPanel::OnUIDurationChanged( )
{
	m_BasOperator->SetDuration( m_Duration->GetValue() );
	return TRUE;
}

Sb::CParticleOperator& CPSOperatorPanel::GetOperator() const
{
	return *m_BasOperator;
}