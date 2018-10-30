/*
 * src/fridgegrid/MainWindow.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "DocumentsDir.hpp"

/* system headers */

/* Qt headers */
#include <QCoreApplication>
#include <QFileInfoList>
#include <QSettings>
#include <QStandardPaths>

/* local library headers */

/* local headers */


#define SETTINGS_CREATETEMPLATESDIR "CreateTemplatesDir"

DocumentsDir::DocumentsDir()
: mDir()
{
   QSettings settings;
   QDir templatesDir( getTemplatesDirName() );

   if( !settings.contains( SETTINGS_CREATETEMPLATESDIR ) )
   {
      settings.setValue( SETTINGS_CREATETEMPLATESDIR, true );
   }

   if( !templatesDir.exists() &&
       settings.value( SETTINGS_CREATETEMPLATESDIR ).toBool() )
   {
      generateTemplateDir();
   }
}


QFileInfoList DocumentsDir::getTemplates() const
{
   return QDir( getTemplatesDirName() ).entryInfoList( QStringList("*.xml"), QDir::Files );
}


QString DocumentsDir::getTemplatesDirName() const
{
   QSettings settings;
   if( settings.value( SETTINGS_CREATETEMPLATESDIR ).toBool() )
   {
      return QStandardPaths::writableLocation( QStandardPaths::DocumentsLocation ) +
             "/" + QCoreApplication::applicationName() + "/Templates";
   }
   else
   {
      return ":/templates/";
   }
}


void DocumentsDir::generateTemplateDir()
{
   QFileInfoList fil( QDir(":/templates/").entryInfoList() );
   QDir templatesDir( getTemplatesDirName() );

   if( !templatesDir.mkpath( "." ) )
   {
      return;
   }

   foreach( const QFileInfo &fi, fil )
   {
      QString srcFile( fi.absoluteFilePath() );
      QString destFile( templatesDir.absoluteFilePath( fi.fileName() ) );
      if( QFile::copy( srcFile, destFile ) )
      {
         QFile::setPermissions( destFile, QFile::permissions( destFile ) | QFile::WriteUser );
      }
   }
}
