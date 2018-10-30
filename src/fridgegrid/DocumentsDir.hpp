/*
 * src/fridgegrid/MainWindow.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef DOCUMENTSDIR_HPP
#define DOCUMENTSDIR_HPP DOCUMENTSDIR_HPP

/* base class */

/* system headers */

/* Qt headers */
#include <QDir>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */




class DocumentsDir
{
public:
   DocumentsDir();

   QFileInfoList getTemplates() const;

private:
   DocumentsDir( const DocumentsDir &that );
   DocumentsDir &operator=( const DocumentsDir &that );

   QDir mDir;
   QString getTemplatesDirName() const;
   void generateTemplateDir();
};

#endif // DOCUMENTSDIR_HPP
