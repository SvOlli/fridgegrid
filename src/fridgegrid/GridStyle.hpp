/*
 * src/fridgegrid/MainWindow.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef STYLE_HPP
#define STYLE_HPP STYLE_HPP

/* base class */
#include <QDialog>

/* system headers */

/* Qt headers */
#include <QFont>
#include <QSize>
#include <QString>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */

/* forward declaration of local classes */


/*!
 \brief \todo

*/
class GridStyle
{
public:
   /*!
    \brief constructor

    \param name
    \param gridSize
    \param font
   */
   explicit GridStyle( const QString &name, const QSize &gridSize,
                       const QFont &font, const QPoint &topLeft,
                       bool toUpper );
   /*!
    \brief copy constructor

    \param parent
   */
   GridStyle( const GridStyle &that );
   /*!
    \brief assignment operator

    \param that
   */
   GridStyle &operator=( const GridStyle &that );
   bool operator<( const GridStyle &that );

   /*!
    \brief destructor

   */
   virtual ~GridStyle();

   Q_PROPERTY( QFont font
               READ font
               WRITE setFont )
   QFont font() const;
   void setFont( const QFont &font );
   Q_PROPERTY( QSize gridSize
               READ gridSize
               WRITE setGridSize )
   QSize gridSize() const;
   void setGridSize( const QSize &size );
   Q_PROPERTY( QPoint topLeft
               READ topLeft
               WRITE setTopLeft )
   QPoint topLeft() const;
   void setTopLeft( const QPoint &topLeft );
   Q_PROPERTY( bool toUpper
               READ toUpper
               WRITE setToUpper )
   bool toUpper() const;
   void setToUpper( bool toUpper );

private:
   QString  mName;
   QSize    mGridSize;
   QFont    mFont;
   QPoint   mTopLeft;
   bool     mToUpper;
};

#endif
