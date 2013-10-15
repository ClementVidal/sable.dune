#ifndef _DUNE_EDITOR_VIEW_EDITOR_PARTICLE_KILLOLD_
#define _DUNE_EDITOR_VIEW_EDITOR_PARTICLE_KILLOLD_

#include <Sable\Core\Common\DataTypes.h>
#include <Sable\Graphics\Particle\Operator\KillOld.h>

#include <QtGui/QFrame>
#include <QtGui/QFormLayout>



namespace Dune
{

class CViewParticleSystem;

/**
*/
class CPSKillOldPanel : public CPSOperatorPanel
{

	Q_OBJECT

public:

	/** @name DataTypes*/
	//@{
	//@}

    /** @name Constructor/Destructor*/
    //@{
    CPSKillOldPanel( QWidget& parent, Sb::CParticleOpKillOld& op );
    virtual ~CPSKillOldPanel();
    //@}

    /** @name Accessor*/
    //@{
    //@}

    /** @name Manipulator*/
    //@{
    //@}

protected:

    // Methods
	Q_SLOT Bool OnUIAgeLimitChanged( );
	Void SetOperator( Sb::CParticleOpKillOld& emitter  );

    // Attributes
	Sb::CParticleOpKillOld*		m_Operator;
	CFloat32RangeWidget*			m_AgeLimit;
};

};

#endif