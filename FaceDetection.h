#pragma once

#include "cv.h"
#include "highgui.h"


#define IMG_DATA_NUM 25

class CFaceDetection
{
	IplImage * m_image; //input image
	IplImage * m_gray;  
	IplImage * m_small_image;
	IplImage * m_rect_imgae;

	int dataSetImg_num; //�����Ͱ���

	char * m_classifier;//�н��� Haar�з��� ������ �ִ� xml
	CvHaarClassifierCascade * m_cascade;//Haar�з������
	CvMemStorage * m_storage;//����� ���� �޸𸮰���
	CvSeq *m_faces; //�з���� ����� ������

public:
	CFaceDetection(void);
	~CFaceDetection(void);

	int setImgageByName(const char* imagePath);//���� ��θ� �޾ƿ� m_image����
	void setImgageByImage(IplImage* image);

	void face_detection_running(); // 1.face_preprocesing() 2.face_detect() 3. face_draw()
	void face_preprocesing();//��ó������
	void face_detect();		 //�󱼰���
	void face_draw();		 //����ȿ���ǥ��

	
	IplImage* makeDetectedFace(); //����Ⱦ󱼿���img
	void makeDateSet();			  //������ �񱳵Ǿ��� ������ �� �����
								  // 1. setImgageByName()�� �̸����� �̹����� �ε�
								  // 2. face_detection_running() �� ����
								  // 3. makeDetectedFace()�� ����ȿ��� ����img�� ��ȯ
								  // 4. ��ó���� ����

};

