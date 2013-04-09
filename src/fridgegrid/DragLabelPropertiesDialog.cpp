/*
 * src/fridgegrid/MainWindow.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "DragLabelPropertiesDialog.hpp"

/* system headers */

/* Qt headers */
#include <QColorDialog>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>

/* local library headers */

/* local headers */
#include "DragLabel.hpp"



DragLabelPropertiesDialog::DragLabelPropertiesDialog( DragLabel *parent )
: QDialog( parent )
, mpParent( parent )
, mpTextEdit( new QLineEdit( this ) )
, mpWidthEdit( new QSpinBox( this ) )
, mpHeightEdit( new QSpinBox( this ) )
, mpColorEdit( new QLineEdit( this ) )
, mOld( parent->toByteArray() )
{
   QLabel *textLabel       = new QLabel(   tr("Text"), this );
   QLabel *widthLabel      = new QLabel(  tr("Width"), this );
   QLabel *heightLabel     = new QLabel( tr("Height"), this );
   QPushButton *ok         = new QPushButton( tr("Ok"), this );
   QPushButton *cancel     = new QPushButton( tr("Cancel"), this );
   QPushButton *colorLabel = new QPushButton( tr("Color"), this );
   QGridLayout *layout     = new QGridLayout( this );
   mpTextEdit->setText( mpParent->labelText() );
   mpWidthEdit->setValue( mpParent->gridSize().width() );
   mpHeightEdit->setValue( mpParent->gridSize().height() );
   mpWidthEdit->setRange( 1, 76 );
   mpHeightEdit->setRange( 1, 30 );
   mpColorEdit->setMaxLength( 6 );
   mpColorEdit->setText( mpParent->color().name().mid(1) );

   layout->addWidget( textLabel,    0, 0, 1, 2 );
   layout->addWidget( widthLabel,   1, 0, 1, 2 );
   layout->addWidget( heightLabel,  2, 0, 1, 2 );
   layout->addWidget( colorLabel,   3, 0, 1, 2 );
   layout->addWidget( mpTextEdit,   0, 2, 1, 2 );
   layout->addWidget( mpWidthEdit,  1, 2, 1, 2 );
   layout->addWidget( mpHeightEdit, 2, 2, 1, 2 );
   layout->addWidget( mpColorEdit,  3, 2, 1, 2 );
   layout->addWidget( ok,           4, 0 );
   layout->addWidget( cancel,       4, 3 );

   //ok->setShortcut( QKeySequence(Qt::Key_Enter) );
   connect( cancel, SIGNAL(clicked()),
            this, SLOT(reject()) );
   connect( ok, SIGNAL(clicked()),
            this, SLOT(accept()) );
   connect( colorLabel, SIGNAL(clicked()),
            this, SLOT(colorDialog()) );

   connect( mpTextEdit, SIGNAL(textChanged(QString)),
            parent, SLOT(setLabelText(QString)) );
   connect( mpWidthEdit, SIGNAL(valueChanged(int)),
            parent, SLOT(setGridSizeWidth(int)) );
   connect( mpHeightEdit, SIGNAL(valueChanged(int)),
            parent, SLOT(setGridSizeHeight(int)) );
   connect( mpColorEdit, SIGNAL(textChanged(QString)),
            this, SLOT(setColorValue(QString)) );

   connect( this, SIGNAL(rejected()),
            this, SLOT(revert()) );
}


DragLabelPropertiesDialog::~DragLabelPropertiesDialog()
{
}


void DragLabelPropertiesDialog::colorDialog()
{
   QColor old( mpParent->color() );
   QColorDialog colorDialog( this );
   colorDialog.setCurrentColor( old );
   connect( &colorDialog, SIGNAL(colorSelected(QColor)),
            mpParent, SLOT(setColor(QColor)) );
   connect( &colorDialog, SIGNAL(currentColorChanged(QColor)),
            this, SLOT(setColorValue(QColor)) );
   if( colorDialog.exec() == QDialog::Rejected )
   {
      mpParent->setColor( old );
      mpColorEdit->setText( old.name().mid(1) );
   }
}


void DragLabelPropertiesDialog::revert()
{
   mpParent->setFromByteArray( mOld );
}


void DragLabelPropertiesDialog::setColorValue( const QString &color )
{
   mpParent->setColor( QColor( QString("#%1").arg(color) ) );
}


void DragLabelPropertiesDialog::setColorValue( const QColor &color )
{
   mpParent->setColor( color );
   mpColorEdit->setText( color.name().mid(1) );
}
