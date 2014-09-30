#ifndef CUTSCREEN_H
#define CUTSCREEN_H

#include<QPoint>
#include<QSize>

enum STATUS{SELECT,MOV,SET_W_H};

class RCutScreen
{
public:
    RCutScreen() {}
    RCutScreen(QSize size);

	
    void setStart(QPoint pos);
    void setEnd(QPoint pos);
    QPoint getStart();
    QPoint getEnd();
    
	
	QPoint getLeftUp();
    QPoint getRightDown();
	
    STATUS getStatus();
    void setStatus(STATUS st);

    int width();
    int height();
    bool isInArea(QPoint pos); // 检测pos是否在截图区域内
    void move(QPoint p);       // 按 p 移动截图区域

private:
    QPoint leftUpPos, rightDownPos;     // 记录 截图区域 左上角、右下角
    QPoint startPos, endPos;            //记录 鼠标开始位置、结束位置
    int maxWidth, maxHeight;            //记录屏幕大小
    // 三个状态 :  选择区域、移动区域、设置width height
    STATUS status;

    void cmpPoint(QPoint &s, QPoint &e); // 比较两位置，判断左上角、右下角
};

#endif // CUTSCREEN_H
