#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include "colorhandle.h"
#include "translationhandle.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    ColorHandle *colorhandle;
    TranslationHandle *translationhandle;

private:
    Ui::MainWindow *ui;

    int uiWidth;    //窗口宽度的变量
    int uiHeight;   //窗口宽度的变量

    bool LFflag;
    bool UDflag;

    QImage ImageMedium;     //用于加载图片的媒介，故命名为"ImageMedium"
    QImage ImageMedium_two;
    QString ImagePathRec;   //备份图片的路径，用于加载原图
    void ImageShow_up(QImage);

private slots:      //私有槽
    void OpenImageDialog();
    bool SaveImageDialog();

    void UpToDown();
    void LeftToRight();
    void Vague();
    void Gray();
    void Cold();
    void Warm();
    void Enhance();
    void Mosaic();
    void Sharpen();

    void LFTranslation();
    void UDTranslation();
    void Original();    //"原始图片"的槽函数声明
};

#endif // MAINWINDOW_H
