/*
 * tools/src/fridgemagnets/MainWindow.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "MainWidget.hpp"

/* system headers */

/* Qt headers */
#include <QApplication>
#include <QDomDocument>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include <QFileDialog>
#include <QFileInfo>
#include <QMessageBox>
#include <QPainter>
#include <QSettings>
#include <QSplitter>
#include <QTextEdit>
#include <QTimer>
#include <QTextStream>
#include <QVBoxLayout>

//#include <QSize>

/* local library headers */

/* local headers */
#include "DragWidget.hpp"


MainWidget::MainWidget( QWidget *parent )
: QWidget( parent )
, mpParent( parent )
, mpSplitter( new QSplitter( Qt::Vertical, this ) )
, mpTextEdit( new QTextEdit( this ) )
, mpDragWidget( new DragWidget( this ) )
{
   QBoxLayout *layout   = new QVBoxLayout( this );
   mpSplitter->addWidget( mpTextEdit );
   mpSplitter->addWidget( mpDragWidget );
   mpSplitter->restoreState( QSettings().value("MainWidgetSplitter").toByteArray() );

   //mpTextEdit->setCurrentFont( QFont("Courier") );
   mpTextEdit->setFontFamily( "Courier" );
   QTimer *updateDelay = new QTimer( this );
   updateDelay->setInterval( 500 );
   updateDelay->setSingleShot( true );
   connect( mpTextEdit, SIGNAL(textChanged()),
            updateDelay, SLOT(start()) );
   connect( updateDelay, SIGNAL(timeout()),
            this, SLOT(commentChange()) );
   layout->setContentsMargins( 0, 0, 0, 0 );
   layout->addWidget( mpSplitter );
   setLayout( layout );

   commentChange();
}


MainWidget::~MainWidget()
{
   QSettings settings;
   settings.setValue( "MainWidgetSplitter", mpSplitter->saveState() );
   settings.setValue( "Comment", mpTextEdit->toPlainText() );
   settings.setValue( "MainField", mpDragWidget->getData() );
}


void MainWidget::commentChange()
{
   if( mpParent )
   {
      const QStringList &comment( mpTextEdit->toPlainText().split('\n', QString::SkipEmptyParts) );
      if( comment.size() > 0 )
      {
         mpParent->setWindowTitle( QString( "%1 (%2)").arg( comment.at(0),
                                                            QCoreApplication::applicationName() ) );
      }
      else
      {
         mpParent->setWindowTitle( QCoreApplication::applicationName() );
      }
   }
}


QSize MainWidget::gridSize()
{
   return mpDragWidget->gridSize();
}


void MainWidget::setGridX( int x )
{
   mpDragWidget->setGridX( x );
}


void MainWidget::setGridY( int y )
{
   mpDragWidget->setGridY( y );
}


void MainWidget::save()
{
   QSettings settings;
   QFileInfo lastFilename( settings.value( "Filename" ).toString() );
   QString fileName = QFileDialog::getSaveFileName( this, QCoreApplication::applicationName(),
                                                    lastFilename.absoluteFilePath(), "*.xml" );
   if( fileName.isEmpty() )
   {
      return;
   }
   if( !fileName.endsWith(".xml",Qt::CaseInsensitive ) )
   {
      fileName.append( ".xml" );
   }
   QDomDocument domDoc;
   QDomElement domRoot = domDoc.createElement( "FridgeGrid" );
   domDoc.appendChild( domRoot );
   QDomElement elements = domDoc.createElement( "elements" );
   domRoot.appendChild( elements );

   QDomElement comment = domDoc.createElement( "comment" );
   comment.setAttribute( "text", mpTextEdit->toPlainText() );
   domRoot.appendChild( comment );

   mpDragWidget->save( &domDoc, &domRoot, &elements );

   QFile file( fileName );
   const int indentSize = 2;
   file.open( QIODevice::WriteOnly );
   QTextStream out( &file );
   domDoc.save( out, indentSize );
   file.close();
}


void MainWidget::load( const QString &fileName )
{
   QFile file;
   if( fileName.isEmpty() )
   {
      QSettings settings;
      QFileInfo lastFilename( settings.value("Filename").toString() );
      QString selectedFileName = QFileDialog::getOpenFileName( this, QCoreApplication::applicationName(),
                                                       lastFilename.absolutePath(), "*.xml" );
      if( selectedFileName.isEmpty() )
      {
         return;
      }
      settings.setValue( "Filename", selectedFileName );
      file.setFileName( selectedFileName );
   }
   else
   {
      file.setFileName( fileName );
   }
   file.open( QIODevice::ReadOnly );

   QString errorStr;
   int errorLine;
   int errorColumn;
   QDomDocument domDoc;

   if( !domDoc.setContent( &file, true, &errorStr, &errorLine, &errorColumn ) )
   {
      QMessageBox::information( window(), QApplication::applicationName(),
                                 tr("Parse error at line %1, column %2:\n%3")
                                 .arg(errorLine)
                                 .arg(errorColumn)
                                 .arg(errorStr) );
      //return false;
   }

   QDomElement domRoot = domDoc.documentElement();
   if( domRoot.tagName() != "FridgeGrid" )
   {
      QMessageBox::information( window(), QApplication::applicationName(),
                                tr("The file is not a FridgeGrid file.") );
      //return false;
   }
#if 0
   else if (root.hasAttribute("version")
              && root.attribute("version") != "1.0")
   {
      QMessageBox::information( window(), QApplication::applicationName(),
                                tr("The file is not a FridgeGrid version 1.0 "
                                   "file.") );
      return false;
   }
#endif

   QDomElement comment = domRoot.firstChildElement( "comment" );
   mpTextEdit->setPlainText( comment.attribute("text") );
   QDomElement elements = domRoot.firstChildElement( "elements" );
   mpDragWidget->load( &elements );

   file.close();
}


void MainWidget::newElement( const QByteArray &d, const QPoint &p )
{
   mpDragWidget->newElement( d, p );
}
