#ifndef _DUNE_EDITOR_VIEW_EDITOR_PARTICLE_EMITTERPOINT_
#define _DUNE_EDITOR_VIEW_EDITOR_PARTICLE_EMITTERPOINT_

#include <Sable\Core\Common\DataTypes.h>

#include <Dune\Editor\Widget\VectorWidget.h>



namespace Dune
{

class CViewParticleSystem;

/**
*/
class CPSEmitterPointPanel : public CPSEmitterPanel
{
	
	Q_OBJECT

public:

	/** @name DataTypes*/
	//@{
	//@}

    /** @name Constructor/Destructor*/
    //@{
    CPSEmitterPointPanel( QWidget& parent, Sb::CParticleOpSourcePoint& op );
    virtual ~CPSEmitterPointPanel();
    //@}

    /** @name Accessor*/
    //@{
	Void SetEmitter( Sb::CParticleOpSourcePoint& emitter );
	//@}

    /** @name Manipulator*/
    //@{
    //@}

private:

    // Methods
	Q_SLOT Bool OnUIOriginChanged();
	Q_SLOT Bool OnUISpreadAngleChanged();

    // Attributes
	Sb::CParticleOpSourcePoint*	m_OpPoint;
	CVector3Widget*			m_Origin;
	QDoubleSpinBox*		m_SpreadAngle;
};

};

#endif