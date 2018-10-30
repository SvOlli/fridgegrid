/*
 * src/fridgegrid/MainWindow.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#include <QApplication>
#include <QDir>
#include <QFontDatabase>
#include "MainWindow.hpp"

#include <QtDebug>

int main( int argc, char *argv[] )
{
   Q_INIT_RESOURCE(FridgeGrid);

   QApplication::setOrganizationName("SvOlli");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("FridgeGrid");

   QApplication app(argc, argv);
#ifdef QT_KEYPAD_NAVIGATION
   QApplication::setNavigationMode(Qt::NavigationModeCursorAuto);
#endif
   {
      QDir dir( ":/fonts" );
      foreach( const QFileInfo &entry, dir.entryInfoList( QStringList("*.ttf")) )
      {
         int id = QFontDatabase::addApplicationFont( entry.absoluteFilePath() );
      }
   }
   MainWindow window;

#ifdef Q_OS_SYMBIAN
   bool smallScreen = true;
#else
   bool smallScreen = QApplication::arguments().contains("-small-screen");
#endif

   if (smallScreen)
      window.showFullScreen();
   else
      window.show();

   return app.exec();
}
