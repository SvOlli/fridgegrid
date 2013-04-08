/*
 * tools/src/fridgemagnets/MainWindow.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#include <QApplication>
#include "MainWindow.hpp"

int main(int argc, char *argv[])
{
   //Q_INIT_RESOURCE(fridgemagnets);

   QApplication::setOrganizationName("SvOlli");
   QApplication::setOrganizationDomain("svolli.org");
   QApplication::setApplicationName("FridgeGrid");

   QApplication app(argc, argv);
#ifdef QT_KEYPAD_NAVIGATION
   QApplication::setNavigationMode(Qt::NavigationModeCursorAuto);
#endif
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
