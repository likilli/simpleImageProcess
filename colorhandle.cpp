
#include "colorhandle.h"
#include "ui_colorhandle.h"

//======================构造函数=====================
ColorHandle::ColorHandle(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorHandle)
{
    ui->setupUi(this);

    uiWidth = this->window()->width();      // 获得当前窗口的宽度
    uiHeight = this->window()->height();    // 获得当前窗口的高度

    last_delta = 0;

    QObject::connect(ui->spinBox,SIGNAL(valueChanged(int)),this,SLOT(setValue(int))); //将spinBox的数值传向slider信号槽
    QObject::connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(setValue(int)));//将slider的数值传向spinBox信号槽
}

//==============================析构函数============================
ColorHandle::~ColorHandle()
{
    delete ui;
}

/*==============================Reload========================
 * 函数名：Reload
 * 功能：加载图片，和MainWindow类中的IamgeShow_up函数功能一样，减少代码的冗余量
 * 输入：QImage变量
 * 输出：无
 * 修改日期：2017-05-26
 *=====================================================================
 */
void ColorHandle::Reload(QImage ImageVarible)
{
    ui->Image_lb->resize(ImageVarible.width(),ImageVarible.height()); //修改标签的大小和图片的大小一致
    int ImageWidth = ImageVarible.width();      //获得图片的宽度
    int ImageHeight = ImageVarible.height();    //获得图片的高度

    if((ImageWidth<uiWidth) && (ImageHeight<uiHeight))
    {
        int widthDistance = uiWidth - ImageWidth;   //获得图片的宽度与窗口宽度的差值，将图片加载至窗口中央
        int heightDistance = uiHeight - ImageHeight;//获得图片的高度与窗口高度的插值，
        widthDistance /= 2;
        heightDistance /= 2;
        ui->Image_lb->move(widthDistance,heightDistance - 30); //将图片移动至窗口中央
    }
    ui->Image_lb->setPixmap(QPixmap::fromImage(ImageVarible));
}

/*==============================getImage========================
 * 函数名：getImage
 * 功能：将MainWindow中的图像地址，进行传递，并接受选项，进行调用相应的函数，对图片进行对应的处理
 * 输入：图片地址，图片处理的选项
 * 输出：无
 * 修改日期：2017-05-26
 *=====================================================================
 */
void ColorHandle::getImage(QString ImageRoad, int Option)
{
    ImagePath = ImageRoad;
    ImagePathBak = ImageRoad;
	ImageMedium.load(ImagePath);
    Reload(ImageMedium);

    if(Option == 1) gotoCold();
    if(Option == 2) gotoWarm();
    if(Option == 3) gotoEnhance();
    if(Option == 4) gotoMosaic();
}

void ColorHandle::setValue(int value)   //对spinbox和滑条进行绑定，
{
    ui->spinBox->setValue(value);
    ui->horizontalSlider->setSliderPosition(value);
}


void ColorHandle::gotoCold()
{
    ui->horizontalSlider->setRange(0,50);   //设置进度条的最大值和最小值，设置好范围
    ui->horizontalSlider->setSliderPosition(0); //设置滑条的默认位置为0
    QObject::connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(Cold())); //当进度条值发生变化时，修改spinbox的值，并调用Cold函数
    QObject::connect(ui->spinBox,SIGNAL(valueChanged(int)),this,SLOT(Cold()));
}

void ColorHandle::Cold()
{
    ImageMedium.load(ImagePathBak);
    delta = ui->spinBox->text().toInt();    //将spinbox中的数字转换成int型的值
    int width = ImageMedium.width();
    int height = ImageMedium.height();

    for(int y=0;y<height;y++)
    {
        for(int x=0;x<width;x++)
        {
            QColor old = QColor(ImageMedium.pixel(x,y));
            ImageMedium.setPixel(x,y,qRgb(old.red(),old.green(),old.blue()+delta)); //对图片的蓝色值进行增加，即可达到冷色调的效果
        }
    }
    Reload(ImageMedium);
}

//gotoWarm函数，功能同gotoCold
void ColorHandle::gotoWarm()
{
    ui->horizontalSlider->setRange(0,50);   //设置滑条的范围
    ui->horizontalSlider->setSliderPosition(0);
    QObject::connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(Warm()));
    QObject::connect(ui->spinBox,SIGNAL(valueChanged(int)),this,SLOT(Warm()));
}

//Warm函数，对图片进行暖色调处理
void ColorHandle::Warm()
{
    ImageMedium.load(ImagePathBak);     //加载原始图片
    delta = ui->spinBox->text().toInt();    //获得spinbox中的值
    int width = ImageMedium.width();        //获得图片的宽度
    int height = ImageMedium.height();      //获得图片的高度

    for(int y=0;y<height;y++)
    {
        for(int x=0;x<width;x++)
        {
            QColor old = QColor(ImageMedium.pixel(x,y));
            ImageMedium.setPixel(x,y,qRgb(old.red()+delta,old.green()+delta,old.blue()));   //增加图片的红色调和绿色调可以实现暖色调
        }
    }
    Reload(ImageMedium);    //加载处理过后的图片
}

//实现功能同上
void ColorHandle::gotoEnhance()
{
    ui->horizontalSlider->setRange(0,80);   //设置进度条的范围为0-80
    ui->horizontalSlider->setSliderPosition(0);     //设置进度条的初始位置
    QObject::connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(Enhance()));      //链接槽和对应的函数
    QObject::connect(ui->spinBox,SIGNAL(valueChanged(int)),this,SLOT(Enhance()));
}

void ColorHandle::Enhance()
{
    QImage *newImage = new QImage(ImageMedium.width(), ImageMedium.height(), QImage::Format_ARGB32);       //将图片进行转换，并存在另一变量中

    delta = ui->spinBox->text().toInt();    //获得spinbox中的值
    QColor oldColor, newColor;
    int h,s,l;
    for(int x=0; x<newImage->width(); x++)
    {
        for(int y=0; y<newImage->height(); y++)
        {
            oldColor = QColor(ImageMedium.pixel(x,y));

            newColor = oldColor.toHsl();
            h = newColor.hue();         //获得图片的色调
            s = newColor.saturation()+delta;     //修改图片的饱和度
            l = newColor.lightness();       //图片的亮度

            s = qBound(0, s, 255);  //we check if the new value is between 0 and 255

            newColor.setHsl(h, s, l);

            newImage->setPixel(x, y, qRgb(newColor.red(), newColor.green(), newColor.blue()));
        }
    }
    last_delta = delta;
    Reload(*newImage);
}

//gotoMosaic函数功能同上
void ColorHandle::gotoMosaic()
{
    ui->horizontalSlider->setRange(1,9);     //设置进度条的范围为1-9，
    ui->horizontalSlider->setSliderPosition(1); //设置初始范围
    QObject::connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(Mosaic()));       //链接槽函数和 马赛克函数
    QObject::connect(ui->spinBox,SIGNAL(valueChanged(int)),this,SLOT(Mosaic()));
}

void ColorHandle::Mosaic()
{
    ImageMedium.load(ImagePathBak);
    delta = ui->spinBox->text().toInt();    //获得spinbox中的值
    int width = ImageMedium.width();
    int height = ImageMedium.height();

    QRgb rgb;
    const int n = delta;//picture percent of mosaic

    for(int j=0;j<height;j+=n)
    {
        for(int i=0;i<width;i+=n)
        {
            rgb = ImageMedium.pixel(i,j);
            for(int x=0;x<n;x++)
            {
                for(int y=0;y<n;y++)
                {
                    if((i+x)<width && (j+y)<height) ImageMedium.setPixel(i+x,j+y,rgb);      //对像素进行马赛克处理
                }
            }
        }
    }
    Reload(ImageMedium);    //加载马🐴赛克后的图片
}

void ColorHandle::on_Orginal_bt_clicked()
{
    ui->spinBox->setValue(1);       //重新设置spinbox中的值
    ui->horizontalSlider->setSliderPosition(1);     //将滑条放置再左边的位置
    ImageMedium.load(ImagePathBak);     //加载图片
    Reload(ImageMedium);

}

bool ColorHandle::on_Save_bt_clicked()
{
    if(ImageMedium.isNull())
        QMessageBox::warning(this,"Warning","Cannot Save!");
    else return ImageMedium.save(QFileDialog::getSaveFileName(this,tr("Save File"),
                                                   "",
                                                     tr("Images (*.png *.bmp *.jpg)")));
    return true;
}

