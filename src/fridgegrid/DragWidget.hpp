/*
 * src/fridgegrid/DragWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef DRAGWIDGET_HPP
#define DRAGWIDGET_HPP DRAGWIDGET_HPP

/* base class */
#include <QLabel>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */
#include "GridStyle.hpp"

/* forward declaration of Qt classes */
class QDomDocument;
class QDomElement;
class QDragEnterEvent;
class QDropEvent;
class QPaintEvent;
class QTextEdit;

/* forward declaration of local classes */


/*!
 \brief \todo

*/
class DragWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief \todo

    \param parent
   */
   explicit DragWidget( QWidget *parent = 0 );
   /*!
    \brief \todo

   */
   virtual ~DragWidget();
   /*!
    \brief \todo

    \return QSize
   */
   QSize gridSize();

   QVariant getData();
   void setData( const QVariant &data );

   GridStyle gridStyle() const;
   void setGridStyle( const GridStyle &gridStyle );

   /*!
    \brief \todo

    \param textEdit
   */
   void setTextEdit( QTextEdit *textEdit );

   /*!
    \brief \todo

    \param pos
    \return QPoint
   */
   QPoint grid2pos( const QPoint &pos );
   /*!
    \brief \todo

    \param grid
    \return QPoint
   */
   QPoint pos2grid( const QPoint &grid );
public slots:
   /*!
    \brief \todo

   */
   void save();
   /*!
    \brief \todo

   */
   void load();
   /*!
    \brief \todo

   */
   void exportPng();
   /*!
    \brief \todo

    \param d
    \param p
   */
   void newElement( const QByteArray &d = QByteArray(), const QPoint &p = QPoint() );

protected:
   /*!
    \brief \todo

    \param event
   */
   void dragEnterEvent( QDragEnterEvent *event );
   /*!
    \brief \todo

    \param event
   */
   void dragMoveEvent( QDragMoveEvent *event );
   /*!
    \brief \todo

    \param event
   */
   void dropEvent( QDropEvent *event );
   /*!
    \brief \todo

    \param event
   */
   void mousePressEvent( QMouseEvent *event);
   /*!
    \brief \todo

    \param event
   */
   void mouseDoubleClickEvent( QMouseEvent *event );
   /*!
    \brief \todo

    \param event
   */
   void paintEvent( QPaintEvent *event );

private:
   /*!
    \brief \todo

    \param w
    \param x
    \param y
   */
   void place( QWidget *w, int x, int y );
   /*!
    \brief \todo

    \param w
    \param p
   */
   void place( QWidget *w, const QPoint &p );
   /*!
    \brief \todo

    \param newSize
   */
   void updateLabelPos( const QSize &newSize );

   QTextEdit   *mpTextEdit; /*!< \brief \todo TODO */
   GridStyle   mGridStyle; /*!< \brief \todo TODO */
};

#endif
