#include "ui_widget.h"
#include"widget.h"
#include<QMessageBox>
#include<QApplication>
#include<QDesktopWidget>
#include<QColor>
#include<QBrush>
#include<QFileDialog>
#include<QTime>

Widget::Widget(QWidget *parent) :
        QWidget(parent), //
        ui(new Ui::Widget) //
{
    ui->setupUi(this);
    input = new InputDialog(this);
    this->setWindowIcon(QIcon(":/x.ico"));
    //No window title
    this->setWindowFlags(Qt::FramelessWindowHint);
    // 右键菜单
    m_saveAction = new QAction(tr("Save"),this);
    m_saveFullAction = new QAction(tr("Save Full Screen"),this);
    m_cancelAction = new QAction(tr("Cancel"),this);
    m_quitAction = new QAction(tr("Quit"),this);
    m_menu = new QMenu(this);
    m_menu->addAction(m_saveAction);
    m_menu->addAction(m_saveFullAction);
    m_menu->addAction(m_cancelAction);
    m_menu->addSeparator();
    m_menu->addAction(m_quitAction);

    //系统托盘
    m_systemTray = new QSystemTrayIcon(this);
    m_systemTray->setIcon(QIcon(":/x.ico"));
    m_systemTray->setToolTip(tr("cutScreen"));
    m_systemTray->setContextMenu(m_menu);
    m_systemTray->show();

    m_shortcut = new QxtGlobalShortcut(QKeySequence("Ctrl+Alt+a"), this);
    connect(m_shortcut, SIGNAL(activated()),this,SLOT(startCutScreen()));

    //取得屏幕大小，初始化 cutScreen
    cutScreen = new RCutScreen(QApplication::desktop()->size());
    resize(cutScreen->width(),cutScreen->height());

    //截图信息显示区域 背景
    infoPix = new QPixmap(200,32);
    QPainter infoP(infoPix);
    infoP.setBrush(QBrush(QColor(Qt::black),Qt::SolidPattern));
    infoP.drawRect(0,0,200,32);
    //保存全屏
    fullScreen = new QPixmap();
    bgScreen = new QPixmap();
    grabFullScreen();


    connect(m_saveAction,SIGNAL(triggered()),this,SLOT(saveScreen()));
    connect(m_saveFullAction,SIGNAL(triggered()),this,SLOT(saveFullScreen()));
    connect(m_cancelAction,SIGNAL(triggered()),this,SLOT(hide()));
    connect(m_quitAction,SIGNAL(triggered()),qApp,SLOT(quit()));
}

Widget::~Widget()
{

}

void Widget::paintEvent(QPaintEvent *)
{
    int x = cutScreen->getLeftUp().x();
    int y = cutScreen->getLeftUp().y();
    int w = cutScreen->getRightDown().x()-x;
    int h = cutScreen->getRightDown().y()-y;

    QPainter painter(this);
    painter.setPen(QPen(Qt::green,1,Qt::SolidLine));
	
    painter.drawPixmap(0,0,*bgScreen);  //画模糊背景

    if( x == -1 || y == -1 )
    {
        return;
    }

    if( w!=0 && h!=0 )
    {
        painter.drawPixmap(x,y,fullScreen->copy(x,y,w,h));  //画截取区域
    }
    painter.drawRect(x,y,w,h);  //截取区域边框
	
    //显示截取区域信息 width height
    int offsetY = 0;
    if( y < 32 )
    {
        offsetY = 32;
    }
    painter.drawPixmap(x,y-32 + offsetY,*infoPix);
    painter.drawText(x+2,y-20 + offsetY,QString("Cut Area: (%1 x %2)-(%3 x %4)").arg(x).arg(y).arg(x+w).arg(y+h));
    painter.drawText(x+2,y-6 + offsetY,QString("          (%1 x %2)").arg(w).arg(h));
}
void Widget::grabFullScreen()
{
    *fullScreen = QPixmap::grabWindow(QApplication::desktop()->winId(),0,0,cutScreen->width(),cutScreen->height());

    //设置透明度实现模糊背景
    QPixmap pix(cutScreen->width(),cutScreen->height());
    pix.fill((QColor(160,160,165,192)));
    *bgScreen = *fullScreen;
    QPainter p(bgScreen);
    p.drawPixmap(0,0,pix);
}

/*****************************************
*
*截取全屏
*
****************************************/
void Widget::saveFullScreen()
{
    fullScreen->save("fullScreen.jpg","JPG");
}
void Widget::startCutScreen()
{
    if( this->isVisible() )
    {
        setVisible(false);
    }
    else
    {
        cutScreen->clearArea();
        grabFullScreen();
        setVisible(true);
        this->activateWindow();
    }
}

/****************************************
*
*保存截取区域
*
****************************************/
void Widget::saveScreen()
{
    QFileDialog::Options options;
    QString selectedFilter;
    QString str = QTime().currentTime().toString();
	//默认文件名：hour-minute-second.jpg
    QString fileName = str.mid(0,2) + "-" + str.mid(3,2) + "-" + str.mid(6,2);
    fileName = QFileDialog::getSaveFileName(this,
                                    tr("Save picture"),
                                    fileName,
                                    tr("JPEG Files (*.jpg);;JPEG (*.jpg)"),
                                    &selectedFilter,
                                    options);
									
    int x = cutScreen->getLeftUp().x();
    int y = cutScreen->getLeftUp().y();
    int w = cutScreen->getRightDown().x()-x;
    int h = cutScreen->getRightDown().y()-y;

    fullScreen->copy(x,y,w,h).save(fileName,"JPG");

    //保存成功后退出
    qApp->quit();
}

void Widget::contextMenuEvent(QContextMenuEvent *)
{
    QPoint pos = this->cursor().pos();
    m_menu->exec(pos);
}

/****************************************
*
*按键功能
*
****************************************/
void Widget::keyReleaseEvent(QKeyEvent* )
{

}

/****************************************
*
*鼠标移动在 SELECT MOV 两种状态下
*
****************************************/
void Widget::mouseMoveEvent(QMouseEvent *e)
{
    if( e->buttons() & Qt::RightButton )
    {
        return;
    }

    if( cutScreen->getStatus()==SELECT ) // 选择区域
    {
        cutScreen->setEnd( e->pos() );
    }
    else if( cutScreen->getStatus()==MOV ) //移动所选区域
    {
        QPoint p(e->x()-movPos.x(),e->y()-movPos.y());
        cutScreen->move(p);
        movPos = e->pos();
    }
    update();
}
/****************************************
*
*按下鼠标，记录初值
*
****************************************/
void Widget::mousePressEvent(QMouseEvent *e)
{
    if( e->button() == Qt::RightButton )
    {
        return;
    }

    int status = cutScreen->getStatus();
    if( status==SELECT ) // 记录鼠标
    {
        cutScreen->setStart( e->pos() );
    }
    else if( status==MOV ) //
    {
        // 不在截图区域内，重新选择，
        if( cutScreen->isInArea(e->pos())==false )
        {
            cutScreen->setStart( e->pos() );
            cutScreen->setStatus(SELECT);
        }
		// 在截图区域，移动截图 鼠标指针成十字
        else 
        {
            movPos = e->pos();
            this->setCursor(Qt::SizeAllCursor);
        }
    }
    update();
}
/****************************************
*
*任何情况下，双击弹出 width height设置框
*
****************************************/
void Widget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if( e->button() == Qt::RightButton )
    {
        return;
    }

    if( cutScreen->getStatus()!= SET_W_H )
    {
        cutScreen->setStatus(SET_W_H);

        input->show();
        input->exec();
		
        if( input->isOk()==true ) //数据有效，改变区域大小
        {
            QPoint pos = cutScreen->getLeftUp();
            cutScreen->setStart(pos);
            cutScreen->setEnd(QPoint(pos.x()+input->getWidth(),pos.y()+input->getHeight()));
        }
		// 回到 MOV 状态
        cutScreen->setStatus(MOV);
    }
}

void Widget::mouseReleaseEvent(QMouseEvent *e)
{
    if( e->button() == Qt::RightButton )
    {
        return;
    }

    if( cutScreen->getStatus()==SELECT ) // SELECT状态下 释放鼠标
    {
        cutScreen->setStatus(MOV);     //移动、撤销
    }
    else if( cutScreen->getStatus()==MOV )  // 鼠标成正常状态
    {
        this->setCursor(Qt::ArrowCursor);
    }
}
