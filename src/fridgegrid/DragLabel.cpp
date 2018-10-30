/*
 * src/fridgegrid/MainWindow.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "DragLabel.hpp"

/* system headers */

/* Qt headers */
#include <QContextMenuEvent>
#include <QMenu>
#include <QPainter>

/* local library headers */

/* local headers */
#include "DragWidget.hpp"
#include "DragLabelPropertiesDialog.hpp"



DragLabel::DragLabel( DragWidget *parent )
: QLabel(parent)
, mpParent( parent )
, mSize( 4, 1 )
, mText()
, mColor( 192, 192, 255 )
{
   draw();
   connect( this, SIGNAL(requestCopy(QByteArray,QPoint)),
            parent, SLOT(newElement(QByteArray,QPoint)) );
}


DragLabel::DragLabel( const QSize &size, const QString &text,
                      DragWidget *parent, const QColor &color )
: QLabel(parent)
, mpParent( parent )
, mSize( size )
, mText( text )
, mColor( color )
{
   draw();
   connect( this, SIGNAL(requestCopy(QByteArray,QPoint)),
            parent, SLOT(newElement(QByteArray,QPoint)) );
}


DragLabel::DragLabel( const QByteArray &d, DragWidget *parent )
: QLabel( parent )
, mpParent( parent )
, mSize()
, mText()
, mColor()
{
   setFromByteArray( d );
   connect( this, SIGNAL(requestCopy(QByteArray,QPoint)),
            parent, SLOT(newElement(QByteArray,QPoint)) );
}


QPoint DragLabel::setFromByteArray( const QByteArray &d )
{
   QPoint point;
   QByteArray dataCopy( d );
   QDataStream dataStream( &dataCopy, QIODevice::ReadOnly );
   dataStream >> mText >> mColor >> mSize >> point;
   draw();
   move( point );
   return point;
}


QByteArray DragLabel::toByteArray() const
{
   QByteArray data;
   QDataStream dataStream( &data, QIODevice::WriteOnly );
   dataStream << mText << mColor << mSize << pos();
   return data;
}


QString DragLabel::labelText() const
{
   return mText;
}


QColor DragLabel::color() const
{
   return mColor;
}


void DragLabel::setLabelText( const QString &text )
{
   mText = text;
   draw();
}


void DragLabel::setGridSizeWidth( int width )
{
   mSize.setWidth( width );
   draw();
}


void DragLabel::setGridSizeHeight( int height )
{
   mSize.setHeight( height );
   draw();
}


void DragLabel::setColor(const QColor &color)
{
   mColor = color;
   draw();
}


void DragLabel::parameterMenu( const QPoint &position )
{
   DragLabelPropertiesDialog dialog( this );
   dialog.move( position - QPoint( 30, 30 ) );
   dialog.exec();
}


void DragLabel::contextMenuEvent( QContextMenuEvent *event )
{
   QMenu menu( this );

   QAction *setParams = new QAction( tr("Set Parameters"), this );
   QAction *duplicate = new QAction( tr("Duplicate"), this );
   QAction *destroy   = new QAction( tr("Delete"), this );
   menu.addAction( setParams );
   menu.addAction( duplicate );
   menu.addAction( destroy );
   QAction *selected = menu.exec( event->globalPos() );
   if( selected == setParams )
   {
      parameterMenu( event->globalPos() );
   }
   else if( selected == duplicate )
   {
      emit requestCopy( toByteArray(), pos() );
   }
   else if( selected == destroy )
   {
      deleteLater();
   }
}


QSize DragLabel::gridSize() const
{
   return mSize;
}


void DragLabel::setGridSize( const QSize &size )
{
   mSize = size;
   draw();
}


void DragLabel::draw()
{
   QFont font( mpParent->gridStyle().font() );
   font.setStyleStrategy( QFont::ForceOutline );

   if( mSize.width() <= 0 )
   {
      mSize.setWidth( 1 );
   }
   if( mSize.height() <= 0 )
   {
      mSize.setHeight( 1 );
   }
   QFontMetrics metric( font );
   QString text( mpParent->gridStyle().toUpper() ? mText.toUpper() : mText );
   QSize size = metric.size( Qt::TextSingleLine, text );

   int imgWidth = 0;
   int imgHeight = 0;
   bool rotate = false;
   QSize grid( mpParent->gridSize() );

   if( (mSize.width() * grid.width() - 3) >= (mSize.height() * grid.height() - 3) )
   {
      imgWidth  = (mSize.width() * grid.width() - 3);
      imgHeight = (mSize.height() * grid.height() - 3);
      rotate = false;
   }
   else
   {
      imgHeight = (mSize.width() * grid.width() - 3);
      imgWidth  = (mSize.height() * grid.height() - 3);
      rotate = true;
   }
   QImage image( imgWidth, imgHeight, QImage::Format_ARGB32_Premultiplied );
   setSizeIncrement( grid );
   image.fill(qRgba(0, 0, 0, 0));

   QPainter painter;
   painter.begin( &image );
   painter.setRenderHints( QPainter::Antialiasing |
                           QPainter::TextAntialiasing |
                           QPainter::QPainter::HighQualityAntialiasing, false );
   painter.setBrush( mColor );
   painter.drawRect( QRectF(0.5, 0.5, image.width()-1, image.height()-1) );

   painter.setFont( font );

   qreal bright = 0.299 * mColor.redF() + 0.587 * mColor.greenF() + 0.114 * mColor.blueF();
   painter.setPen( bright >= 0.5 ? QColor(0,0,0) : QColor(255,255,255) );
   painter.drawText( QRect(mpParent->gridStyle().topLeft(), size), Qt::AlignCenter, text );
   painter.end();

   if( rotate )
   {
      QTransform rot;
      rot.rotate( 90 );
      image = image.transformed( rot );
   }
   setPixmap( QPixmap::fromImage(image) );
   resize( image.width(), image.height() );
}


void DragLabel::writeDomAttributes( QDomElement *domElement ) const
{
   domElement->setAttribute( "text", mText );
   domElement->setAttribute( "x", pos().x() / mpParent->gridSize().width() );
   domElement->setAttribute( "y", pos().y() / mpParent->gridSize().height() );
   domElement->setAttribute( "width", mSize.width() );
   domElement->setAttribute( "height", mSize.height() );
   domElement->setAttribute( "color", mColor.name() );
}


QPoint DragLabel::readDomAttributes( const QDomElement *domElement )
{
   bool ok = true;
   int x = mpParent->gridSize().width() * domElement->attribute( "x" ).toInt( &ok );
   if( !ok ) return QPoint();
   int y = mpParent->gridSize().height() * domElement->attribute( "y" ).toInt( &ok );
   if( !ok ) return QPoint();
   int width = domElement->attribute( "width" ).toInt( &ok );
   if( !ok ) return QPoint();
   int height = domElement->attribute( "height" ).toInt( &ok );
   if( !ok ) return QPoint();
   QString text = domElement->attribute( "text");
   if( text.isNull() ) return QPoint();
   mText = text;
   mSize.setHeight( height );
   mSize.setWidth( width );
   QColor color( domElement->attribute( "color" ) );
   if( color.isValid() )
   {
      mColor = color;
   }
   draw();
   return QPoint( x, y );
}
