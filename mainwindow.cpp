/* ==============================MainWindow=============================
 * Copyright：No
 * 修改日期：2017-06-01
 * 说明：MainWindow类、主要的函数功能的实现
 * =====================================================================
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"



/*=============================MainWindow======================
 * 函数名：MainWindow类的构造函数
 * 功能： 实现动作与相应的槽的链接、获得部分需要的参数的值
 * 输入：
 * 输出：
 * 修改日期：2017-06-01
 * =============================================================
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    LFflag = true;
    UDflag = true;

    uiWidth = this->window()->width();  //获得窗口的宽度
    uiHeight = this->window()->height();    //获得窗口的高度

    connect(ui->actionOpen,     SIGNAL(triggered(bool)), this, SLOT(OpenImageDialog()) );       //将"打开"操作和对应的槽函数进行链接
    connect(ui->actionSave,     SIGNAL(triggered(bool)), this, SLOT(SaveImageDialog()) );       //将"保存"操作和对应的槽函数进行链接
    connect(ui->actionUD,       SIGNAL(triggered(bool)), this, SLOT(UpToDown())        );       //链接"上下倒转"
    connect(ui->actionLF,       SIGNAL(triggered(bool)), this, SLOT(LeftToRight())     );       //链接"左右倒转"

    connect(ui->actionGray,     SIGNAL(triggered(bool)), this, SLOT(Gray())            );       //灰色
    connect(ui->actionCold,     SIGNAL(triggered(bool)), this, SLOT(Cold())            );       //冷色调
    connect(ui->actionWarm,     SIGNAL(triggered(bool)), this, SLOT(Warm())            );       //暖色调
    connect(ui->actionEnhan,    SIGNAL(triggered(bool)), this, SLOT(Enhance())         );       //鲜艳
    connect(ui->actionGaussian, SIGNAL(triggered(bool)), this, SLOT(Vague())           );       //高斯模糊
    connect(ui->actionMosaic,   SIGNAL(triggered(bool)), this, SLOT(Mosaic())          );       //马赛克
    connect(ui->actionSharpen,  SIGNAL(triggered(bool)), this, SLOT(Sharpen())         );       //锐化
    connect(ui->UDTranslation,  SIGNAL(triggered(bool)), this, SLOT(UDTranslation())   );       //上下平移
    connect(ui->LRTranslation,  SIGNAL(triggered(bool)), this, SLOT(LFTranslation())   );       //左右平移
    connect(ui->actionOrig,   SIGNAL(triggered(bool)), this, SLOT(Original())        );       //原始图像
}

MainWindow::~MainWindow()       //析构函数
{
    delete ui;
}

/*===============================ImageShow_up===========================
 * 函数名：ImageShow_up
 * 功能：将传递过来的变量，进行加载，并加载至窗口中央
 * 输入：QImage类
 * 输出：
 *======================================================================
*/
void MainWindow::ImageShow_up(QImage Image)     //加载图片函数，为了减少图片的冗余度，因为每次处理完图片，都要加载，故将图片的加载单独写成一个函数
{

    ui->Image_label->resize(Image.width(),Image.height());      //将标签的大小设置为图片的大小
    int ImageWidth = Image.width();     //图片的宽度
    int ImageHeight = Image.height();   //图片的高度

    if((ImageWidth<uiWidth) && (ImageHeight<uiHeight))
    {
        int widthDistance = uiWidth - ImageWidth;
        int heightDistance = uiHeight - ImageHeight;
        widthDistance /= 2;
        heightDistance /= 2;
        ui->Image_label->move(widthDistance,heightDistance);    //将图片移动至中央
    }
    else
    {
        this->window()->resize(Image.width(),Image.height());
        ui->Image_label->move(0,0);
    }
    ui->Image_label->setPixmap(QPixmap::fromImage(Image));
}

/*==============================OpenImageDialog========================
 * 函数名：OpenIamgeDialog
 * 功能：弹出窗口，选择图片，并调用图片显示函数加载图片
 * 输入：无
 * 输出：无
 * 修改日期：2017-05-22
 *=====================================================================
 */
void MainWindow::OpenImageDialog()      //打开提示框，选择图片
{
    QString ImagePath = QFileDialog::getOpenFileName(this,tr("Open File"),"",tr("Images (*.png *.jpg *.bmp)"));
    ImagePathRec = ImagePath;

    ImageMedium.load(ImagePath);
    if(!ImageMedium.load(ImagePath))
    {
        QMessageBox::information(this,"错误","未选择图片，图片打开失败！");
    }
    ImageShow_up(ImageMedium);      //加载图片
}

/*==============================SaveImageDialog========================
 * 函数名：SaveIamgeDialog
 * 功能：弹出窗口，选择路径，并将处理过后的图片进行保存
 * 输入：无
 * 输出：图片
 * 修改日期：2017-05-22
 *=====================================================================
 */
bool MainWindow::SaveImageDialog()      //保存图片的对话框
{
    if(ImageMedium.isNull())
        QMessageBox::warning(this,"Warning","Cannot Save!");
    return ImageMedium.save(QFileDialog::getSaveFileName(this,tr("Save File"),
                                                       "",
                                                         tr("Images (*.png *.bmp *.jpg)")));
}

/*==============================UpToDown========================
 * 函数名：UpToDown
 * 功能：将图片进行上下颠倒
 * 输入：无
 * 输出：无
 * 修改日期：2017-05-25
 *=====================================================================
 */
void MainWindow::UpToDown()     //上下反转
{
    int width = ImageMedium.width() - 1;

    for(int i=0;i<=width;i++)
    {
        int height = ImageMedium.height() - 1;
        for(int j=0;j<=height;j++)
        {
            QColor one = QColor(ImageMedium.pixel(i,j));        //获得此点的色彩值，
            QColor two = QColor(ImageMedium.pixel(i,height));   //获得(i,j)点以图片一半高度为轴对称的点的色彩值，
            ImageMedium.setPixelColor(i,j,two);     //交换点的色彩值
            ImageMedium.setPixelColor(i,height,one);        //交换色彩值
            height--;       //高度减小
        }
    }
    ImageShow_up(ImageMedium);      //加载图片
}

/*==============================LeftToRight========================
 * 函数名：OpenIamgeDialog
 * 功能：弹出窗口，选择图片，并调用图片显示函数加载图片
 * 输入：无
 * 输出：无
 * 修改日期：2017-05-22
 *=====================================================================
 */
void MainWindow::LeftToRight()      //左右反转
{                                   //和上下反转的思路一样，以图片的一边长度为x轴，对应的点交换RGB值
    int height = ImageMedium.height() - 1;

    for(int i=0;i<=height;i++)
    {
        int width = ImageMedium.width() - 1;
        for(int j=0;j<=width;j++)
        {
            QColor one = QColor(ImageMedium.pixel(j,i));
            QColor two = QColor(ImageMedium.pixel(width,i));
            ImageMedium.setPixelColor(j,i,two);
            ImageMedium.setPixelColor(width,i,one);
            width--;
        }
    }
    ImageShow_up(ImageMedium);  //加载处理过后的图片
}

/*==============================Vague========================
 * 函数名：Vague
 * 功能：对图片进行模糊操作
 * 输入：无
 * 输出：无
 * 修改日期：2017-05-22
 *===========================================================
 */
void MainWindow::Vague()        //模糊
{
    int width = ImageMedium.width() - 1;    //获得图片的长度，并-
    int height = ImageMedium.height() - 1;      //获得图片的高度，并减一

    for(int i=1;i<width;i++)
    {
        for(int j=1;j<height;j++)
        {
            QColor One = QColor(ImageMedium.pixel(i-1,j-1));    //获得此点周围的八个点的RGB值，并求出平均值，复制给此点，
            QColor Two = QColor(ImageMedium.pixel(i,j-1));
            QColor Three = QColor(ImageMedium.pixel(i+1,j-1));
            QColor Four = QColor(ImageMedium.pixel(i-1,j));
            QColor Five = QColor(ImageMedium.pixel(i+1,j));
            QColor Six = QColor(ImageMedium.pixel(i-1,j+1));
            QColor Seven = QColor(ImageMedium.pixel(i,j+1));
            QColor Eight = QColor(ImageMedium.pixel(i+1,j+1));

            int red = One.red() + Two.red() + Three.red() + Four.red() + Five.red() + Six.red() + Seven.red() + Eight.red();
            red /= 8;       //获得和

            int green = One.green() + Two.green() + Three.green() + Four.green() + Five.green() + Six.green() + Seven.green() + Eight.green();
            green /= 8;

            int blue = One.blue() + Two.blue() + Three.blue() + Four.blue() + Five.blue() + Six.blue() + Seven.blue() + Eight.blue();
            blue /= 8;

            ImageMedium.setPixel(i,j,qRgb(red,green,blue));
        }
    }
    ImageShow_up(ImageMedium);
}

/*==============================Gray========================
 * 函数名：Gray
 * 功能：对图片转换成灰色
 * 输入：无
 * 输出：无
 * 修改日期：2017-05-22
 *===========================================================
 */
void MainWindow::Gray()
{
    int width = ImageMedium.width();    //获得图片的宽度
    int height = ImageMedium.height();  //获得图片的高度

    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            QColor old = QColor(ImageMedium.pixel(i,j));    //获得当前点的色彩值
            int gray = old.red() + old.blue() + old.green();    //灰色值=红色+蓝色+绿色值的平均值
            gray /= 3;
            ImageMedium.setPixel(i,j,qRgb(gray,gray,gray));
        }
    }
    ImageShow_up(ImageMedium);  //加载处理过的图片
}


void MainWindow::Cold()
{
    colorhandle = new ColorHandle;
    this->close();      //当前窗口关闭
    colorhandle->show();
    colorhandle->getImage(ImagePathRec,1);      //在ColorHandle的类中，参数1是调用冷色调
    colorhandle->exec();    //执行实例化的colorhandle变量
    this->show();       //当前窗口显示
    delete colorhandle; //删除实例化的对象，释放空间
}


void MainWindow::Warm()
{
    colorhandle = new ColorHandle;
    this->close();
    colorhandle->show();
    colorhandle->getImage(ImagePathRec,2);
    colorhandle->exec();
    this->show();
    delete colorhandle;
}

void MainWindow::Enhance()
{
    colorhandle = new ColorHandle;
    this->close();
    colorhandle->show();
    colorhandle->getImage(ImagePathRec,3);
    colorhandle->exec();
    this->show();
    delete colorhandle;
}

void MainWindow::Mosaic()
{
    colorhandle = new ColorHandle;
    this->close();
    colorhandle->show();
    colorhandle->getImage(ImagePathRec,4);
    colorhandle->exec();
    this->show();
    delete colorhandle;
}


void MainWindow::Sharpen()
{
    int width = ImageMedium.width();    //获得图片的宽度
    int height = ImageMedium.height();  //获得图片的高度
    QImage newImage = QImage(width, height,QImage::Format_RGB888);  //另定义一个QImage变量，进行"双缓冲，可以加快速度"
    int window[3][3] = {0,-1,0,-1,4,-1,0,-1,0};

    for (int x=1; x<width; x++)
    {
        for(int y=1; y<height; y++)
        {
            int sumR = 0;
            int sumG = 0;
            int sumB = 0;

            //对每一个像素使用模板
            for(int m=x-1; m<= x+1; m++)
                for(int n=y-1; n<=y+1; n++)
                {
                    if(m>=0 && m<width && n<height)
                    {
                        sumR += QColor(ImageMedium.pixel(m,n)).red()*window[n-y+1][m-x+1];
                        sumG += QColor(ImageMedium.pixel(m,n)).green()*window[n-y+1][m-x+1];
                        sumB += QColor(ImageMedium.pixel(m,n)).blue()*window[n-y+1][m-x+1];
                    }
                }

            int old_r = QColor(ImageMedium.pixel(x,y)).red();
            sumR += old_r;
            sumR = qBound(0, sumR, 255);    //确保像素在0-255之间

            int old_g = QColor(ImageMedium.pixel(x,y)).green();
            sumG += old_g;
            sumG = qBound(0, sumG, 255);

            int old_b = QColor(ImageMedium.pixel(x,y)).blue();
            sumB += old_b;
            sumB = qBound(0, sumB, 255);
            newImage.setPixelColor(x,y, qRgb(sumR, sumG, sumB));
        }
    }
    ImageShow_up(newImage);
}


void MainWindow::UDTranslation()
{
    translationhandle = new TranslationHandle;
    this->close();
    translationhandle->show();
    translationhandle->getImage(ImagePathRec);
    translationhandle->exec();
    this->show();
    delete translationhandle;
}

void MainWindow::LFTranslation()
{
    translationhandle = new TranslationHandle;
    this->close();
    translationhandle->show();
    translationhandle->getImage(ImagePathRec);
    translationhandle->exec();
    this->show();
    delete translationhandle;
}

/*==============================Origianl========================
 * 函数名：Original
 * 功能：加载初始的图片
 * 输入：无
 * 输出：无
 * 修改日期：2017-05-27
 *===========================================================
 */
void MainWindow::Original()
{
    ImageMedium_two.load(ImagePathRec);
    ImageMedium = ImageMedium_two;
    ImageShow_up(ImageMedium_two);
}
