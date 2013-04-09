/*
 * src/fridgegrid/DragLabel.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef DRAGLABEL_HPP
#define DRAGLABEL_HPP DRAGLABEL_HPP


/* base class */
#include <QLabel>

/* system headers */

/* Qt headers */
#include <QDomElement>

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QDragEnterEvent;
class QDragMoveEvent;
class QFrame;

/* forward declaration of local classes */
class DragWidget;


/*!
 \brief \todo

*/
class DragLabel : public QLabel
{
Q_OBJECT

public:
   /*!
    \brief \todo

    \param parent
   */
   DragLabel( DragWidget *parent );
   /*!
    \brief \todo

    \param size
    \param text
    \param parent
    \param color
   */
   DragLabel( const QSize &size, const QString &text, DragWidget *parent, const QColor &color = QColor(192,192,255) );
   /*!
    \brief \todo

    \param d
    \param parent
   */
   DragLabel( const QByteArray &d, DragWidget *parent );
   /*!
    \brief \todo

    \return QString
   */
   QString labelText() const;
   /*!
    \brief \todo

    \return QColor
   */
   QColor color() const;
   /*!
    \brief \todo

    \return QSize
   */
   QSize gridSize() const;
   /*!
    \brief \todo

    \param d
    \return
   */
   QPoint setFromByteArray( const QByteArray &d );
   /*!
    \brief \todo

    \return QByteArray
   */
   QByteArray toByteArray() const;
   /*!
    \brief \todo

    \param size
   */
   void setGridSize( const QSize &size );
   /*!
    \brief \todo

    \param domElement
   */
   void writeDomAttributes( QDomElement *domElement ) const;
   /*!
    \brief \todo

    \param domElement
    \return QPoint
   */
   QPoint readDomAttributes( const QDomElement *domElement );

public slots:
   /*!
    \brief \todo

   */
   void parameterMenu( const QPoint &position );
   /*!
    \brief \todo

    \param text
   */
   void setLabelText( const QString &text );
   /*!
    \brief \todo

    \param width
   */
   void setGridSizeWidth( int width );
   /*!
    \brief \todo

    \param height
   */
   void setGridSizeHeight( int height );
   /*!
    \brief \todo

    \param color
   */
   void setColor( const QColor &color );
   /*!
    \brief \todo

   */
   void draw();

signals:
   /*!
    \brief \todo

    \param d
    \param p
   */
   void requestCopy( const QByteArray &d, const QPoint &p );

protected:
   /*!
    \brief \todo

    \param event
   */
   void contextMenuEvent( QContextMenuEvent *event );

private:

   DragWidget  *mpParent; /*!< \brief \todo TODO */
   QSize       mSize; /*!< \brief \todo TODO */
   QString     mText; /*!< \brief \todo TODO */
   QColor      mColor; /*!< \brief \todo TODO */
};
#endif
