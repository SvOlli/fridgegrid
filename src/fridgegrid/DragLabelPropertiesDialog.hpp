/*
 * src/fridgegrid/MainWindow.hpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

#ifndef DRAGLABELPROPERTIESDIALOG_HPP
#define DRAGLABELPROPERTIESDIALOG_HPP DRAGLABELPROPERTIESDIALOG_HPP

/* base class */
#include <QDialog>

/* system headers */

/* Qt headers */

/* local library headers */

/* local headers */

/* forward declaration of Qt classes */
class QLineEdit;
class QSpinBox;

/* forward declaration of local classes */
class DragLabel;


/*!
 \brief \todo

*/
class DragLabelPropertiesDialog : public QDialog
{
   Q_OBJECT
public:
   /*!
    \brief \todo

    \param parent
   */
   explicit DragLabelPropertiesDialog( DragLabel *parent = 0 );
   /*!
    \brief \todo

   */
   virtual ~DragLabelPropertiesDialog();

signals:
   
public slots:
   /*!
    \brief \todo

   */
   void colorDialog();
   /*!
    \brief \todo

    \param color
   */
   void setColorValue( const QString &color );
   /*!
    \brief \todo

    \param color
   */
   void setColorValue( const QColor &color );
   /*!
    \brief \todo

   */
   void revert();

private:
   DragLabel   *mpParent; /*!< \brief \todo TODO */
   QLineEdit   *mpTextEdit; /*!< \brief \todo TODO */
   QSpinBox    *mpWidthEdit; /*!< \brief \todo TODO */
   QSpinBox    *mpHeightEdit; /*!< \brief \todo TODO */
   QLineEdit   *mpColorEdit; /*!< \brief \todo TODO */
   QByteArray  mOld; /*!< \brief \todo TODO */
};

#endif
