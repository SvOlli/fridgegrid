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
#include <QDir>
#include <QFileInfo>
#include <QDockWidget>
#include <QMenu>
#include <QSettings>
#include <QSignalMapper>
#include <QSplitter>
#include <QTextEdit>
#include <QTimer>
#include <QToolBar>
#include <QToolButton>

/* local library headers */

/* local headers */
#include "DragWidget.hpp"

#include <QtDebug>

MainWindow::MainWindow( QWidget *parent, Qt::WindowFlags flags )
: QMainWindow( parent, flags )
, mpLastFilesMenu( new QMenu( this ) )
, mpLastFilesMapper( new QSignalMapper( this ) )
, mpSplitter( new QSplitter( Qt::Vertical, this ) )
, mpTextEdit( new QTextEdit( mpSplitter ) )
, mpDragWidget( new DragWidget( mpSplitter ) )
, mLastFilenames()
{
   QCommonStyle style;
   QSettings settings;
   QMenu *templateMenu( new QMenu( this ) );
   mLastFilenames = settings.value( "LastUsed" ).toStringList();
   mpLastFilesMenu->setToolTipsVisible( true );

   connect( mpLastFilesMapper, SIGNAL(mapped(QString)),
            mpDragWidget, SLOT(load(QString)) );
   connect( mpDragWidget, SIGNAL(fileUsed(QString)),
            this, SLOT(fileUsed(QString)) );

   templateMenu->setObjectName( "TemplateMenu" );
   mpLastFilesMenu->setObjectName( "LastFileMenu" );

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

   action = new QAction( style.standardIcon( QStyle::SP_FileDialogNewFolder ), "New Grid", this );
   action->setShortcuts( QKeySequence::New );
   action->setMenu( templateMenu );
   action->setVisible(true);
   toolBar->addAction( action );

   // just want to tell the associated button, that we always want the menu list
   // can't this be done easier?
   {
      QList<QWidget*> wl( action->associatedWidgets() );
      foreach(QWidget *w, wl)
      {
         QToolButton *b = qobject_cast<QToolButton*>(w);
         if( b )
         {
            b->setPopupMode( QToolButton::InstantPopup );
         }
      }
   }

   QSignalMapper *signalMapper = new QSignalMapper( this );
   connect( signalMapper, SIGNAL(mapped(QString)),
            mpDragWidget, SLOT(load(QString)) );
   QFileInfoList fil( QDir(":/templates/").entryInfoList() );
   foreach( const QFileInfo &fi, fil )
   {
      action = templateMenu->addAction( fi.baseName().replace('_',' ') );
      action->setData( fi.filePath() );
      connect( action, SIGNAL(triggered()),
               signalMapper, SLOT(map()));
      signalMapper->setMapping( action, fi.filePath() );
   }

   action = new QAction( style.standardIcon( QStyle::SP_DialogOpenButton ), "Open File", this );
   action->setShortcuts( QKeySequence::Open );
   action->setMenu( mpLastFilesMenu );
   connect( action, SIGNAL(triggered()),
            mpDragWidget, SLOT(load()) );
   toolBar->addAction( action );

   action = new QAction( style.standardIcon( QStyle::SP_DialogSaveButton ), "Save File", this );
   action->setShortcuts( QKeySequence::Save );
   connect( action, SIGNAL(triggered()),
            mpDragWidget, SLOT(save()) );
   toolBar->addAction( action );

   action = new QAction( style.standardIcon( QStyle::SP_CommandLink ), "Export To PNG", this );
   action->setShortcuts( QKeySequence::SaveAs );
   connect( action, SIGNAL(triggered()),
            mpDragWidget, SLOT(exportPng()) );
   toolBar->addAction( action );

   toolBar = addToolBar( tr("Element") );
   toolBar->setObjectName( "ElementsToolBar" );
   toolBar->setContextMenuPolicy( Qt::PreventContextMenu );

   action = new QAction( style.standardIcon( QStyle::SP_FileIcon ), "New Element", this );
   action->setShortcuts( QKeySequence::AddTab );
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

   fileUsed();
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


void MainWindow::fileUsed( const QString &fileName )
{
   QSettings settings;

   if( !fileName.isEmpty() )
   {
      mLastFilenames.removeAll( fileName );
      mLastFilenames.prepend( fileName );
      while( mLastFilenames.size() > 20 ) /* \todo TODO: make configurable */
      {
         mLastFilenames.takeLast();
      }
      settings.setValue( "LastUsed", mLastFilenames );
   }

   mpLastFilesMenu->clear();
   foreach ( const QString &fileName, mLastFilenames ) {
      QAction *entry = mpLastFilesMenu->addAction( fileName.mid(fileName.lastIndexOf("/")+1) );
      entry->setToolTip( fileName );
      connect( entry, SIGNAL(triggered()),
               mpLastFilesMapper, SLOT(map()));
      mpLastFilesMapper->setMapping( entry, fileName );
   }
}
