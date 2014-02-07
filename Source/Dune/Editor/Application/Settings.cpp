#include <Dune/Editor/Application/Settings.h>

#include <Dune/Model/Project/Project.h>
#include <Dune/Common/Log.h>

#include <QtCore/QTextStream>

using namespace Dune;

CApplicationSettings::CApplicationSettings( )
{
    Sb::CFilePath path = CoreManager.GetFileSystem().GetRootPath();
    path.AppendDir( "Tmp" );
	m_InternalBakePath = path.GetPath();
    Load();
}

CApplicationSettings::~CApplicationSettings()
{
    Save();
}

Void CApplicationSettings::AddRecentProject( CProject& p )
{
    m_RecentProject.Delete( p.GetProjectFilePath().GetPath() );

    m_RecentProject.AddItemAtBegin( p.GetProjectFilePath().GetPath() );
    if( m_RecentProject.GetItemCount() >= 6 )
        m_RecentProject.RemoveItemAtEnd();

	Save();
}

Bool CApplicationSettings::Save()
{
	QDomDocument doc;
	QDomElement appNode = doc.createElement( "Application" );
	doc.appendChild( appNode );

	QDomElement recentProjects = doc.createElement( "RecentProject" );
	appNode.appendChild( recentProjects );

    Sb::CList< Sb::CStaticString<128> >::Iterator it;
    ForEachItem( it, m_RecentProject )
    {
        QDomElement project = doc.createElement( "Project" );
        project.setAttribute( "Path", it.GetData().GetBuffer() );
		recentProjects.appendChild( project );
    }
/*
	QDomElement internalBakePath = doc.createElement( "InternalBakePath" );
	internalBakePath.setAttribute( "Path", m_InternalBakePath.GetBuffer() );
	appNode.appendChild( internalBakePath );
*/
	QFile file( "ApplicationSettings.xml" );
	if( !file.open( QIODevice::WriteOnly ) )
		return FALSE;

	QTextStream ts( &file );
	ts << doc.toString();

	file.close();

    return TRUE;
}

String CApplicationSettings::GetInternalBakePath() const
{
	return m_InternalBakePath.GetBuffer();
}

Bool CApplicationSettings::Load()
{
	QDomDocument doc;
	QFile file( "ApplicationSettings.xml" );
	if (!file.open(QIODevice::ReadOnly))
	{
		file.close();
		return FALSE;
	}

	if( !doc.setContent( &file ) )
	{
		return FALSE;
	}
	file.close();

    QDomElement appNode = doc.documentElement();

    if( appNode.tagName() != "Application" )
        return FALSE;

    QDomNodeList childNode = appNode.childNodes();

    for(Int32 i=0;i<childNode.count();i++ )
    {
		if( childNode.item(i).isElement() )
		{
			QDomElement domElem = childNode.item(i).toElement();
			if( domElem.tagName() == "RecentProject" )
			{
				QDomNodeList projects = domElem.childNodes();

				for(Int32 j=0;j<projects.count();j++ )
				{
					QDomElement project = projects.item(j).toElement();
					QString s = "";
					s = project.attribute( "Path", s ); 
					if( s.length() )
					{
						QByteArray buffer = s.toLocal8Bit();
						m_RecentProject.AddItemAtEnd( (String)buffer.data() );
					}
				}
			}
	/*		else if( domElem.tagName() == "InternalBakePath" )
			{
				QString s = "";
				s = domElem.attribute( "Path", s ); 
				QByteArray buffer = s.toLocal8Bit();
				m_InternalBakePath = (String)buffer.data() ;
			}*/
		}
    }

    return TRUE;
}

const CApplicationSettings::RecentProjectList& CApplicationSettings::GetRecentProjectList() const
{
    return m_RecentProject;
}