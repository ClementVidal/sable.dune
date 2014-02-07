#include <Dune/Editor/View/Scene.h> 

using namespace Dune;

CViewScene::CViewScene( CViewMainFrame& mainFrame ) :
    QDockWidget( "Scene Browser", &mainFrame ),
	CEventClient( mainFrame.GetApp() )
{
	QWidget* content = new QWidget();

    QVBoxLayout* layout = new QVBoxLayout();

	m_Tree = new CViewSceneTree( *this, GetApp() );
	layout->addWidget( m_Tree );

	layout->setContentsMargins( 0, 0, 0, 0 );
	content->setLayout( layout );

	setWidget( content );
}

CViewScene::~CViewScene()
{

}
