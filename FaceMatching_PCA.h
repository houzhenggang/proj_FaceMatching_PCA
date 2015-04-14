#pragma once


#include <vector>
#include <atlstr.h>

#include "cv.h"
#include "cxcore.h"
#include "cvaux.h"
#include "highgui.h"

#include "FaceDetection.h"

using namespace cv;
using namespace std;

typedef struct NameDistance{
	CString m_cName;
	double Distance;
} NameDistance;



class CFaceMatching_PCA
{

private:
//	static CFaceMatching_PCA* instance;

public:
	
	static CFaceMatching_PCA* getInstance(){
		
		static CFaceMatching_PCA *instance = NULL;   
		if(instance == NULL )instance = new CFaceMatching_PCA();
		return instance;
	}

	int m_nHeight;
	int m_nWidth;

	// * 데이터셋을 클래스로 셋팅 * //
	int				  m_nDataSetNum;   // 얼굴영상의 개수
	vector<IplImage*> m_imgResultFace; // 전처리후의 데이터셋 얼굴이미지
	vector<CString> m_cstrImgFileName; // 얼굴파일의 이름

	//*PCA 변수*//
	IplImage ** PCA_pImgDataSet;
	IplImage* avg; //평균 이미지
	IplImage** eigen_vector;	// 고유벡터(eigenvector) or eigenface
	double * eigen_value;        // 고유값(eigenvalue)
	double ** coeffs;	 // decomposition coefficients 고유계수
	double** projection_vector;
//	IplImage** projection_image; //계산된 아이겐 벡터들을 바탕으로 학습 이미지를 계산된 공간으로 사영
	CvTermCriteria criteria;   // criteria eigen objects 계산에 필요한변수


	//* Matcing 관련 변수*//
	CFaceDetection *facedt;				//얼굴영역을 검출및 전처리
	vector<NameDistance*> m_Distance;	//벡터사이의 거리계산

public:
	CFaceMatching_PCA(void);

	~CFaceMatching_PCA(void);

	int f_setnDataSetNum(int num);								// 영상의 개수를 setting(주의 insert때마다 update)
	int f_getnDataSetNum();										// 영상의 개수를 get
	int f_insertImage(IplImage * imgInput, CString cstrName);	// 얼굴이미지를 insert
	int f_insertDataSet();										// 모든 데이터셋을 insert ( f_insertImage()사용 )

	/*수정필요*/
	int	 f_insertAddtionalImage(char* cFilePath, int nCurrentDataSetNum);//추가 데이터발생시
	int  f_updateAddtionalImage( char* strFilePath, char * nCurrentDataSetNum);

	// * PCA algorithm * //
	void f_PCA();			// PCA 관련 전반적인 수행  ( f_PCA_cal() )
	int  f_PCA_cal();		// 실제 PCA계산


	// * Matching * //
	// Matching image
	CString f_matchingImage(IplImage* Image);  // 입력영상과 데이터셋에서 매칭을 수행( f_calcDistance() )
	double f_matching_calcDistance(double* vector_1, double* vector_2, int starting_position, int ending_position, int measurement, double* eigenvalue);//거리계산

	// Matching interface
	IplImage * f_matching_insertImage(IplImage* imgInput);	// CFaceDetection을 사용하여 얼굴영역을 전처리
	CString f_matching_result(char * filepath);				// 성공시 가장 유사한 파일이름을 반환( f_matching_insertImage(), f_matchingImage()사용)
	
	
};

