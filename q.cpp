// пример работы детектора границ Кенни - cvCanny()
//
// robocraft.ru
//

#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>

IplImage* image = 0;
IplImage* gray = 0;
IplImage* dst = 0;
int Hmin=3;
int threshold1=91;
int threshold2=120;

void myTrackbarHmin(int pos) {
	if(pos%2&&pos>=3)
        Hmin = pos;
    else Hmin = pos+1;
    cvCanny(gray, dst, threshold1, threshold2, Hmin);
}

void threshold1f(int pos){
	threshold1=pos;
	cvCanny(gray, dst, threshold1, threshold2, Hmin);
}

void threshold2f(int pos){
	threshold2=pos;
	cvCanny(gray, dst, threshold1, threshold2, Hmin);
}

// рисуем целеуказатель
void drawTarget(IplImage* img, int x, int y, int radius)
{
        //cvCircle(gray,cvPoint(x, y),radius,CV_RGB(0,0,0),1,8);
        //cvLine(img, cvPoint(x-radius/2, y-radius/2), cvPoint(x+radius/2, y+radius/2),CV_RGB(250,0,0),1,8);
        //cvLine(img, cvPoint(x-radius/2, y+radius/2), cvPoint(x+radius/2, y-radius/2),CV_RGB(250,0,0),1,8);
}

void fndc(IplImage* result,int lbx,int lby,int rtx,int rty){
	int Xc = 0;
    int Yc = 0;
    int counter = 0; // счётчик числа белых пикселей
    printf("\nroi=(%d;%d)x(%d;%d)\n",lbx,lby,rtx,rty);
    cvLine(gray, cvPoint(lbx, lby), cvPoint(rtx, lby),CV_RGB(0,0,0),1,8);
    cvLine(gray, cvPoint(lbx, lby), cvPoint(lbx, rty),CV_RGB(0,0,0),1,8);
    cvLine(gray, cvPoint(rtx, lby), cvPoint(rtx, rty),CV_RGB(0,0,0),1,8);
    cvLine(gray, cvPoint(lbx, rty), cvPoint(rtx,rty),CV_RGB(0,0,0),1,8);

    // пробегаемся по пикселям изображения
    for(int y=rty; y<result->height&&y<=lby; y++)
    {
        uchar* ptr = (uchar*) (result->imageData + y * result->widthStep);
        for(int x=lbx; x<result->width&&x<=rtx; x++)
        {
            if( ptr[x]>0 )
            {
                Xc += x;
                Yc += y;
                counter++; 
            }
        }
    }
    
    if(counter!=0)
    {
        float centerx = float(Xc)/counter;
        float centery = float(Yc)/counter;
        printf("%f x %f (!)\n", centerx, centery);
        cvCircle(gray,cvPoint(centerx, centery),10,CV_RGB(0,0,0),1,8);
    }
    else printf("counter==0\n");
}

// обработчик событий от мышки
void myMouseCallback( int event, int x, int y, int flags, void* param )
{
        IplImage* img = (IplImage*) param;

        switch( event ){
                case CV_EVENT_MOUSEMOVE: 
                        break;

                case CV_EVENT_LBUTTONDOWN:
                        printf("\n%d x %d", x, y);
                        drawTarget(img, x, y, 10);
                        fndc(img,x-20,y+20,x+20,y-20);
                        break;

                case CV_EVENT_LBUTTONUP:
                        break;
        }
}

int main(int argc, char* argv[])
{
        // имя картинки задаётся первым параметром
        const char* filename = argc == 2 ? argv[1] : "q.bmp";
        // получаем картинку
        image = cvLoadImage(filename,1);

        printf("[i] image: %s\n", filename);
        assert( image != 0 );

        // создаём одноканальные картинки
        gray = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
        dst = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );

        // окно для отображения картинки
        cvNamedWindow("original",CV_WINDOW_AUTOSIZE);
        cvNamedWindow("gray",CV_WINDOW_AUTOSIZE);
        cvNamedWindow("cvCanny",CV_WINDOW_AUTOSIZE);

        // преобразуем в градации серого
        cvCvtColor(image, gray, CV_RGB2GRAY);
        // показываем картинки
        cvShowImage("original",image);
        cvShowImage("gray",gray);
        

        // получаем границы
        cvCanny(gray, dst, threshold1, threshold2, Hmin);
        cvCreateTrackbar("aperture_size", "original", &Hmin, 7, myTrackbarHmin);
        cvCreateTrackbar("threshold1", "original", &threshold1, 256, threshold1f);
        cvCreateTrackbar("threshold2", "original", &threshold2, 256, threshold2f);
        cvSetMouseCallback( "cvCanny", myMouseCallback, (void*) dst);

		while(1){
        	cvShowImage("cvCanny", dst );
			cvShowImage("gray",gray);
        	// ждём нажатия клавиши
        	
        	char c = cvWaitKey(50);
            if (c == 27) { // если нажата ESC - выходим
                    break;
            }
		}
        

        // освобождаем ресурсы
        cvReleaseImage(&image);
        cvReleaseImage(&gray);
        cvReleaseImage(&dst);
        // удаляем окна
        cvDestroyAllWindows();
        return 0;
}
