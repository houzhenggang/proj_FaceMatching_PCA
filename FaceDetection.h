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

	int dataSetImg_num; //데이터개수

	char * m_classifier;//학습된 Haar분류기 정보가 있는 xml
	CvHaarClassifierCascade * m_cascade;//Haar분류기생성
	CvMemStorage * m_storage;//계산을 위한 메모리공간
	CvSeq *m_faces; //분류기로 검출된 얼굴저장

public:
	CFaceDetection(void);
	~CFaceDetection(void);

	int setImgageByName(const char* imagePath);//파일 경로를 받아와 m_image생성
	void setImgageByImage(IplImage* image);

	void face_detection_running(); // 1.face_preprocesing() 2.face_detect() 3. face_draw()
	void face_preprocesing();//전처리과정
	void face_detect();		 //얼굴검출
	void face_draw();		 //검출된영역표시

	
	IplImage* makeDetectedFace(); //검출된얼굴영역img
	void makeDateSet();			  //실제로 비교되어질 데이터 셋 만들기
								  // 1. setImgageByName()로 이름으로 이미지를 로드
								  // 2. face_detection_running() 로 검출
								  // 3. makeDetectedFace()로 검출된영역 추출img로 변환
								  // 4. 전처리후 저장

};

