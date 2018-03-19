
#include "translationhandle.h"
#include "ui_translationhandle.h"



TranslationHandle::TranslationHandle(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TranslationHandle)
{
    ui->setupUi(this);
    latestVerticalChange = 0;
    latestHorizontalChange = 0;

    uiWidth = this->window()->width();  //获得窗口的宽度
    uiHeight = this->window()->height();     //获得窗口的高度

    //横向滑条和用于左右平移的spinbox进行绑定
    QObject::connect(ui->horizontalspinBox,SIGNAL(valueChanged(int)),this,SLOT(horizontal_setValue(int))); //将spinBox的数值传向slider信号槽
    QObject::connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(horizontal_setValue(int)));//将slider的数值传向spinBox信号槽
    //纵向滑条和用于上下平移的spinbox进行绑定
    QObject::connect(ui->verticalspinBox,SIGNAL(valueChanged(int)),this,SLOT(vertical_setValue(int))); //将spinBox的数值传向slider信号槽
    QObject::connect(ui->verticalSlider,SIGNAL(valueChanged(int)),this,SLOT(vertical_setValue(int)));//将slider的数值传向spinBox信号槽
}

//TranslationHandle类的析构函数，用于释放内存
TranslationHandle::~TranslationHandle()
{
    delete ui;
}


void TranslationHandle::Reload(QImage ImageVarible)
{
    ui->Image_lb->resize(ImageVarible.width(),ImageVarible.height());
    int ImageWidth = ImageVarible.width();      //获得图片的宽度
    int ImageHeight = ImageVarible.height();    //获得图片的高度

    if((ImageWidth<uiWidth) && (ImageHeight<uiHeight))
    {
        int widthDistance = uiWidth-ImageWidth;     //获得窗口宽度和图片宽度的差值
        int heightDistance = uiHeight-ImageHeight;  //获得窗口高度和图片高低的插值
        widthDistance /= 2;
        heightDistance /= 2;
        ui->Image_lb->move(widthDistance,heightDistance);       //将图片移动值窗口中央
    }
    ui->Image_lb->setPixmap(QPixmap::fromImage(ImageVarible));
}


void TranslationHandle::getImage(QString ImageRoad)
{
	ImagePath = ImageRoad;
	ImagePathBak = ImagePath;
	ImageMedium.load(ImageRoad);
    Reload(ImageMedium);        //先加载为处理的图片

    width = ImageMedium.width();        //图片的高度
    height = ImageMedium.height();      //图片的宽度

    ui->horizontalSlider->setRange(0,width*2);
    ui->horizontalspinBox->setRange(0,width*2);
    ui->horizontalSlider->setSliderPosition(width);
    ui->horizontalspinBox->setValue(width);

    ui->verticalSlider->setRange(0,height*2);
    ui->verticalSlider->setSliderPosition(height);
    ui->verticalspinBox->setRange(0,height*2);
    ui->verticalspinBox->setValue(height);

	QObject::connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(horizontalTranslation()));
    QObject::connect(ui->horizontalSlider,SIGNAL(valueChanged(int)),this,SLOT(horizontalTranslation()));
    QObject::connect(ui->verticalSlider,SIGNAL(valueChanged(int)),this,SLOT(verticalTranslation()));
    QObject::connect(ui->verticalSlider,SIGNAL(valueChanged(int)),this,SLOT(verticalTranslation()));
}

/*==============================horizontal_setValue========================
 * 函数名：horizontal_setValue
 * 功能：当横向滑条滑动或spinbox中的值变化时，横向滑条和spinbox的值共同变化，
 * 输入：整形的 横向滑条的进度值或spinbox的值
 * 输出：无
 * 修改日期：2017-05-26
 *=====================================================================
 */
void TranslationHandle::horizontal_setValue(int value)
{
    ui->horizontalspinBox->setValue(value);
    ui->horizontalSlider->setSliderPosition(value);
}

/*==============================vertical_setValue========================
 * 函数名：vertical_setValue
 * 功能：当纵向滑条滑动或spinbox中的值变化是，纵向的滑条和spinbox的值共同变化
 * 输入：整形的 纵向的滑条的进度值或spinbox的值
 * 输出：无
 * 修改日期：2017-05-26
 *=====================================================================
 */
void TranslationHandle::vertical_setValue(int value)
{
    ui->verticalspinBox->setValue(value);
    ui->verticalSlider->setSliderPosition(value);
}


void TranslationHandle::horizontalTranslation()
{
    horizontal_r = ui->horizontalspinBox->text().toInt();   //获得即将便宜的量
    if(horizontal_r>width)	//偏移的量的值大于图片的宽度时，代表向右偏移
    {
        int TranslationValue = horizontal_r - width;	//求出向右的偏移量
    	for(int y=0;y<height;y++)
    	{
            for(int x=width-1;x>=TranslationValue;x--)
    		{
                QColor old = QColor(ImageMedium.pixel(x-TranslationValue,y));
                ImageMedium.setPixelColor(x,y,old);
    		}
    	}
        for(int y=0;y<height;y++)
            for(int x=0;x<TranslationValue;x++)
                ImageMedium.setPixel(x,y,qRgb(255,255,255));   //对剩余的地方处理，变为白色
    }
    if(horizontal_r<width)	//当便宜的量的值为小于图片的宽度时，认定为向左便宜
    {
        int TranslationValue = width - horizontal_r;	//求出向左的偏移量
        for(int y=0;y<height;y++)
        {
        	for(int x=0;x<width-TranslationValue;x++)
        	{
                QColor old = QColor(ImageMedium.pixel(x+TranslationValue,y));       //向左移动图片
        		ImageMedium.setPixelColor(x,y,old);
        	}
        }
        for(int y=0;y<height;y++)
            for(int x=width-TranslationValue;x<width;x++)
                ImageMedium.setPixel(x,y,qRgb(255,255,255));        //对剩余的地方进行处理，使其变为白色
    }
    ImageMediumRec = ImageMedium;
    Reload(ImageMedium);        //加载处理过后的图片
}


void TranslationHandle::verticalTranslation()
{
    vertical_r = ui->verticalspinBox->text().toInt();       //获得spinbox中的值，用于确定向哪个方向偏移
    if(vertical_r<height)       //如果spinbox中的值大于图片的高度，则表明向上偏移
    {
        int TranslationValue = height - vertical_r;     //求出向上偏移的量
    	for(int y=height-1;y>=TranslationValue;y--)
    	{
    		for(int x=0;x<width;x++)
    		{
                QColor old = QColor(ImageMedium.pixel(x,y-TranslationValue));       //对图片进行向上的偏移
    			ImageMedium.setPixelColor(x,y,old);
    		}
    	}
    	for(int y=0;y<TranslationValue;y++)
    		for(int x=0;x<width;x++)
                ImageMedium.setPixel(x,y,qRgb(255,255,255));        //对剩余的地方进行处理，变为白色
    }
    if(vertical_r>height)	//如果spinbox中的值小于图片的高度，则认定为向下偏移
    {
        int TranslationValue = vertical_r - height;     //求出向下偏移的量
        for(int y=0;y<height-TranslationValue;y++)
    	{
    		for(int x=0;x<width;x++)
    		{
                QColor old = QColor(ImageMedium.pixel(x,y+TranslationValue));       //对图片进行偏移
    			ImageMedium.setPixelColor(x,y,old);
    		}
    	}
        for(int y=height-TranslationValue;y<height;y++)
    		for(int x=0;x<width;x++)
                ImageMedium.setPixel(x,y,qRgb(255,255,255));        //对剩余的部分处理，变为白色
    }
    latestVerticalChange = (latestVerticalChange == vertical_r)?0:abs(vertical_r-latestVerticalChange);
    ImageMediumRec = ImageMedium;
    Reload(ImageMedium);
}


void TranslationHandle::on_Original_bt_clicked()
{
    ImageMedium.load(ImagePathBak);
    ui->horizontalspinBox->setValue(ImageMedium.width());
    ui->horizontalSlider->setSliderPosition(ImageMedium.width());
    ui->verticalspinBox->setValue(ImageMedium.height());
    ui->verticalSlider->setSliderPosition(ImageMedium.height());
	Reload(ImageMedium);
}



bool TranslationHandle::on_Save_bt_clicked()
{
    if(ImageMedium.isNull())
        QMessageBox::warning(this,"Warning","Cannot Save!");
    else ImageMedium.save(QFileDialog::getSaveFileName(this,tr("保存图片"),
                                                   "",
                                                     tr("Images (*.png *.bmp *.jpg)")));
    return true;
}
