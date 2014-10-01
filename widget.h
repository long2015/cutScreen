#ifndef WIDGET_H
#define WIDGET_H

#include<QtGui>

#include<QPixmap>
#include<QPainter>
#include<QPoint>
#include<QLabel>
#include<QPixmap>
#include<QLineEdit>
#include<QAction>
#include<QMenu>
#include"cutScreen.h"
#include"inputdialog.h"
#include"qxtglobalshortcut/qxtglobalshortcut.h"

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
    void contextMenuEvent(QContextMenuEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void keyReleaseEvent(QKeyEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

    void grabFullScreen();

private slots:
    void saveScreen();  
    void saveFullScreen();
    void startCutScreen();

private:
    Ui::Widget  *ui;
    QMenu       *m_menu;        //右键菜单
    QAction     *m_saveAction;
    QAction     *m_saveFullAction;
    QAction     *m_cancelAction;
    QAction     *m_quitAction;

    QSystemTrayIcon     *m_systemTray;
    QxtGlobalShortcut   *m_shortcut;

    RCutScreen  *cutScreen;     // RCutScreen对象,记录x y w h 数据
    InputDialog *input;         // 设置 width height
    QPixmap     *infoPix;       // 截图信息显示背景
    QPixmap     *fullScreen;    // 保存全屏图像
    QPixmap     *bgScreen;      // 模糊背景图
    QPoint      movPos;         // 移动位置
};

#endif // WIDGET_H
