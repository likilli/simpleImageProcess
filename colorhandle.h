#ifndef COLORHANDLE_H
#define COLORHANDLE_H

#include <QDialog>
#include <QImage>
#include <QMessageBox>
#include <QPixmap>
#include <QColor>
#include <QFileDialog>

namespace Ui {
class ColorHandle;
}

class ColorHandle : public QDialog
{
    Q_OBJECT

public:
    explicit ColorHandle(QWidget *parent = 0);
    ~ColorHandle();

    void getImage(QString, int);

private slots:      //私有槽
    bool on_Save_bt_clicked();      //"储存图片"的槽函数声明
    void on_Orginal_bt_clicked();   //"原始图片"的槽函数声明

private:        //私有变量
    Ui::ColorHandle *ui;

    int delta;      //程度值，一般从spinbox中获取
    int uiWidth;    //窗口宽度的变量
    int uiHeight;   //窗口高度的变量
    int last_delta;
    QImage ImageMedium;     //用于加载图片的媒介，故命名为"ImageMedium"
    QString ImagePath;
    QString ImagePathBak;   //备份图片的路径，用于加载原图

    void Reload(QImage);
    void gotoCold();
    void gotoWarm();
    void gotoEnhance();
    void gotoMosaic();

private slots:      //私有槽
    void setValue(int);

    void Cold();
    void Warm();
    void Enhance();
    void Mosaic();
};

#endif // COLORHANDLE_H
