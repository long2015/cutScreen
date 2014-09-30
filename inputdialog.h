#ifndef DIALOG_H
#define DIALOG_H

#include<QDialog>
#include<QLabel>
#include<QPushButton>
#include<QLineEdit>

class InputDialog : public QDialog
{
    Q_OBJECT
public:
    InputDialog(QWidget* parent=0);
    int getWidth(){return width;}
    int getHeight(){return height;}

public slots:
    bool isOk();
    void get();
    void cancel();
	
private:
    QLabel *WLabel;    // width
	QLineEdit* inputW;
    QLabel *HLabel;    // height
    QLineEdit* inputH;
    QPushButton *okButton; // 确定
    QPushButton *noButton; // 取消

    int width, height; // 保存取得数据
};

#endif // DIALOG_H
