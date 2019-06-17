#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTime>
#include <QGraphicsEllipseItem>
#include <QColor>
#include <QColorDialog>
#include <QFontDialog>
#include <QKeyEvent>

template<class T>
void setBrushColor(T * item)
{
    //模板函数
    QColor color = item->brush().color();
    color = QColorDialog::getColor(color, nullptr, "选择填充颜色");
    if(color.isValid()){
        item->setBrush(QBrush(color));
    }
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //创建状态栏标签
    labViewCord = new QLabel("View 坐标：");
    labViewCord->setMinimumWidth(150);
    this->ui->statusBar->addWidget(labViewCord);

    labSceneCord = new QLabel("Scene 坐标：");
    labSceneCord->setMinimumWidth(150);
    this->ui->statusBar->addWidget(labSceneCord);

    labItemCord = new QLabel("Item 坐标：");
    labItemCord->setMinimumWidth(150);
    this->ui->statusBar->addWidget(labItemCord);

    labItemInfo = new QLabel("ItemInfo：");
    labItemInfo->setMinimumWidth(200);
    this->ui->statusBar->addWidget(labItemInfo);

    //创建QGraphicsScene
    scene = new QGraphicsScene(-300, -200, 600, 200);
    this->ui->view->setScene(scene);
    this->ui->view->setCursor(Qt::CrossCursor);//设置鼠标光标形状
    this->ui->view->setMouseTracking(true);
    this->ui->view->setDragMode(QGraphicsView::RubberBandDrag);
    this->setCentralWidget(this->ui->view);

    QObject::connect(this->ui->view, SIGNAL(mouseMovePoint(QPoint)), this, SLOT(on_mouseMovePoint(QPoint)));
    QObject::connect(this->ui->view, SIGNAL(mouseClicked(QPoint)), this, SLOT(on_mouseClicked(QPoint)));
    QObject::connect(this->ui->view, SIGNAL(mouseDoubleClick(QPoint)), this, SLOT(on_mouseDoubleClick(QPoint)));
    QObject::connect(this->ui->view, SIGNAL(keyPress(QKeyEvent *)), this, SLOT(on_keyPress(QKeyEvent *)));

    qsrand(QTime::currentTime().second());//随机数初始化


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_mouseMovePoint(QPoint point)
{
    //鼠标移动事件,Point是GraphicsView的坐标，物理坐标
    labViewCord->setText(QString::asprintf("View 坐标：%d, %d", point.x(), point.y()));
    QPointF pointScene = this->ui->view->mapToScene(point);//转换到scene坐标
    labSceneCord->setText(QString::asprintf("Scene 坐标：%.0f, %.0f", pointScene.x(), pointScene.y()));

}

void MainWindow::on_mouseClicked(QPoint point)
{
    //鼠标单击事件
    QPointF pointScene = this->ui->view->mapToScene(point);//转换到Scene坐标
    QGraphicsItem * item = nullptr;
    item = scene->itemAt(pointScene, this->ui->view->transform());//光标下的图形项
    if(item != nullptr){
        //有图形项
        QPointF pointItem = item->mapFromScene(pointScene);//图形项局部坐标
        labItemCord->setText(QString::asprintf("Item 坐标:%.0f, %.0f", pointItem.x(), pointItem.y()));
        labItemInfo->setText(item->data(ItemDescription).toString() + ", ItemId = " + item->data(ItemId).toString());
    }
}

void MainWindow::on_mouseDoubleClick(QPoint point)
{
    //鼠标双击,调用相应的对话框,设置填充颜色，线条颜色和字体
    QPointF pointScene = this->ui->view->mapToScene(point);//转换到scene坐标
    QGraphicsItem * item = nullptr;
    item = scene->itemAt(pointScene, this->ui->view->transform());//光标下的图形项
    if(item == nullptr){
        return;//没有图形项
    }

    switch(item->type()){
    case QGraphicsRectItem::Type://矩形框
    {
        //强制类型转换
        QGraphicsRectItem * theItem = qgraphicsitem_cast<QGraphicsRectItem *>(item);
        setBrushColor(theItem);
        break;
    }
    case QGraphicsEllipseItem::Type://椭圆和圆都是此类型
    {
        QGraphicsEllipseItem * theItem = qgraphicsitem_cast<QGraphicsEllipseItem *>(item);
        setBrushColor(theItem);
        break;
    }
    case QGraphicsPolygonItem::Type://梯形和三角形
    {
        QGraphicsPolygonItem * theItem = qgraphicsitem_cast<QGraphicsPolygonItem *>(item);
        setBrushColor(theItem);
        break;
    }
    case QGraphicsLineItem::Type://直线，设置线条颜色
    {
        QGraphicsLineItem * theItem;
        theItem = qgraphicsitem_cast<QGraphicsLineItem *>(item);
        QPen pen = theItem->pen();
        QColor color = theItem->pen().color();
        color = QColorDialog::getColor(color, this, "选择线条颜色");
        if(color.isValid()){
            pen.setColor(color);
            theItem->setPen(pen);
        }
        break;
    }
    case QGraphicsTextItem::Type://文字，设置字体
    {
        QGraphicsTextItem * theItem;
        theItem = qgraphicsitem_cast<QGraphicsTextItem *>(item);
        QFont font = theItem->font();
        bool ok = false;
        font = QFontDialog::getFont(&ok, font, this, "设置字体");
        if(ok){
            theItem->setFont(font);
        }
        break;
    }

    }

}

void MainWindow::on_keyPress(QKeyEvent *event)
{
    //按键事件,只对只有一个图形项被选中时才可以执行键盘操作
    if(scene->selectedItems().count() != 1){
        return;//没有选中的图形项，或选中的多于一个
    }
    QGraphicsItem * item = scene->selectedItems().at(0);
    if(event->key() == Qt::Key_Delete){
        //删除
        scene->removeItem(item);
    }else if(event->key() == Qt::Key_Space){
        //顺时针旋转90'
        item->setRotation(90 + item->rotation());
    }else if(event->key() == Qt::Key_PageUp){
        //放大
        item->setScale(0.1 + item->scale());
    }else if(event->key() == Qt::Key_PageDown){
        //缩小
        item->setScale(-0.1 + item->scale());
    }else if(event->key() == Qt::Key_Left){
        //左移
        item->setX(-1 + item->x());
    }else if(event->key() == Qt::Key_Right){
        //右移
        item->setX(1 + item->x());
    }else if(event->key() == Qt::Key_Up){
        //上移
        item->setY(-1+item->y());
    }else if(event->key() == Qt::Key_Down){
        //下移
        item->setY(1 + item->y());
    }

}

void MainWindow::on_actItem_Ellipse_triggered()
{
    //添加一个椭圆
    QGraphicsEllipseItem * item = new QGraphicsEllipseItem(-50, -30, 100, 60);
    item->setFlags(QGraphicsItem::ItemIsMovable |
                     QGraphicsItem::ItemIsSelectable |
                     QGraphicsItem::ItemIsFocusable);
    item->setBrush(Qt::blue);//填充颜色
    item->setZValue(++frontZ);//用于叠放顺序
    item->setPos(-50 + (qrand() % 100), -50 + (qrand() % 100) );//初始位置
    item->setData(ItemId, ++seqNum);//自定义数据,ItemId键
    item->setData(ItemDescription, "椭圆");//自定义数据，ItemDesciption键

    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}

void MainWindow::on_actZoomIn_triggered()
{
    //放大
    int cnt = this->scene->selectedItems().count();
    if(cnt == 1){
        QGraphicsItem * item = scene->selectedItems().at(0);
        item->setScale(0.1 + item->scale());
    }else{
        this->ui->view->scale(1.1, 1.1);
    }
}

void MainWindow::on_actRotateLeft_triggered()
{
    //逆时针旋转
    int cnt = scene->selectedItems().count();
    if(cnt == 1){
        QGraphicsItem * item = scene->selectedItems().at(0);
        item->resetTransform();
    }else {
        this->ui->view->resetTransform();
    }
}

void MainWindow::on_actEdit_Front_triggered()
{
    //bring to front,前置
    int cnt = scene->selectedItems().count();
    if(cnt > 0){
        //只处理选中的第一个图形项
        QGraphicsItem * item = scene->selectedItems().at(0);
        item->setZValue(++frontZ);
    }
}

void MainWindow::on_actEdit_Back_triggered()
{
    //bring to back,后置
    int cnt = scene->selectedItems().count();
    if(cnt > 0){
        //只处理选中的第一个图形选项
        QGraphicsItem * item = scene->selectedItems().at(0);
        item->setZValue(--backZ);
    }
}

void MainWindow::on_actGroup_triggered()
{
    //组合
    int cnt = scene->selectedItems().count();
    if(cnt > 1){
        QGraphicsItemGroup * group = new QGraphicsItemGroup;//创建组合
        scene->addItem(group);//组合添加到场景中


        for(int i = 0; i < cnt; ++i){
            QGraphicsItem * item = scene->selectedItems().at(0);
            item->setSelected(false);//清除选择虚线框
            item->clearFocus();
            group->addToGroup(item);//添加到组合
        }

        group->setFlags(QGraphicsItem::ItemIsMovable |
                            QGraphicsItem::ItemIsSelectable |
                            QGraphicsItem::ItemIsFocusable);

        group->setZValue(++frontZ);

        scene->clearSelection();

        group->setSelected(true);
    }
}

void MainWindow::on_actGroupBreak_triggered()
{
    //删除所有选中的图形项
    int cnt = scene->selectedItems().count();
    if(cnt == 1){
        //没有作类型判断
        QGraphicsItemGroup * group = (QGraphicsItemGroup *)scene->selectedItems().at(0);
        scene->destroyItemGroup(group);
    }
}

void MainWindow::on_actEdit_Delete_triggered()
{
    //删除所有选中的图形项
    int cnt = scene->selectedItems().count();
    if(cnt > 0){
        for(int i = 0; i < cnt; ++i){
            QGraphicsItem * item = scene->selectedItems().at(0);
            scene->removeItem(item);//删除图形项
        }
    }
}

void MainWindow::on_actItem_Rect_triggered()
{
    //添加一个矩形
    QGraphicsRectItem * item = new QGraphicsRectItem(-50, -30, 100, 60);
    item->setFlags(QGraphicsItem::ItemIsMovable |
                       QGraphicsItem::ItemIsMovable |
                        QGraphicsItem::ItemIsSelectable |
                           QGraphicsItem::ItemIsFocusable);
    item->setBrush(QBrush(Qt::yellow));//填充颜色
    item->setZValue(++frontZ);//用于叠放顺序
    item->setPos(-50 + (qrand() % 100), -50 + (qrand() % 100));//初始位置
    item->setData(ItemId, ++seqNum);//自定义数据，ItemId键
    item->setData(ItemDescription, "椭圆");//自定义数据，ItemDescription键

    scene->addItem(item);
    scene->clearSelection();
    item->setSelected(true);
}
