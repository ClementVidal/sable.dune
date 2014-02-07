#ifndef _DUNE_EDITOR_CTRL_PROJECT_ASSET_
#define _DUNE_EDITOR_CTRL_PROJECT_ASSET_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Core/Event/Header.h>

#include <Dune/Editor/Ctrl/Ctrl.h>
#include <Dune/Model/Asset/FBX/Header.h>


namespace Sable
{
    class CTexture2D;
}

namespace Dune
{

class CViewAssetTree;
class CCtrlProject;
class CAsset;
class CPropertyInterface;
class CItmdBlock;
class CAnimCurveSet;
class CAssetFont;
class CAssetParticleSystem;
class CAssetShader;
class CAssetMaterial;
class CAssetTexture2D;

/** 
Main project class 
*/
class CCtrlAsset : public CCtrl
{

public:

    /** @name Constructor/Destructor*/
    //@{
    CCtrlAsset( CApplication& app );
    virtual ~CCtrlAsset();
    //@}

    /** @name Accessor*/
    //@{
    //@}

    /** @name Manipulator*/
    //@{
	Void				OnAddObject( const CHandle& parent, const CHandle& h );
	Void				ExportToLibrary( CBankObject& obj, const Sb::CFilePath& path );
	//@}

private:

    // Methods

    // Attributes

};

}

#endif
