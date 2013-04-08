/*
 * tools/src/fridgemagnets/MainWindow.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP MAINWINDOW_HPP

/* base class */
#include <QMainWindow>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QDockWidget;
class QLineEdit;
class QModelIndex;
class QPushButton;
class QSplitter;
class QTextEdit;

/* forward declaration of local classes */
class DragWidget;


/*!
 \brief the central widget

*/
class MainWindow : public QMainWindow
{
   Q_OBJECT

public:
   /*!
    \brief constuctor

    \param parent
    \param flags
   */
   MainWindow( QWidget *parent = 0, Qt::WindowFlags flags = 0 );

   /*!
    \brief destructor

   */
   virtual ~MainWindow();

public slots:
   /*!
    \brief \todo

   */
   void commentChange();

signals:

protected:

private:
   Q_DISABLE_COPY( MainWindow )

   QSplitter      *mpSplitter; /*!< \brief \todo TODO */
   QTextEdit      *mpTextEdit; /*!< \brief \todo TODO */
   DragWidget     *mpDragWidget; /*!< \brief the central widget */
};

/*! @} */

#endif
