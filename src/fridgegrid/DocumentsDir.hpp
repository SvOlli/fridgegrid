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


/*!
 \brief A small abstraction layer that copies the template data
 to documents directory.

*/
class DocumentsDir
{
public:
   /*!
    \brief standard contructor

   */
   DocumentsDir();

   /*!
    \brief get a list of template files (full path)

    \return QFileInfoList
   */
   QFileInfoList getTemplates() const;

private:
   /*!
    \brief prohibit copying
    */
   DocumentsDir( const DocumentsDir &that );
   /*!
    \brief prohibit assigning
    */
   DocumentsDir &operator=( const DocumentsDir &that );

   /*!
    \brief get the full path of the templates directory

    This might be either a name pointing to somewhere inside
    the Documents directory, or to the compiled-in data.

    \return QString full path of templates directory
   */
   QString  getTemplatesDirName() const;

   /*!
    \brief get a list of template files (full path)

    \return QFileInfoList
   */
   void     generateTemplateDir();

   bool     mUseDocDir;                   /*!< \brief copy internal files to documents */
};

#endif // DOCUMENTSDIR_HPP
