#include"inputdialog.h"
#include<QMessageBox>
#include<QPainter>

InputDialog::InputDialog(QWidget *parent)
    : QDialog(parent)
{
    int width = 210, height = 120;
    resize(width,height);

    // width 设置标签
    WLabel = new QLabel(this);
    WLabel->setText("Width: ");    WLabel->setGeometry(10,10,50,30);
    inputW = new QLineEdit(this);
    inputW->setGeometry(60,10,130,30);
    inputW->setFocus();

    // height 设置标签
    HLabel = new QLabel(this);
    HLabel->setText("Height: ");   HLabel->setGeometry(10,50,50,30);
    inputH = new QLineEdit(this);
    inputH->setGeometry(60,50,130,30);

    // 确定按钮
    okButton = new QPushButton(this);
    okButton->setText("OK!");
    okButton->setGeometry(10,90,75,23);

    // 取消按钮
    noButton = new QPushButton(this);
    noButton->setText("Cancel");
    noButton->setGeometry(115,90,75,23);

    connect(okButton,SIGNAL(clicked()),this,SLOT(get()));
    connect(noButton,SIGNAL(clicked()),this,SLOT(cancel()));
	
    //默认是 0
    width = 0;
    height = 0;
}
void InputDialog::get()
{
    if( inputW->text()!="" && inputH->text()!="" )
    {
        width = inputW->text().toInt();
        height = inputH->text().toInt();
        hide();
    }
    else  // 没输入数据时
    {
        QMessageBox msgBox;
         msgBox.setText("No input ");
         msgBox.exec();
     }
}
/****************************************
*
*取消时，并设值 -1 输入框清空
*
****************************************/
void InputDialog::cancel()
{
    width = -1;
    height = -1;
    inputW->setText("");
    inputH->setText("");
    hide();
}

bool InputDialog::isOk()
{
    if( inputW->text()!="" && inputH->text()!="" )
    {
        return true;
    }
    // 为 -1 说明按了取消键
    else if( width==-1 && height==-1 )
    {
        width=0;
        height=0;
        return false;
    }
    return false;
}

