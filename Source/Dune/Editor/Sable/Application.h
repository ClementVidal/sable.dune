#ifndef _DUNE_APPLICATION_DUNE_
#define _DUNE_APPLICATION_DUNE_

#include <Sable/Core/Common/DataTypes.h>
#include <Sable/Application/Application.h>
#include <Sable/Core/Common/Manager.h>
#include <Sable/Graphics/Common/Manager.h>
#include <Sable/Input/Common/Manager.h>
#include <Sable/Core/Event/Header.h>

#include <Dune/Common/Log.h>

namespace Dune
{

class CApplication;
class CSableEditorRenderer;

/**  
Main class of the system.
*/
class CSableApplication : public Sb::CApplication
{

public:

    /** @name Constructor/destructor */
    //@{
	CSableApplication( Dune::CApplication& duneApp );
    ~CSableApplication();
    //@}

    /** @name Manipulator*/
    //@{
    virtual Void    Initialize();
    virtual Void    UnInitialize();
    virtual Bool    Update();
    //@}

    /** @name Accessors*/
    //@{
	CDuneLogHandler&					        GetLogHandler();
    //@}

    /** @name Signal*/
    //@{
    Sb::CEventSignal<CSableEditorRenderer& >*  SignalUpdate;
    Sb::CEventSignal<CSableEditorRenderer& >*  SignalRender;
    //@}

private:

    //Attribute
    CDuneLogHandler             m_LogHandler;
};

}

#endif
