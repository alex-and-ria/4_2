// пример работы детектора границ Кенни - cvCanny()
//
// robocraft.ru
//

#include <cv.h>
#include <highgui.h>
#include <stdlib.h>
#include <stdio.h>

#define qmode
#define sh_coords

typedef unsigned short int usi;

IplImage* image = 0;
IplImage* gray = 0, *gray1 = 0;
IplImage* dst = 0;
int Hmin=3;
int threshold1=91;
int threshold2=120;
int ngridw=640;//=125;
int ngridh=475;//=47;
unsigned int pumpx=395,pumpy=1715,bpx=350,bpy=1550;
unsigned char ispmp=0;

void myTrackbarHmin(int pos) {
	if(pos>=3){
		if(pos%2==1)
        	Hmin = pos;
    	else Hmin = pos+1;
    	cvCanny(gray, dst, threshold1, threshold2, Hmin);
    }
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
        //cvCircle(gray1,cvPoint(x, y),radius,CV_RGB(1,0,0),1,8);
        //cvLine(img, cvPoint(x-radius/2, y-radius/2), cvPoint(x+radius/2, y+radius/2),CV_RGB(250,0,0),1,8);
        //cvLine(img, cvPoint(x-radius/2, y+radius/2), cvPoint(x+radius/2, y-radius/2),CV_RGB(250,0,0),1,8);
}

void fndc(IplImage* result,int lbx,int lby,int rtx,int rty){
	int Xc = 0;
    int Yc = 0;
    int counter = 0; // счётчик числа белых пикселей
    #ifndef qmode
    printf("\nroi=(%d;%d)x(%d;%d)\n",lbx,lby,rtx,rty);
    #endif
    cvLine(gray1, cvPoint(lbx, lby), cvPoint(rtx, lby),CV_RGB(0,0,0),1,8);
    cvLine(gray1, cvPoint(lbx, lby), cvPoint(lbx, rty),CV_RGB(0,0,0),1,8);
    cvLine(gray1, cvPoint(rtx, lby), cvPoint(rtx, rty),CV_RGB(0,0,0),1,8);
    cvLine(gray1, cvPoint(lbx, rty), cvPoint(rtx,rty),CV_RGB(0,0,0),1,8);

    // пробегаемся по пикселям изображения
    for(int y=rty; y<result->height&&y<=lby; y++)
    {
        uchar* ptr = (uchar*) (result->imageData + y * result->widthStep);
        for(int x=lbx; x<result->width&&x<=rtx; x++)
        {
            if( ptr[x]>0 &&(x>bpx||y<bpy))
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
        #ifdef sh_coords
        printf("%f x %f (!)\n", centerx, centery);
        #endif
        //if(centerx>bpx||centery<bpy)
        	cvCircle(gray1,cvPoint(centerx, centery),10,CV_RGB(0,255,0),1,8);
    }
    else{
    	#ifndef qmode
    	printf("counter==0\n");
    	#endif
    }
}

// обработчик событий от мышки
void myMouseCallback( int event, int x, int y, int flags, void* param )
{
        IplImage* img = (IplImage*) param;

        switch( event ){
                case CV_EVENT_MOUSEMOVE: 
                        break;

                case CV_EVENT_LBUTTONDOWN:
                		//#ifndef qmode
                        printf("\n%d x %d\n", x, y);
                        //#endif
                        drawTarget(img, x, y, 50);
                        fndc(img,x-20,y+20,x+20,y-20);
                        break;
                case CV_EVENT_LBUTTONUP:
                	if(ispmp>=1){
                		if(ispmp==1){
                			printf("\npumpx=%d pumpy=%d",x,y);
                			pumpx=x; pumpy=y; ispmp++;
                		}
                		else if(ispmp==2){
                			printf("\nbpx=%d",x);
                			bpx=x; ispmp++;
                		}
                		else if(ispmp==3){
                			printf("\nbpy=%d",y);
                			bpy=y; ispmp=0;
                		}
                		/*else if(ispmp==4){
                			ispmp=0;
                		}*/
                	}
                	break;
               	case CV_EVENT_RBUTTONUP:
            		printf("\npmp_mode\n");
            		ispmp=1;
                	break;
        }
}

void fndall(IplImage* result, int gridw=40, int gridh=40){//R; TODO;
	int hor=result->width/gridw;
	int ver=result->height/gridh;
	for(usi i=0;i<hor;i++){
		for(usi j=0;j<ver;j++){
			fndc(result,i*gridw,(j+1)*gridh,(i+1)*gridw,j*gridh);
		}
	}
	//printf("\nqqq\n"); cvWaitKey(0);
	/*if(hor*gridw<result->width||ver*gridh<result->height){
		fndc(result,result->width-gridw,result->height,result->width,result->height-gridh);
		printf("\neoi; ni\n"); cvWaitKey(0);
	}*/
}

void chnggw(int pos){
	ngridw=pos;
	fndall(dst,ngridw,ngridh);
}

void chnggh(int pos){
	ngridh=pos;
	fndall(dst,ngridw,ngridh);
}

void putpmp(IplImage* result){
	if(pumpx>bpx) bpx=pumpx;
	if(pumpy<bpy) bpy=pumpy;//less y -- more high;
	cvCircle(result,cvPoint(pumpx,pumpy),10,CV_RGB(255,255,0),1,8);
	cvLine(result, cvPoint(bpx, result->height), cvPoint(bpx, bpy),CV_RGB(255,0,0),1,8);
	cvLine(result, cvPoint(0, bpy), cvPoint(bpx, bpy),CV_RGB(255,0,0),1,8);
}

/*void fndpmp(IplImage* result){//TODO;
	printf("\npump:\n");
	
	float xc=0,yc=result->height-1; unsigned char coh=1;
	for(int y=result->height-2; y>0; y--){
        uchar* ptr = (uchar*) (result->imageData + y * result->widthStep);
        for(int x=0; x<result->width; x++){
            if( ptr[x]>0 &&coh>252&&x>=xc){
                xc=x; yc=y; coh++;
            }
            else coh--;
        }
    }
    cvCircle(gray1,cvPoint(xc,yc),100,CV_RGB(0,0,0),1,8);
    cvShowImage("gray1",gray1);
    printf("\npmp: %f x %f\n",xc,yc);
    printf("\npmp\n");
    printf("\npmp1\n");
}*/

int main(int argc, char* argv[])
{
		float kernel[9];
        kernel[0]=-0.1; kernel[1]=0.2; kernel[2]=-0.1;

        kernel[3]=0.2; kernel[4]=2; kernel[5]=0.2;

        kernel[6]=-0.1; kernel[7]=0.2; kernel[8]=-0.1;
        CvMat kernel_matrix=cvMat(3,3,CV_32FC1,kernel);
        // имя картинки задаётся первым параметром
        const char* filename = argc == 2 ? argv[1] : "q.jpg";
        // получаем картинку
        image = cvLoadImage(filename,1);

        printf("[i] image: %s\n", filename);
        assert( image != 0 );

        // создаём одноканальные картинки
        gray = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
        //gray1 = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );
        dst = cvCreateImage( cvGetSize(image), IPL_DEPTH_8U, 1 );

        // окно для отображения картинки
        cvNamedWindow("original",CV_WINDOW_NORMAL);
        cvNamedWindow("gray",CV_WINDOW_NORMAL);
        cvNamedWindow("gray1",CV_WINDOW_NORMAL);
        cvNamedWindow("cvCanny",CV_WINDOW_NORMAL);

        // преобразуем в градации серого
        cvCvtColor(image, gray, CV_RGB2GRAY); gray1=cvLoadImage(filename,1);//cvCvtColor(image, gray1, CV_RGB2GRAY);
        // показываем картинки
        cvShowImage("original",image);
        cvShowImage("gray1",gray1);
        
        cvFilter2D(gray, gray, &kernel_matrix, cvPoint(-1,-1));
        cvShowImage("gray",gray);
        /*cvWaitKey(0);
        kernel[0]=-0.1; kernel[1]=-0.1; kernel[2]=-0.1;
        kernel[3]=-0.1; kernel[4]=2; kernel[5]=-0.1;
        kernel[6]=-0.1;kernel[7]=-0.1;kernel[8]=-0.1;
        kernel_matrix=cvMat(3,3,CV_32FC1,kernel);
        cvFilter2D(gray, gray, &kernel_matrix, cvPoint(-1,-1));
        cvShowImage("gray",gray);*/
        //cvWaitKey(0);
        putpmp(gray1);
        cvShowImage("gray1",gray1);
        //fndpmp(dst);
        
        // получаем границы
        cvCanny(gray, dst, threshold1, threshold2, Hmin);
        cvCreateTrackbar("aperture_size", "original", &Hmin, 7, myTrackbarHmin);
        cvCreateTrackbar("threshold1", "original", &threshold1, 256, threshold1f);
        cvCreateTrackbar("threshold2", "original", &threshold2, 256, threshold2f);
        cvCreateTrackbar("gridw", "original", &ngridw, dst->width, chnggw);
        cvCreateTrackbar("gridh", "original", &ngridh, dst->height, chnggh);
        cvSetMouseCallback( "cvCanny", myMouseCallback, (void*) dst);

		while(1){
        	cvShowImage("cvCanny", dst );
        	gray1=cvLoadImage(filename,1);//cvCvtColor(image, gray1, CV_RGB2GRAY);
        	// ждём нажатия клавиши
        	fndall(dst,ngridw,ngridh);
        	putpmp(gray1);
        	cvShowImage("gray1",gray1);
        	#ifdef sh_coords
        	char c = cvWaitKey(0);
        	#else
        	char c = cvWaitKey(50);
        	#endif
            if (c == 27) { // если нажата ESC - выходим
                    break;
            }
		}
        

        // освобождаем ресурсы
        cvReleaseImage(&image);
        cvReleaseImage(&gray);
        cvReleaseImage(&gray1);
        cvReleaseImage(&dst);
        // удаляем окна
        cvDestroyAllWindows();
        return 0;
}
