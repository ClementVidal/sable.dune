#ifndef _DUNE_EDITOR_WIDGET_DEPENDENCYVIEWER_
#define _DUNE_EDITOR_WIDGET_DEPENDENCYVIEWER_

#include <Sable\Core\Common\DataTypes.h>
#include <Dune\Editor\Application\Application.h>

#include <QtGui\QDialog>
#include <QtGui\QTreeWidget>


namespace Dune
{

class CBankObject;

/**
Object dependency viewer
*/
class CDependencyViewer : public QDialog
{
    
    Q_OBJECT()

public:

    /** @name Constructor/Destructor*/
    //@{
    CDependencyViewer( CApplication& app, const CBankObject& obj );
    virtual ~CDependencyViewer();
    //@}

    /** @name Accesors*/
    //@{
    CApplication& GetApp() ;
    //@}

    /** @name Manipulator*/
    //@{
    //@}

private:

    // Types

    // Methods
    Q_SLOT Bool         OnUIOk();
    Q_SLOT Bool         OnUIExpandOrCollapse( QTreeWidgetItem* item );
    Q_SLOT Bool         OnUIDisplayOutputDependency();
    Q_SLOT Bool         OnUIDisplayInputDependency();

    Void                Populate(const CBankObject& obj, QTreeWidgetItem& parent, Bool outputOrInput );

    // Attributes
    const CBankObject*      m_Object;
    QTreeWidget*            m_TreeCtrl;
    CApplication*       m_App;

};

};

#endif