#ifndef _DUNE_EDITOR_CTRL_EDITOR_PARTICLESYSTEM_
#define _DUNE_EDITOR_CTRL_EDITOR_PARTICLESYSTEM_

#include <Sable/Core/Common/DataTYpes.h>
#include <Sable/Core/ManagedObject/Ref.h>
#include <Sable/Core/Event/Header.h>
#include <Sable/Graphics/Particle/Operator/Header.h>
#include <Sable/Graphics/Camera/Perspective.h>

#include <Dune/Editor/Sable/EditorRenderer.h>

#include <Dune/Editor/Ctrl/Editor/Editor.h>

namespace Dune
{

class CPropertyVector2f;
class CViewParticleSystem;

/** 
Controller class for USerInterfaceEditor
*/
class CCtrlEditorParticleSystem : public CCtrlEditor
{

public:

    /** @name Constructor/Destructor*/
    //@{
    CCtrlEditorParticleSystem( CViewParticleSystem& view, const CHandle& ps );
    ~CCtrlEditorParticleSystem();
    //@}

    /** @name Accessor*/
    //@{
    const CHandle&  	GetRootObject() const;
	Sb::CParticleSystem*	GetParticleSystem()const;
    Sb::CCameraPerspective& GetSableCamera();
    //@}

    /** @name Manipulator*/
    //@{
	Bool				SerializeParticleFile( const Sb::CFilePath& path, EAccesMode mode ) const;

	Void					RemoveOperator( Sb::CParticleOperator& op );
	Sb::CParticleOperator*	AddOperator( const Sb::CTypeInfo& type );
    Void					OnProjectDestroyed();
	Void					Initialize();
	//@}

private:

    // DataType

    // Attributes

    // Methods

	// Attributes
	CHandle					    m_Handle;
	Sb::CParticleSystem*		m_ParticleSystem;
    Sb::CRef<Sb::CSceneWorld>	m_World;
    CSableEditorRenderer	    m_SableRenderer;
    Sb::CSceneView		        m_SableView;
    Sb::CCameraPerspective      m_SableCamera;
};

}

#endif
