#ifndef WIDGET_H
#define WIDGET_H

#include<QtGui>

#include<QPixmap>
#include<QPainter>
#include<QPoint>
#include<QLabel>
#include<QPixmap>
#include<QLineEdit>
#include"cutScreen.h"
#include"inputdialog.h"

namespace Ui {
    class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

private:
    void saveScreen();  
    void saveFullScreen();

private:
    Ui::Widget *ui;

    RCutScreen *cutScreen;      // RCutScreen对象,记录x y w h 数据
    InputDialog *input;         // 设置 width height
        QPixmap *infoPix;       // 截图信息显示背景
    QPixmap *fullScreen;        // 保存全屏图像
    QPixmap *bgScreen;          // 模糊背景图
    QPoint movPos;              // 移动位置
};

#endif // WIDGET_H
