#include "FaceDetection.h"


CFaceDetection::CFaceDetection(void)
{
	dataSetImg_num = IMG_DATA_NUM;
	m_classifier = "setting_files/haarcascades2/haarcascade_frontalface_alt.xml";//�н��� Haar�з��� ������ �ִ� xml
	m_cascade = 0;//Haar�з������
	m_storage = 0;//����� ���� �޸𸮰���

	// 1.�з��� ������ cascade�� �Ҵ�
	m_cascade = (CvHaarClassifierCascade *) cvLoad(m_classifier, 0, 0, 0);
	if(!m_cascade){
		printf("cascade cvLoad() error\n");
		exit(-1);
	}
}


CFaceDetection::~CFaceDetection(void)
{
	cvReleaseImage(&m_image);
	cvReleaseHaarClassifierCascade(&m_cascade);
	cvReleaseMemStorage(&m_storage);
}

int CFaceDetection::setImgageByName(const char* imagePath)
{
	IplImage* imageOri = cvLoadImage(imagePath, 1);
	if(!imageOri){
		printf("image error\n"); //�̹����� path�� ���� ���
//		exit(-1);  
		return 0;
	}	

	setImgageByImage(imageOri);//�̹����� �޸𸮿� ����

	return 1;
}


void CFaceDetection::setImgageByImage(IplImage* image)
{
	this->m_image = cvCreateImage(cvGetSize(image), image->depth, 3);
	cvCopy(image, this->m_image);
}



void CFaceDetection::face_detection_running()
{
	face_preprocesing();
	face_detect();
	face_draw();
}

//=============image ��ó������==================
void CFaceDetection::face_preprocesing()
{
	m_storage = cvCreateMemStorage(0); 
		
	double scale = 1.3;
	m_gray = cvCreateImage(cvSize(m_image->width, m_image->height), 8, 1);
	m_small_image = cvCreateImage(cvSize( cvRound(m_image->width/scale), cvRound(m_image->height/scale)), 8, 1);

	cvCvtColor(m_image, m_gray, CV_BGR2GRAY);		 //�׷��̿������� ��ȯ
	cvResize(m_gray, m_small_image, CV_INTER_LINEAR);//�缱������������ resize����
	cvEqualizeHist(m_small_image, m_small_image);    //������׷���Ȱȭ

	if(m_storage) 
		cvClearMemStorage(m_storage);

//	cvNamedWindow("m_small_image", 1);
//	cvShowImage("m_small_image", m_small_image);
}


//================������==================
void CFaceDetection::face_detect()
{	
	if(m_cascade){
		m_faces = cvHaarDetectObjects(m_image, m_cascade, m_storage, 1.2, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize(0, 0));
	}

}

//================����ȿ���ǥ��==================
void CFaceDetection::face_draw()
{
	int i;
	// for each face draw the bounding rectangle
	for(i = 0; i < (m_faces ? m_faces->total : 0); i++) {
		CvRect *r = (CvRect*)cvGetSeqElem(m_faces, i);
		CvPoint pt1 = { r->x, r->y };
		CvPoint pt2 = { r->x + r->width, r->y + r->height };
		cvRectangle(m_image, pt1, pt2, CV_RGB(255, 0, 0), 3, 8, 0);

		cvSetImageROI(m_gray, cvRect(pt1.x, pt1.y, pt2.x-pt1.x, pt2.y-pt1.y) );
		m_rect_imgae = (IplImage*)cvClone(m_gray); // �󱼰��⿵�� ǥ��
//		cvCvtColor(m_rect, m_rect_imgae, CV_RGB2GRAY);
		cvResetImageROI(m_gray);
	}

	// create window and show the image with outlined faces
// 	cvNamedWindow("Face Detection", 1);
// 	cvShowImage("Face Detection", m_image);
//	cvWaitKey(0);
}


IplImage* CFaceDetection::makeDetectedFace()
{
	printf("detected num : %d \n", m_faces->total);
	if(m_faces->total!=0)
		return m_rect_imgae;
	else 
		return NULL;
}

void CFaceDetection::makeDateSet()
{
	char file_name_ori[100];
	char file_name_result[100];
	int IMG_NUM = dataSetImg_num;
	for (int i =0 ; i < IMG_NUM ; i++){
		sprintf(file_name_ori, "./setting_files/faceImg_test_ori/%d.jpg", i+1);	//�������
		sprintf(file_name_result, "./setting_files/faceImg_test_result/%d.jpg", i+1);// ������set ���� ���
		
		if(this->setImgageByName(file_name_ori)==1)	
			printf("..find and allocate image..");
		else 
			printf("..can't find image..");
		
		//this->setImgageByName(file_name_ori);
		this->face_detection_running();
		
		IplImage * imgDetected = this->makeDetectedFace();
		IplImage * imgResize = cvCreateImage(cvSize(80, 80), IPL_DEPTH_8U, 1);
		
		if(imgDetected!=NULL){
			cvResize(imgDetected, imgResize, CV_INTER_CUBIC);
			cvEqualizeHist(imgResize, imgResize); //�߰�
			cvSaveImage(file_name_result, imgResize);
		}
		cvReleaseImage(&imgDetected);
		cvReleaseImage(&imgResize);
 	}
}

