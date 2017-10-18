/*
 * src/fridgegrid/MainWindow.cpp
 * written by Sven Oliver Moll
 *
 * distributed under the terms of the GNU General Public License (GPL)
 * available at http://www.gnu.org/licenses/gpl.html
 */

/* class declaration */
#include "MainWindow.hpp"

/* system headers */

/* Qt headers */
#include <QAction>
#include <QApplication>
#include <QCommonStyle>
#include <QDockWidget>
#include <QSettings>
#include <QSplitter>
#include <QTextEdit>
#include <QTimer>
#include <QToolBar>

/* local library headers */

/* local headers */
#include "DragWidget.hpp"


MainWindow::MainWindow( QWidget *parent, Qt::WindowFlags flags )
: QMainWindow( parent, flags )
, mpSplitter( new QSplitter( Qt::Vertical, this ) )
, mpTextEdit( new QTextEdit( mpSplitter ) )
, mpDragWidget( new DragWidget( mpSplitter ) )
{
   QCommonStyle style;
   QSettings settings;

   mpSplitter->setObjectName( "CentralSplitter" );
   mpSplitter->addWidget( mpTextEdit );
   mpSplitter->addWidget( mpDragWidget );
   mpSplitter->restoreState( settings.value( "Splitter").toByteArray() );
   setCentralWidget( mpSplitter );

   mpTextEdit->setObjectName( "CommentEditor" );
   mpTextEdit->setAcceptRichText( false );
   mpTextEdit->setText( settings.value( "Comment" ).toString() );
   commentChange();

   mpDragWidget->setObjectName( "MainArea" );
   mpDragWidget->setTextEdit( mpTextEdit );
   mpDragWidget->resize( 400, 50 );
   mpDragWidget->setData( settings.value( "Saved" ) );

   QToolBar *toolBar = 0;
   QAction  *action  = 0;

   toolBar = addToolBar( tr("File") );
   toolBar->setObjectName( "FileToolBar" );
   toolBar->setContextMenuPolicy( Qt::PreventContextMenu );

   action = new QAction( style.standardIcon( QStyle::SP_DialogOpenButton ), "Open", this );
   action->setShortcuts( QKeySequence::New );
   connect( action, SIGNAL(triggered()),
            mpDragWidget, SLOT(load()) );
   toolBar->addAction( action );

   action = new QAction( style.standardIcon( QStyle::SP_DialogSaveButton ), "Save", this );
   action->setShortcuts( QKeySequence::Open );
   connect( action, SIGNAL(triggered()),
            mpDragWidget, SLOT(save()) );
   toolBar->addAction( action );

   toolBar = addToolBar( tr("Element") );
   toolBar->setObjectName( "ElementsToolBar" );
   toolBar->setContextMenuPolicy( Qt::PreventContextMenu );

   action = new QAction( style.standardIcon( QStyle::SP_FileIcon ), "New", this );
   action->setShortcuts( QKeySequence::New );
   connect( action, SIGNAL(triggered()),
            mpDragWidget, SLOT(newElement()) );
   toolBar->addAction( action );

   restoreGeometry( settings.value( "Geometry" ).toByteArray() );
   restoreState( settings.value( "State" ).toByteArray() );
   QList<QDockWidget*> docks( findChildren<QDockWidget*>() );
   foreach( QDockWidget *dock, docks )
   {
      restoreDockWidget( dock );
   }

   QTimer *updateDelay = new QTimer( this );
   updateDelay->setInterval( 500 );
   updateDelay->setSingleShot( true );
   connect( mpTextEdit, SIGNAL(textChanged()),
            updateDelay, SLOT(start()) );
   connect( updateDelay, SIGNAL(timeout()),
            this, SLOT(commentChange()) );
}


MainWindow::~MainWindow()
{
   QSettings settings;
   settings.setValue( "Geometry", saveGeometry() );
   settings.setValue( "State", saveState() );
   settings.setValue( "Splitter", mpSplitter->saveState() );
   settings.setValue( "Comment", mpTextEdit->toPlainText() );
   settings.setValue( "Saved", mpDragWidget->getData() );
}


void MainWindow::commentChange()
{
   const QStringList &comment( mpTextEdit->toPlainText().split('\n', QString::SkipEmptyParts) );
   if( comment.size() > 0 )
   {
      setWindowTitle( QString( "%1 (%2)").arg( comment.at(0),
                                               QCoreApplication::applicationName() ) );
   }
   else
   {
      setWindowTitle( QCoreApplication::applicationName() );
   }
}
