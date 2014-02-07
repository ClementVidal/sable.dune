#ifndef _DUNE_EDITOR_VIEW_EDITOR_PARTICLE_EMITTER_
#define _DUNE_EDITOR_VIEW_EDITOR_PARTICLE_EMITTER_

#include <Sable/Core/Common/DataTypes.h>

#include <Dune/Editor/Widget/Float32RangeWidget.h>
#include <Dune/Editor/Widget/ColorWidget.h>
#include <Dune/Editor/View/Editor/Particle/Operator.h>

#include <QtGui/QFrame>
#include <QtGui/QFormLayout>
#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QDoubleSpinBox>



namespace Dune
{

class CViewParticleSystem;

/**
*/
class CPSEmitterPanel : public CPSOperatorPanel
{

	Q_OBJECT

public:

    /** @name Constructor/Destructor*/
    //@{
    CPSEmitterPanel( QWidget& parent, Sb::CParticleOpSource& op );
    virtual ~CPSEmitterPanel();
    //@}

    /** @name Accessor*/
    //@{
    //@}

    /** @name Manipulator*/
    //@{
    //@}

protected:

    // Methods
	Q_SLOT Bool OnUIEmitRateChanged( double d );
	Q_SLOT Bool OnUIEmissionTimeoutChanged();
	Q_SLOT Bool OnUISpeedChanged();
	Q_SLOT Bool OnUIScaleChanged();
	Q_SLOT Bool OnUILifeTimeChanged();
	Q_SLOT Bool OnUIColorChanged();
	virtual Void SetEmitter( Sb::CParticleOpSource& emitter );

    // Attributes
	Sb::CParticleOpSource*		m_OpSource;

	QDoubleSpinBox*			m_EmitRate;
	QDoubleSpinBox*			m_EmissionTimeout;
	CFloat32RangeWidget*	m_Speed;
	CFloat32RangeWidget*	m_Scale;
	CFloat32RangeWidget*	m_LifeTime;
	CColorWidget*			m_Color;
};

};

#endif