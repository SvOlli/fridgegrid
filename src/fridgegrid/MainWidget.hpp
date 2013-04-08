/*
 * tools/src/fridgemagnets/MainWidget.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef MAINWIDGET_HPP
#define MAINWIDGET_HPP MAINWIDGET_HPP

/* base class */
#include <QLabel>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QSplitter;
class QTextEdit;

/* forward declaration of local classes */
class DragWidget;


/*!
 \brief \todo

*/
class MainWidget : public QWidget
{
   Q_OBJECT

public:
   /*!
    \brief constructor

    \param parent
   */
   explicit MainWidget( QWidget *parent = 0 );
   /*!
    \brief destructor

   */
   virtual ~MainWidget();
   /*!
    \brief \todo

    \return QSize
   */
   QSize gridSize();

public slots:
   /*!
    \brief \todo

    \param x
   */
   void setGridX( int x );
   /*!
    \brief \todo

    \param y
   */
   void setGridY( int y );
   /*!
    \brief save current setup to XML file

   */
   void save();
   /*!
    \brief load current setup to XML file

   */
   void load( const QString &fileName = QString() );
   /*!
    \brief \todo

    \param d
    \param p
   */
   void newElement( const QByteArray &d = QByteArray(), const QPoint &p = QPoint() );
   /*!
    \brief \todo

   */
   void commentChange();

private:
   QWidget        *mpParent;
   QSplitter      *mpSplitter;
   QTextEdit      *mpTextEdit;
   DragWidget     *mpDragWidget;
};

#endif
