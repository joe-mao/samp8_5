#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QLabel>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    static const int ItemId = 1;//图形项自定义数据的key
    static const int ItemDescription = 2;//图形项自定义数据的key
    int seqNum = 0;//用于图形项的编号，每个图形项有一个编号
    int frontZ = 0;//用于bring to front
    int backZ = 0;//用于bring to back
    QGraphicsScene * scene;
    QLabel * labViewCord;
    QLabel * labSceneCord;
    QLabel * labItemCord;
    QLabel * labItemInfo;

private slots:
    void on_mouseMovePoint(QPoint point);//鼠标移动
    void on_mouseClicked(QPoint point);//鼠标单击
    void on_mouseDoubleClick(QPoint point);//鼠标双击
    void on_keyPress(QKeyEvent * event);//按键

    void on_actItem_Ellipse_triggered();

    void on_actZoomIn_triggered();

    void on_actRotateLeft_triggered();

    void on_actEdit_Front_triggered();

    void on_actEdit_Back_triggered();

    void on_actGroup_triggered();

    void on_actGroupBreak_triggered();

    void on_actEdit_Delete_triggered();

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
