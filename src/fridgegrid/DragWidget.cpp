/*
 * src/fridgegrid/MainWindow.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "DragWidget.hpp"

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
#include <QTextStream>
#include <QTextEdit>

//#include <QSize>

/* local library headers */

/* local headers */
#include "DragLabel.hpp"


DragWidget::DragWidget( QWidget *parent )
: QWidget( parent )
, mpTextEdit( 0 )
, mGridStyle( "Default", QSize(15,15),
              QFont("ErbosDraco Nova Open NBP", 6), QPoint( 2, 1 ), true )
{
   setMinimumSize( 400, 200 );
   setAcceptDrops( true );
}


DragWidget::~DragWidget()
{
}


QSize DragWidget::gridSize()
{
   return mGridStyle.gridSize();
}


void DragWidget::setTextEdit( QTextEdit *textEdit )
{
   mpTextEdit = textEdit;
}


void DragWidget::updateLabelPos( const QSize &newSize )
{
   QMap<DragLabel*,QPoint> sizemap;
   QList<DragLabel*> labels( findChildren<DragLabel*>() );
   foreach( DragLabel *label, labels )
   {
      sizemap.insert( label, pos2grid( label->pos() ) );
   }
   mGridStyle.setGridSize( newSize );
   foreach( DragLabel *label, labels )
   {
      place( label, grid2pos( sizemap.value(label) ) );
      label->draw();
   }
   update();
}


QPoint DragWidget::grid2pos( const QPoint &pos )
{
   return QPoint( pos.x() / mGridStyle.gridSize().width(),
                  pos.y() / mGridStyle.gridSize().height() );
}


QPoint DragWidget::pos2grid( const QPoint &grid )
{
   return QPoint( grid.x() * mGridStyle.gridSize().width(),
                  grid.y() * mGridStyle.gridSize().height() );
}


void DragWidget::dragEnterEvent( QDragEnterEvent *event )
{
   if( event->mimeData()->hasFormat("application/x-fridgemagnet") )
   {
      if( children().contains( event->source() ) )
      {
         event->setDropAction( Qt::MoveAction );
         event->accept();
      }
      else
      {
         event->acceptProposedAction();
      }
   }
   else if( event->mimeData()->hasText() )
   {
      event->acceptProposedAction();
   }
   else
   {
      event->ignore();
   }
}


void DragWidget::dragMoveEvent( QDragMoveEvent *event )
{
   if( event->mimeData()->hasFormat("application/x-fridgemagnet") )
   {
      if( children().contains(event->source()) )
      {
         event->setDropAction(Qt::MoveAction);
         event->accept();
      }
      else
      {
         event->acceptProposedAction();
      }
   }
   else if( event->mimeData()->hasText() )
   {
      event->acceptProposedAction();
   }
   else
   {
      event->ignore();
   }
}


void DragWidget::dropEvent( QDropEvent *event )
{
   if( event->mimeData()->hasFormat("application/x-fridgemagnet") )
   {
      const QMimeData *mime = event->mimeData();

      QByteArray itemData( mime->data("application/x-fridgemagnet") );
      QDataStream dataStream( &itemData, QIODevice::ReadOnly );

      QPoint offset;
      QByteArray data;
      dataStream >> offset >> data;
      DragLabel *newLabel = new DragLabel( data, this );

      place( newLabel, event->pos() - offset );
      newLabel->show();
      newLabel->setAttribute( Qt::WA_DeleteOnClose );

      if( event->source() == this )
      {
         event->setDropAction( Qt::MoveAction );
         event->accept();
      }
      else
      {
         event->acceptProposedAction();
      }
   }
   else if( event->mimeData()->hasText() )
   {
      QStringList pieces = event->mimeData()->text().split( '\n', QString::SkipEmptyParts );
      QPoint position = event->pos();

      foreach( const QString &piece, pieces )
      {
         DragLabel *newLabel = new DragLabel( this );
         newLabel->setText( piece );
         if( event->mimeData()->hasColor() )
         {
            QColor color = qvariant_cast<QColor>( event->mimeData()->colorData() );
            newLabel->setColor( color );
         }
         place( newLabel, position );
         newLabel->show();
         newLabel->setAttribute( Qt::WA_DeleteOnClose );

         position += QPoint( newLabel->width(), 0 );
      }

      event->acceptProposedAction();
   }
   else
   {
      event->ignore();
   }
}


void DragWidget::mousePressEvent( QMouseEvent *event )
{
   if( event->button() != Qt::LeftButton )
   {
      return;
   }
   DragLabel *child = static_cast<DragLabel*>( childAt(event->pos()) );
   if( !child )
   {
      return;
   }

   QPoint hotSpot = event->pos() - child->pos();

   QByteArray itemData;
   QDataStream dataStream(&itemData, QIODevice::WriteOnly);
   dataStream << hotSpot << child->toByteArray();

   QMimeData *mimeData = new QMimeData;
   mimeData->setData( "application/x-fridgemagnet", itemData );
   mimeData->setText( child->labelText() );
   mimeData->setColorData( child->color() );

   QDrag *drag = new QDrag( this );
   drag->setMimeData( mimeData );
   drag->setPixmap( *(child->pixmap()) );
   drag->setHotSpot( hotSpot );

   child->hide();

   if( drag->exec( Qt::MoveAction | Qt::CopyAction, Qt::CopyAction ) == Qt::MoveAction )
   {
      child->close();
   }
   else
   {
      child->show();
   }
}


void DragWidget::mouseDoubleClickEvent( QMouseEvent *event )
{
   if( event->button() != Qt::LeftButton )
   {
      return;
   }
   DragLabel *child = static_cast<DragLabel*>( childAt(event->pos()) );
   if( !child )
   {
      return;
   }

   child->parameterMenu( event->globalPos() );
}


void DragWidget::paintEvent( QPaintEvent *event )
{
   Q_UNUSED( event );
   QPainter painter( this );
   painter.setRenderHint( QPainter::Antialiasing );
   painter.setBrush( Qt::black );
   for( int i = mGridStyle.gridSize().width() - 1; i < size().width() ; i += mGridStyle.gridSize().width() )
   {
      painter.drawLine( i, 0, i, size().height() - 1 );
   }
   for( int i = mGridStyle.gridSize().height() - 1; i < size().height() ; i += mGridStyle.gridSize().height() )
   {
      painter.drawLine( 0, i, size().width() - 1, i );
   }
   painter.end();
}


void DragWidget::place( QWidget *w, int x, int y )
{
   if( (x % mGridStyle.gridSize().width()) > (mGridStyle.gridSize().width() / 2) )
   {
      x += mGridStyle.gridSize().width();
   }
   if( (y % mGridStyle.gridSize().height()) > (mGridStyle.gridSize().height() / 2) )
   {
      y += mGridStyle.gridSize().height();
   }
   x -= (x % mGridStyle.gridSize().width());
   y -= (y % mGridStyle.gridSize().height());
   w->move( x + 1, y + 1 );
}


void DragWidget::place( QWidget *w, const QPoint &p )
{
   place( w, p.x(), p.y() );
}


void DragWidget::save()
{
   QSettings settings;
   QFileInfo lastFilename( settings.value( "Filename" ).toString() );
   QString fileName = QFileDialog::getSaveFileName( this, QCoreApplication::applicationName(),
                                                    lastFilename.absoluteFilePath(), "*.xml" );
   if( fileName.isEmpty() )
   {
      return;
   }
   settings.setValue( "Filename", fileName );

   QDomDocument domDoc;
   QDomElement domRoot = domDoc.createElement( "FridgeGrid" );
   domDoc.appendChild(domRoot);
   QDomElement comment = domDoc.createElement( "comment" );
   if( mpTextEdit )
   {
      comment.setAttribute( "text", mpTextEdit->toPlainText() );
   }
   domRoot.appendChild( comment );
   QDomElement elements = domDoc.createElement( "elements" );
   domRoot.appendChild( elements );

   QList<DragLabel*> labels( findChildren<DragLabel*>() );
   foreach( DragLabel *label, labels )
   {
      QDomElement domElement = domDoc.createElement("element");
      label->writeDomAttributes( &domElement );
      elements.appendChild(domElement);
   }

   QFile file( fileName );
   const int indentSize = 2;
   file.open( QIODevice::WriteOnly );
   QTextStream out( &file );
   domDoc.save( out, indentSize );
   file.close();
}


void DragWidget::load()
{
   QSettings settings;
   QFileInfo lastFilename( settings.value("Filename").toString() );
   QString fileName = QFileDialog::getOpenFileName( this, QCoreApplication::applicationName(),
                                                    lastFilename.absolutePath(), "*.xml" );
   if( fileName.isEmpty() )
   {
      return;
   }
   settings.setValue( "Filename", fileName );

   QFile file( fileName );
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

   QList<DragLabel*> labels( findChildren<DragLabel*>() );
   foreach( DragLabel *label, labels )
   {
      label->deleteLater();
   }

   QDomElement comment = domRoot.firstChildElement( "comment" );
   if( mpTextEdit )
   {
      mpTextEdit->setPlainText( comment.attribute("text") );
   }
   QDomElement elements = domRoot.firstChildElement( "elements" );
   QDomNodeList elementList = elements.elementsByTagName( "element" );
   for( int i = 0; i < elementList.size(); ++i )
   {
      const QDomElement &domElement = elementList.at(i).toElement();
      DragLabel *newLabel = new DragLabel( this );
      QPoint pos( newLabel->readDomAttributes( &domElement ) );
      if( pos.isNull() )
      {
         delete newLabel;
      }
      else
      {
         place( newLabel, pos );
         newLabel->show();
         newLabel->setAttribute(Qt::WA_DeleteOnClose);
      }
   }

   //return true;
}


void DragWidget::newElement( const QByteArray &d, const QPoint &p )
{
   DragLabel *wordLabel = 0;
   if( d.isNull() )
   {
      wordLabel = new DragLabel( this );
   }
   else
   {
      wordLabel = new DragLabel( d, this );
   }
   place( wordLabel, p.x(), p.y() );
   wordLabel->show();
   wordLabel->setAttribute( Qt::WA_DeleteOnClose );
}


QVariant DragWidget::getData()
{
   QVariantList list;
   QList<DragLabel*> labels( findChildren<DragLabel*>() );
   foreach( DragLabel *label, labels )
   {
      QVariantMap map;
      map.insert( "pos", pos2grid( label->pos() ) );
      map.insert( "data", label->toByteArray() );
      list.append( map );
   }
   return list;
}


void DragWidget::setData( const QVariant &data )
{
   QVariantList list( data.toList() );
   QVariant element;

   QList<DragLabel*> labels( findChildren<DragLabel*>() );
   foreach( DragLabel *label, labels )
   {
      label->deleteLater();
   }

   foreach( element, list )
   {
      QVariantMap map( element.toMap() );
      DragLabel *newLabel = new DragLabel( this );
      QPoint pos( grid2pos( map.value( "pos" ).toPoint() ) );
      newLabel->setFromByteArray( map.value( "data" ).toByteArray() );
      if( pos.isNull() )
      {
         delete newLabel;
      }
      else
      {
         place( newLabel, pos );
         newLabel->show();
         newLabel->setAttribute( Qt::WA_DeleteOnClose );
      }
   }
}


GridStyle DragWidget::gridStyle() const
{
   return mGridStyle;
}


void DragWidget::setGridStyle(const GridStyle &gridStyle)
{
   mGridStyle = gridStyle;
}
