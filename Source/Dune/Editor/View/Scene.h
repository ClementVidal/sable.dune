#ifndef _DUNE_EDITOR_VIEW_SCENE_
#define _DUNE_EDITOR_VIEW_SCENE_

#include <Sable/Core/Common/DataTypes.h>

#include <Dune/Editor/Ctrl/EventClient.h>
#include <Dune/Editor/Utilities/IconTable.h>
#include <Dune/Editor/View/SceneTree.h>

#include <QtGui/QDockWidget>

namespace Dune
{

/**
Bank view
*/
class CViewScene : public QDockWidget, public CEventClient
{

	Q_OBJECT 

public:

    /** @name Constructor/Destructor*/
    //@{
    CViewScene( CViewMainFrame& mainFrame );
    ~CViewScene();
    //@}

    /** @name Accesors*/
    //@{
	//@}

    /** @name Manipulator*/
    //@{
    //@}

private:

    // friendship

    // Types

    // Methods

    // Attributes   
    CViewSceneTree*		m_Tree;
};

};

#endif