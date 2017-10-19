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
, mpSplitter( new QSplitter( Qt::Vertical, this ) )
, mpTextEdit( new QTextEdit( mpSplitter ) )
, mpDragWidget( new DragWidget( mpSplitter ) )
{
   QCommonStyle style;
   QSettings settings;
   QMenu *templateMenu( new QMenu( this ) );

   templateMenu->setObjectName( "TemplateMenu" );

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

   QFileInfoList fil( QDir(":/templates/").entryInfoList() );
   foreach( const QFileInfo &fi, fil )
   {
      action = templateMenu->addAction( fi.baseName().replace('_',' ') );
      action->setData( fi.filePath() );
      connect( action, SIGNAL(triggered()),
               mpDragWidget, SLOT(load()) );
   }

   action = new QAction( style.standardIcon( QStyle::SP_DialogOpenButton ), "Open File", this );
   action->setShortcuts( QKeySequence::Open );
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
