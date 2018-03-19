#ifndef TRANSLATIONHANDLE_H
#define TRANSLATIONHANDLE_H

#include <QDialog>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QSlider>
#include <QSpinBox>
#include <QMessageBox>
#include <QDebug>
#include <cmath>

namespace Ui {
class TranslationHandle;
}

class TranslationHandle : public QDialog
{
    Q_OBJECT

public:
    explicit TranslationHandle(QWidget *parent = 0);
    ~TranslationHandle();

    void getImage(QString);

private:
    Ui::TranslationHandle *ui;

    int uiWidth;    //窗口宽度的变量的声明
    int uiHeight;   //窗口宽度的变量的声明
    int width;
    int height;
    int horizontal_r;   //用于获取横向spinbox中的值的变量
    int vertical_r;     //用于获取纵向spinbox中的值的变量

    int latestVerticalChange;
    int latestHorizontalChange;

    QString ImagePath;
    QString ImagePathBak;   //备份图片的路径，用于加载原图
    QImage ImageMedium;     //用于加载图片的媒介，故命名为"ImageMedium"
    QImage ImageMediumRec;

    void Reload(QImage);

private slots:       //私有槽

    void on_Original_bt_clicked();  //"原始图片"的槽函数声明
    bool on_Save_bt_clicked();      //"储存图片"的槽函数声明

    void horizontal_setValue(int);
    void vertical_setValue(int);

    void horizontalTranslation();
    void verticalTranslation();

};

#endif // TRANSLATIONHANDLE_H
