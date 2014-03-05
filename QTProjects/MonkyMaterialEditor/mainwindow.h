#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QMap>

QT_BEGIN_NAMESPACE
class QAction;
class QUndoStack;
class QMenu;
class QScrollArea;
class QLineEdit;
class QVBoxLayout;
class QHBoxLayout;
class QLabel;
QT_END_NAMESPACE

class MaterialGraphWidget;
class Node;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QUndoStack* UndoStack() const { return m_undoStack; }

    void GraphChanged( bool isInStateThatIsSaved );

signals:

public slots:
    void DeleteNode();
    void ItemMoved( Node* movedNode, const QPointF &moveStartPosition );
    void FinishedEditingAttributeEntry();
    void NodeSelected( Node* node );

protected:
    void CreateActions();
    void CreateAttributePanel();
    void CreateMenus();

    QWidget* CreateAttributeEntry( const QString& label, const QString& value );
    void ClearAttributeEntries();

private slots:

    void EditMenuAboutToShow();
    void EditMenuAboutToHide();

    void NewFileSlot();
    void SaveFileSlot();
    void SaveAsFileSlot();
    void OpenFileSlot();
    void QuitSlot();

private:

    struct AttributeWidgetInfo
    {
        AttributeWidgetInfo( QWidget* widget, QLabel* label, QLineEdit* le );
        ~AttributeWidgetInfo();
        QWidget* widget;
        QLabel* label;
        QLineEdit* lineEdit;
    };

    QList< AttributeWidgetInfo* > m_currentAttributeLines;

    MaterialGraphWidget* m_graph;
    QUndoStack* m_undoStack;

    QScrollArea* m_attributesScrollArea;
    QVBoxLayout* m_attributeLayout;
    QWidget* m_attributeScrollAreaWidget;

    QAction* m_undoAction;
    QAction* m_redoAction;
    QAction* m_deleteAction;
    QAction* m_newFileAction;
    QAction* m_openAction;
    QAction* m_saveAction;
    QAction* m_saveAsAction;
    QAction* m_quitAction;

    QMenu* m_fileMenu;
    QMenu* m_editMenu;

    QString m_currentFile;

};

#endif // MAINWINDOW_H
