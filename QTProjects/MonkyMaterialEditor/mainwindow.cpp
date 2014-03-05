#include "mainwindow.h"
#include "MaterialGraphWidget.h"
#include "MoveCommand.h"
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QUndoStack>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QMessageBox>
#include "Node.h"
#include <QFileDialog>
#include "NodeType.h"
#include "./CoreEngine/FileManager.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
  , m_attributesScrollArea( nullptr )
  , m_attributeLayout( nullptr )
  , m_attributeScrollAreaWidget( nullptr )
{
    Monky::FileManager& fileManager = Monky::getFileManager();
    fileManager.initialize( "./data/", "data.zip", Monky::FileManager::DATA_ARCHIVE_THEN_FOLDER, "supersecretpassword" );

    m_undoStack = new QUndoStack( this );
    m_graph = new MaterialGraphWidget( this );

    connect(m_graph, SIGNAL(ItemMoved(Node*,QPointF)),
                 this, SLOT(ItemMoved(Node*,QPointF)));

    connect( m_graph, SIGNAL( NodeSelected(Node*) ),
             this, SLOT( NodeSelected(Node*) ) );

    CreateActions();
    CreateAttributePanel();
    CreateMenus();

    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget( m_graph );
    layout->addWidget( m_attributesScrollArea );

    QWidget* widget = new QWidget();
    widget->setLayout( layout );

    setCentralWidget(widget);
    setWindowTitle("Monky Material Editor - Unsaved*");

}
//----------------------------------------------------
MainWindow::~MainWindow()
{
    NodeType::CleanupNodeTypes();
}
//----------------------------------------------------
void MainWindow::GraphChanged( bool isInStateThatIsSaved )
{
    if( m_currentFile.isEmpty() )
        setWindowTitle( QString( "Monky Material Editor - Unsaved" ) + ( isInStateThatIsSaved ? "" : "*" ) );
    else
        setWindowTitle( "Monky Material Editor - " + m_currentFile + ( isInStateThatIsSaved ? "" : "*" ) );
}
//----------------------------------------------------
void MainWindow::DeleteNode()
{
    QList<QGraphicsItem*> list = m_graph->scene()->selectedItems();
    if( list.size() > 0 )
    {
        Node* possibleNode = reinterpret_cast<Node*>( list.first() );
        if( possibleNode )
        {
            m_graph->DeleteNode( possibleNode );
        }
    }
}
//----------------------------------------------------
void MainWindow::ItemMoved( Node* movedNode, const QPointF &moveStartPosition )
{
    m_undoStack->push( new MoveCommand( movedNode, moveStartPosition ) );
}
//----------------------------------------------------
void MainWindow::FinishedEditingAttributeEntry()
{
    Node* selectedNode = m_graph->GetSelectedNode();
    if( selectedNode )
    {
        QList< std::pair< QString, QString > >& attribs = selectedNode->Attributes();
        for( int i = 0; i < m_currentAttributeLines.size() ; ++i )
        {
            if( i < attribs.size() )
                attribs[i].second =  m_currentAttributeLines[i]->lineEdit->text();
        }
        selectedNode->AttributesUpdated();
    }
}
//----------------------------------------------------
void MainWindow::NodeSelected( Node* node )
{
    ClearAttributeEntries();
    if( node != nullptr )
    {
        m_attributesScrollArea->setStyleSheet( "QScrollArea { background-color: red; }" );
        QList< std::pair< QString, QString > >& attribs = node->Attributes();
        for( int i = 0; i < attribs.size(); ++i )
        {
           m_attributeLayout->insertWidget( -1, CreateAttributeEntry( attribs[i].first, attribs[i].second ) );
        }
    }
    else
    {
        m_attributesScrollArea->setStyleSheet( "QScrollArea { background-color: green; }" );
    }
}
//----------------------------------------------------
void MainWindow::CreateActions()
{
    m_deleteAction = new QAction( tr("&Delete"), this );
    m_deleteAction->setShortcut( tr("Del") );
    connect(m_deleteAction, SIGNAL(triggered()), this, SLOT(DeleteNode()));

    m_undoAction = m_undoStack->createUndoAction( this, tr("&Undo" ) );
    m_undoAction->setShortcuts( QKeySequence::Undo );
    m_redoAction = m_undoStack->createRedoAction( this, tr("&Redo" ) );
    m_redoAction->setShortcuts( QKeySequence::Redo );

    m_newFileAction = new QAction( tr("&New"), this );
    m_newFileAction->setShortcuts( QKeySequence::New );
    m_openAction = new QAction( tr("&Open" ), this );
    m_openAction->setShortcuts( QKeySequence::Open );
    m_saveAction = new QAction( tr("&Save" ), this );
    m_saveAction->setShortcuts( QKeySequence::Save );
    m_saveAsAction = new QAction( tr("&Save As"), this );
    m_saveAsAction->setShortcuts( QKeySequence::SaveAs );
    m_quitAction = new QAction( tr("&Quit"), this );
    m_quitAction->setShortcuts( QKeySequence::Quit );

    connect( m_newFileAction, SIGNAL(triggered()), this, SLOT(NewFileSlot()) );
    connect( m_openAction, SIGNAL(triggered()), this, SLOT(OpenFileSlot()) );
    connect( m_saveAction, SIGNAL(triggered()), this, SLOT(SaveFileSlot()) );
    connect( m_saveAsAction, SIGNAL(triggered()), this, SLOT(SaveAsFileSlot()) );
    connect( m_quitAction, SIGNAL(triggered()), this, SLOT(QuitSlot()) );

}
//----------------------------------------------------
void MainWindow::CreateAttributePanel()
{    
    m_attributesScrollArea = new QScrollArea();
    m_attributeScrollAreaWidget = new QWidget();
    m_attributeLayout = new QVBoxLayout( m_attributesScrollArea );

    m_attributesScrollArea->setMinimumSize( 1200,200 );
    m_attributeScrollAreaWidget->setMinimumHeight( 200 );
    m_attributesScrollArea->setWidgetResizable( true );

    m_attributeLayout->setAlignment( Qt::AlignTop );
    m_attributeLayout->setContentsMargins( 0, 0, 0, 0 );
    m_attributeLayout->setSpacing( 0 );


    m_attributeScrollAreaWidget->setLayout( m_attributeLayout );
    m_attributesScrollArea->setWidget( m_attributeScrollAreaWidget );

}
//----------------------------------------------------
void MainWindow::CreateMenus()
{
    m_fileMenu = menuBar()->addMenu( tr( "&File" ) );
    m_fileMenu->addAction( m_newFileAction );
    m_fileMenu->addAction( m_openAction );
    m_fileMenu->addAction( m_saveAction );
    m_fileMenu->addAction( m_saveAsAction );
    m_fileMenu->addAction( m_quitAction );

    m_editMenu = menuBar()->addMenu( tr( "&Edit" ) );
    m_editMenu->addAction( m_undoAction );
    m_editMenu->addAction( m_redoAction );
    m_editMenu->addSeparator();
    m_editMenu->addAction( m_deleteAction );

    connect(m_editMenu, SIGNAL(aboutToShow()),
                 this, SLOT(EditMenuAboutToShow()));
    connect(m_editMenu, SIGNAL(aboutToHide()),
             this, SLOT(EditMenuAboutToHide()));

}
//----------------------------------------------------
QWidget* MainWindow::CreateAttributeEntry(const QString& label , const QString &value)
{
    QWidget* widget = new QWidget( m_attributeScrollAreaWidget );
    QHBoxLayout* entryLayout = new QHBoxLayout();
    QLabel* labelWidget = new QLabel( label, widget );
    entryLayout->addWidget( labelWidget );
    QLineEdit* lineEdit = new QLineEdit( widget );
    lineEdit->setText( value );
    entryLayout->addWidget( lineEdit );
    entryLayout->setContentsMargins( 2, 1, 0, 1 );

    connect( lineEdit, SIGNAL( editingFinished() ),
             this, SLOT( FinishedEditingAttributeEntry() ) );


    widget->setLayout( entryLayout );
    widget->setMinimumHeight( 10 );

    m_currentAttributeLines.push_back( new AttributeWidgetInfo( widget, labelWidget, lineEdit ) );
    return widget;
}
//----------------------------------------------------
void MainWindow::ClearAttributeEntries()
{
    for( int i = 0; i < m_currentAttributeLines.size(); ++i )
    {
        m_attributeLayout->removeWidget( m_currentAttributeLines[i]->widget );
        delete m_currentAttributeLines[i];
    }
    m_currentAttributeLines.clear();
}
//----------------------------------------------------
void MainWindow::EditMenuAboutToShow()
{
    QList<QGraphicsItem*> list = m_graph->scene()->selectedItems();
    if( list.size() > 0 )
    {
        Node* possibleNode = reinterpret_cast<Node*>( list.first() );
        m_deleteAction->setEnabled( possibleNode && possibleNode != m_graph->GetOutputChannelsNode() );
    }
    else
        m_deleteAction->setEnabled(false);
}
//----------------------------------------------------
void MainWindow::EditMenuAboutToHide()
{
    m_deleteAction->setEnabled(true);

}
//----------------------------------------------------
void MainWindow::NewFileSlot()
{
    int button = QMessageBox::question( this, "Save?", "Would you like to save the current material?" );
    if( button == QMessageBox::Yes )
    {
        SaveAsFileSlot();
        m_graph->NewMaterial();
        setWindowTitle("Monky Material Editor - Unsaved*");
    }
    else if( button == QMessageBox::No )
    {
        m_graph->NewMaterial();
        setWindowTitle("Monky Material Editor - Unsaved*");
    }
}
//----------------------------------------------------
void MainWindow::SaveFileSlot()
{
    if( m_currentFile.isEmpty() )
    {
        QString filePath = QFileDialog::getSaveFileName( this, tr("Save Material"), "./data/materials", tr("XML (*.xml)") );
        if( !filePath.isEmpty() )
        {
            m_currentFile = filePath;
            m_graph->SaveMaterial( m_currentFile );
            setWindowTitle("Monky Material Editor - " + m_currentFile );
        }
    }
    else
    {
        m_graph->SaveMaterial( m_currentFile );
        setWindowTitle("Monky Material Editor - " + m_currentFile );
    }
}
//----------------------------------------------------
void MainWindow::SaveAsFileSlot()
{
    QString filePath = QFileDialog::getSaveFileName( this, tr("Save Material"), m_currentFile, tr("XML (*.xml)") );
    if( !filePath.isEmpty() )
    {
        m_currentFile = filePath;
        m_graph->SaveMaterial( m_currentFile );
        setWindowTitle("Monky Material Editor - " + m_currentFile );
    }
}
//----------------------------------------------------
void MainWindow::OpenFileSlot()
{
    QString filePath = QFileDialog::getOpenFileName( this, tr("Open Material"), m_currentFile, tr("XML (*.xml)") );
    if( filePath.size() > 0 )
    {
        m_currentFile = filePath;
        m_undoStack->clear();
        m_graph->OpenMaterial( m_currentFile );
        setWindowTitle("Monky Material Editor - " + m_currentFile );
    }
}
//----------------------------------------------------
void MainWindow::QuitSlot()
{
    int button = QMessageBox::question( this, "Save?", "Would you like to save the current material?", ( QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel ) );
    if( button == QMessageBox::Yes )
    {
        SaveAsFileSlot();
        close();
    }
    else if( button == QMessageBox::No )
    {
        close();
    }

}
//----------------------------------------------------
//----------------------------------------------------
MainWindow::AttributeWidgetInfo::AttributeWidgetInfo( QWidget* widget, QLabel* label, QLineEdit* le )
    :   widget( widget )
    ,   label( label )
    ,   lineEdit( le )
{}
MainWindow::AttributeWidgetInfo::~AttributeWidgetInfo()
{
    delete widget;
}
