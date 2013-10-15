#ifndef _DUNE_EDITOR_VIEW_EDITOR_PARTICLE_GRAVITY_
#define _DUNE_EDITOR_VIEW_EDITOR_PARTICLE_GRAVITY_

#include <Sable\Core\Common\DataTypes.h>

#include <QtGui\QFormLayout>



namespace Dune
{

class CViewParticleSystem;

/**
*/
class CPSGravityPanel : public CPSOperatorPanel
{

	Q_OBJECT

public:

	/** @name DataTypes*/
	//@{
	//@}

    /** @name Constructor/Destructor*/
    //@{
    CPSGravityPanel( QWidget& parent, Sb::CParticleOpGravity& op );
    virtual ~CPSGravityPanel();
    //@}

    /** @name Accessor*/
    //@{
	Void SetOperator( Sb::CParticleOpGravity& emitter );
    //@}

    /** @name Manipulator*/
    //@{
    //@}

	/** @name Signals*/
	//@{
	//@}

protected:

    // Methods
	Q_SLOT  Bool  OnUIGravityChanged( );

    // Attributes
	Sb::CParticleOpGravity*		m_Operator;
	CVector3Widget*			m_Gravity;
};

};

#endif