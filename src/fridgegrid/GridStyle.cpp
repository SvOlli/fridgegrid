/*
 * src/fridgegrid/MainWindow.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "GridStyle.hpp"

/* system headers */

/* Qt headers */
#include <QColorDialog>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>

/* local library headers */

/* local headers */


GridStyle::GridStyle( const QString &name, const QSize &gridSize,
                      const QFont &font, const QPoint &topLeft,
                      bool toUpper )
: mName( name )
, mGridSize( gridSize )
, mFont( font )
, mTopLeft( topLeft )
, mToUpper( toUpper )
{
}


GridStyle::GridStyle( const GridStyle &that )
: mName( that.mName )
, mGridSize( that.mGridSize )
, mFont( that.mFont )
, mTopLeft( that.mTopLeft )
, mToUpper( that.mToUpper )
{
}


GridStyle &GridStyle::operator=( const GridStyle &that )
{
   mName     = that.mName;
   mGridSize = that.mGridSize;
   mFont     = that.mFont;
   mTopLeft  = that.mTopLeft;
   mToUpper  = that.mToUpper;

   return *this;
}


bool GridStyle::operator<( const GridStyle &that )
{
   return mName < that.mName;
}


GridStyle::~GridStyle()
{
}


QFont GridStyle::font() const
{
   return mFont;
}


void GridStyle::setFont( const QFont &font )
{
   mFont = font;
}


QSize GridStyle::gridSize() const
{
   return mGridSize;
}


void GridStyle::setGridSize( const QSize &size )
{
   mGridSize = size;
}


QPoint GridStyle::topLeft() const
{
   return mTopLeft;
}


void GridStyle::setTopLeft( const QPoint &topLeft )
{
   mTopLeft = topLeft;
}


bool GridStyle::toUpper() const
{
   return mToUpper;
}


void GridStyle::setToUpper( bool toUpper )
{
   mToUpper = toUpper;
}
