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

	// * �����ͼ��� Ŭ������ ���� * //
	int				  m_nDataSetNum;   // �󱼿����� ����
	vector<IplImage*> m_imgResultFace; // ��ó������ �����ͼ� ���̹���
	vector<CString> m_cstrImgFileName; // �������� �̸�

	//*PCA ����*//
	IplImage ** PCA_pImgDataSet;
	IplImage* avg; //��� �̹���
	IplImage** eigen_vector;	// ��������(eigenvector) or eigenface
	double * eigen_value;        // ������(eigenvalue)
	double ** coeffs;	 // decomposition coefficients �������
	double** projection_vector;
//	IplImage** projection_image; //���� ���̰� ���͵��� �������� �н� �̹����� ���� �������� �翵
	CvTermCriteria criteria;   // criteria eigen objects ��꿡 �ʿ��Ѻ���


	//* Matcing ���� ����*//
	CFaceDetection *facedt;				//�󱼿����� ����� ��ó��
	vector<NameDistance*> m_Distance;	//���ͻ����� �Ÿ����

public:
	CFaceMatching_PCA(void);

	~CFaceMatching_PCA(void);

	int f_setnDataSetNum(int num);								// ������ ������ setting(���� insert������ update)
	int f_getnDataSetNum();										// ������ ������ get
	int f_insertImage(IplImage * imgInput, CString cstrName);	// ���̹����� insert
	int f_insertDataSet();										// ��� �����ͼ��� insert ( f_insertImage()��� )

	/*�����ʿ�*/
	int	 f_insertAddtionalImage(char* cFilePath, int nCurrentDataSetNum);//�߰� �����͹߻���
	int  f_updateAddtionalImage( char* strFilePath, char * nCurrentDataSetNum);

	// * PCA algorithm * //
	void f_PCA();			// PCA ���� �������� ����  ( f_PCA_cal() )
	int  f_PCA_cal();		// ���� PCA���


	// * Matching * //
	// Matching image
	CString f_matchingImage(IplImage* Image);  // �Է¿���� �����ͼ¿��� ��Ī�� ����( f_calcDistance() )
	double f_matching_calcDistance(double* vector_1, double* vector_2, int starting_position, int ending_position, int measurement, double* eigenvalue);//�Ÿ����

	// Matching interface
	IplImage * f_matching_insertImage(IplImage* imgInput);	// CFaceDetection�� ����Ͽ� �󱼿����� ��ó��
	CString f_matching_result(char * filepath);				// ������ ���� ������ �����̸��� ��ȯ( f_matching_insertImage(), f_matchingImage()���)
	
	
};

