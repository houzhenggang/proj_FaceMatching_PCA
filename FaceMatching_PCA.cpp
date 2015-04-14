#include "FaceMatching_PCA.h"


// * ������ *//
CFaceMatching_PCA::CFaceMatching_PCA(void)
{
	facedt = new CFaceDetection();							//CFaceDetectionŬ��
	m_nDataSetNum = 0;
}



// * �Ҹ��� *//
CFaceMatching_PCA::~CFaceMatching_PCA(void)
{
	m_imgResultFace.clear();
	m_cstrImgFileName.clear();
}

int CFaceMatching_PCA::f_setnDataSetNum(int num)
{
	if(num<0){//�߸��� ���� ������ error
		printf("...error ::CFaceMatching_PCA::f_setnDataSetNum() : num value is wrong!!\n");
		return -1; //����
	}
	m_nDataSetNum = num;
	printf("..f_setnDataSetNum inserted.. num : %d \n", m_nDataSetNum);
	return 1;//����
}

int CFaceMatching_PCA::f_getnDataSetNum()
{	
	return m_nDataSetNum;
}

int CFaceMatching_PCA::f_insertImage(IplImage * imgInput, CString cstrName)
{
	IplImage* gray;

	gray = cvCreateImage(cvGetSize(imgInput), imgInput->depth, 1);
	cvCopy(imgInput, gray);

	m_imgResultFace.push_back(gray); //�� ���� ����
	m_cstrImgFileName.push_back(cstrName);// �����̸� ����
//	m_nDataSetNum++;//��������
	printf("..image inserted.. %s \n", cstrName );
	
// 	cvNamedWindow("f_insertImage", CV_WINDOW_AUTOSIZE);   //!! test�̹��������ֱ�
// 	cvShowImage("f_insertImage", gray);
// 	cvWaitKey(0);
//	cvReleaseImage(&gray);
	return 1;
}

int CFaceMatching_PCA::f_insertDataSet()	
{
	if(m_nDataSetNum < 0){//�߸��� ���� ������ error
		printf("...error ::CFaceMatching_PCA::f_setnDataSetNum() : num %d value is wrong!!\n",m_nDataSetNum );
		return -1; //����
	}
	m_imgResultFace.clear();
	m_cstrImgFileName.clear();

	if(m_nDataSetNum > 20){//�߸��� ���� ������ error
		m_imgResultFace.reserve( sizeof(IplImage*)*100 );
		m_cstrImgFileName.reserve( sizeof(CString)*100 );
		m_Distance.reserve( sizeof(NameDistance*)*100 );

	}

	IplImage * img;
	CString imgName;
	char file_name_result[100];	

	for (int i = 0 ; i<m_nDataSetNum ; i++){
		sprintf(file_name_result, "./setting_files/faceImg_test_result/%d.jpg", i+1);
		img = cvLoadImage( file_name_result , CV_LOAD_IMAGE_GRAYSCALE);
		imgName.Format("%s", file_name_result);

		m_nHeight = img->height;
		m_nWidth = img->width;
		if( this->f_insertImage( img, imgName ) !=1 ){
			printf("...f_insertDataSet()... f_insertImage() error\n");
			return -1;	// InsertImage
		}
	}
	printf("..data set inserted.. num : %d \n", m_nDataSetNum);
	cvReleaseImage(&img);

	return 1;
}

int CFaceMatching_PCA::f_insertAddtionalImage( char* strFilePath, int nCurrentDataSetNum)
{
	if(nCurrentDataSetNum < m_nDataSetNum){//�߸��� ���� ������ error
		printf("...error ::CFaceMatching_PCA::f_insertAddtionalImage() : num value is wrong!!\n");
		return -1; //����
	}

	//////////////////��ó������ �ʿ�/////////////
	IplImage * imgOriginal;
	IplImage * imgInput;
	IplImage * imgResult;
	CString imgName;
	char file_name_result[100];	

	
		
	//��ó�� gray, resize, dectection
	imgOriginal = cvLoadImage(strFilePath, CV_LOAD_IMAGE_COLOR);
	imgInput= this->f_matching_insertImage(imgOriginal);			//  f_matching_insertImage()
	if( imgInput == NULL){
		printf("..image detection failed.. f_matching_result()");
		return -1;//����
	}
	
	//  ./setting_files/faceImg_test_result/%d.jpg������
	sprintf(file_name_result, "./setting_files/faceImg_test_result/%d.jpg", nCurrentDataSetNum+1 );
	cvEqualizeHist(imgInput, imgInput);
	cvSaveImage(file_name_result, imgInput );
	
	// ./setting_files/faceImg_test_result/%d.jpg���� �ε��ϰ� Ŭ������ ����


	imgResult = cvLoadImage( file_name_result , CV_LOAD_IMAGE_GRAYSCALE);
	imgName.Format("%s", file_name_result);
	if( this->f_insertImage( imgResult, imgName ) !=1 ) return -1;// InsertImage()
//	if( this->f_insertImage( imgInput, file_name_result ) !=1 ) return -1;// InsertImage()

// 	cvNamedWindow("imgInput", CV_WINDOW_AUTOSIZE);   //!! test�̹��������ֱ�
// 	cvShowImage("imgInput", imgInput);
// 	cvWaitKey(0);

	cvReleaseImage(&imgOriginal);
	cvReleaseImage(&imgResult);
	cvReleaseImage(&imgInput);

	return 1;//����
}


int CFaceMatching_PCA::f_updateAddtionalImage( char* strFilePath, char * nCurrentDataSetNum)
{
	//////////////////��ó������ �ʿ�/////////////
	IplImage * imgOriginal;
	IplImage * imgInput;
	IplImage * imgResult;
	CString imgName;
	char file_name_result[100];	


	//��ó�� gray, resize, dectection
	imgOriginal = cvLoadImage(strFilePath, CV_LOAD_IMAGE_COLOR);
	imgInput= this->f_matching_insertImage(imgOriginal);			//  f_matching_insertImage()
	if( imgInput == NULL){
		printf("..image detection failed.. f_matching_result()");
		return -1;//����
	}

	//  ./setting_files/faceImg_test_result/%d.jpg������
	sprintf(file_name_result, "./setting_files/faceImg_test_result/%s", nCurrentDataSetNum );
	cvEqualizeHist(imgInput, imgInput);
	cvSaveImage(file_name_result, imgInput );


	// ./setting_files/faceImg_test_result/%d.jpg���� �ε��ϰ� Ŭ������ ����

	//	sprintf(file_name_result, "./setting_files/faceImg_test_result/%d.jpg", nCurrentDataSetNum+1 );
	imgResult = cvLoadImage( file_name_result , CV_LOAD_IMAGE_GRAYSCALE);
	imgName.Format("%s", file_name_result);

	if( this->f_insertImage( imgResult, imgName ) !=1 ) return -1;// InsertImage()
	//	m_nDataSetNum++;// ����� ������ ��������

	cvReleaseImage(&imgOriginal);
	cvReleaseImage(&imgResult);
	cvReleaseImage(&imgInput);

	return 1;//����
}

void CFaceMatching_PCA::f_PCA()
{

	//* ������ �ʱ�ȭ �۾� *//

	// PCA_pImgDataSet
	PCA_pImgDataSet = new IplImage*[m_nDataSetNum];
	for (int i = 0; i < m_nDataSetNum; i++){
		PCA_pImgDataSet[i] = m_imgResultFace[i];
		
// 		printf("PCA_pImgDataSet[%d]\n", i);
// 	 	cvNamedWindow("PCA_pImgDataSet", CV_WINDOW_AUTOSIZE);
// 	  	cvShowImage("PCA_pImgDataSet", PCA_pImgDataSet[i]);
// 	  	cvWaitKey(0);
	}

	// avg
	avg = cvCreateImage(cvSize(m_nWidth,m_nHeight), IPL_DEPTH_32F, 1);

	// eigen_vector
	eigen_vector = new IplImage*[m_nDataSetNum -1];
	for (int i = 0; i < m_nDataSetNum - 1; i++)
		eigen_vector[i] = cvCreateImage(cvGetSize(PCA_pImgDataSet[i]), IPL_DEPTH_32F, 1);

	// eigen_value
	eigen_value = new double[m_nDataSetNum];
	memset(eigen_value, 0, sizeof(double)*m_nDataSetNum);

	// coeffs
	coeffs = new double*[m_nDataSetNum];
	coeffs[0] = new double[m_nDataSetNum*(m_nDataSetNum - 1)];
	for (int i = 1; i < m_nDataSetNum; i++){
		coeffs[i] = coeffs[i - 1] + (m_nDataSetNum - 1);
	}

	// projection_vector
	projection_vector = new double*[m_nDataSetNum];
	projection_vector[0] = new double[m_nDataSetNum*(m_nDataSetNum - 1)];
	for (int i = 1; i < m_nDataSetNum; i++){
		projection_vector[i] = projection_vector[i - 1] + (m_nDataSetNum - 1);
	}



	//* PCA algorithm*//
	f_PCA_cal();


	// PCA �������� �޸�����
//	delete[] PCA_pImgDataSet;
	delete[] coeffs[0];
	delete[] coeffs;
//	delete[] eigen_value;

}



int CFaceMatching_PCA::f_PCA_cal()
{
// 	IplImage* cov_matirx = cvCreateImage(cvSize(m_nWidth,m_nHeight), IPL_DEPTH_8U, 1);
// 	cvCalcCovarMatrix((const CvArr**)PCA_pImgDataSet, m_nDataSetNum-1, cov_matirx, avg, CV_COVAR_NORMAL);
// 	cvNamedWindow("avg", CV_WINDOW_AUTOSIZE);
// 	cvShowImage("avg", avg);
// 	cvWaitKey(0);


	// 1. cvTermCriteria() : ��꿡 �ʿ��� CvTermCriteria�ڷ��� ����
//	criteria = cvTermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, m_nDataSetNum , 0.01);
	criteria = cvTermCriteria(CV_TERMCRIT_ITER , m_nDataSetNum-1 , 0.03);

	// 2. cvCalcEigenObjects() : �����ͼµ��� PCA���� �ʿ�� �ϴ� ��� ����, ���̰� ������
	cvCalcEigenObjects(m_nDataSetNum, PCA_pImgDataSet, eigen_vector, 0, 0, 0, &criteria, avg, (float*)eigen_value);

//	cvEigenProjection(eigen_vector, m_nDataSetNum, 0, 0, (float*)coeffs, avg, projection_image);
// 	for(int i = 0 ; i < m_nDataSetNum ; i++){
// 		cout<< "eigen_value " << eigen_value[i] <<endl;
// 	}

	// 3. cvCalcDecompCoeff() :  ���� ���̰� �����̽��� �̹����� �翵�ϴ� ����
	cout <<"projection_vector" <<endl;
	for (int i = 0; i < m_nDataSetNum; i++)	{
		for (int j = 0; j < m_nDataSetNum-1; j++)		{
			projection_vector[i][j] = cvCalcDecompCoeff(PCA_pImgDataSet[i], eigen_vector[j], avg);
			if (projection_vector[i][j] >5000 || projection_vector[i][j] <-5000)	{
				projection_vector[i][j] = 0;
			}
			if(_isnan(projection_vector[i][j]) ){
				projection_vector[i][j] = 0;
			}
	//		cout << "["<<i<<"]["<<j<<"]" << projection_vector[i][j] << "\t";
		}
	//	cout<<endl;
	}
	
	
	return 1;
}


////////////////////////////// Matching ////////////////////////////
CString CFaceMatching_PCA::f_matchingImage(IplImage* Image)
{
	printf("f_matchingImage() start\n");
	IplImage* gray;

	double LIMIT_DISTANCE = 19000.0;
	double LIMIT_DEGREE = -0.2;
	int MODE_DISTANCE = 0;
	int MODE_DEGREE = 2;

	m_Distance.clear();

	gray = cvCreateImage(cvGetSize(Image), Image->depth, 1);
	cvCopy(Image, gray);
	double* SearchVector = new double[m_nDataSetNum -1];
	vector<double> m_pdistances;

	for (int i = 0; i < m_nDataSetNum - 1; i++)	{
		SearchVector[i] = cvCalcDecompCoeff(gray, eigen_vector[i], avg); //���� ���̰� ���Ϳ� ���� �̹����� �翵
//		if(SearchVector[i] > 5000 || SearchVector[i] < -5000) SearchVector[i] = 0;
		if(_isnan(SearchVector[i]) ){
			SearchVector[i] = 0;
		}
		cout << "..."<< i<<" : SearchVector : "<<SearchVector[i]<< endl;
	}
	double TestVal;

	//����� m_pdistances�� ����
	for (int i = 0; i < m_nDataSetNum; i++){	
		double distance = f_matching_calcDistance((double*)SearchVector, (double*)projection_vector[i], 0, m_nDataSetNum -2, MODE_DISTANCE, eigen_value);
		double Tp		= f_matching_calcDistance((double*)SearchVector, (double*)projection_vector[i], 0, m_nDataSetNum -2, MODE_DEGREE, eigen_value);

 		if( distance < LIMIT_DISTANCE && Tp < LIMIT_DEGREE ){
 			cout << "..."<< i<<" : Tp : "<< Tp << ",   distance: " <<distance << endl;
		}
		else{
			cout << "..."<< i<<" : Tp : "<< Tp << ",   distance: " <<distance << "...out of bound !!" << endl;
			Tp = 0;
		}
		m_pdistances.push_back(Tp); 
		NameDistance* distanceSet = new NameDistance;
		distanceSet->Distance = Tp;
		distanceSet->m_cName = m_cstrImgFileName[i];
		m_Distance.push_back(distanceSet);
	}

	//m_pdistances �ּҰ�ã��
	double Mymin = INFINITE;
	int Index = 0;
	int i = 0;
	Mymin =  m_pdistances.at(0);
	for (int i = 0; i<m_nDataSetNum; i++){

		cout << i << "=> Mymin : "<<Mymin<<", m_pdistances.at(i) :" << m_pdistances.at(i)<< endl;
		if (Mymin > m_pdistances.at(i)){
			Mymin = m_pdistances.at(i);
			Index = i;
			printf("f_matchingImage() Index: %d\n", Index);
		}
	}

	cvReleaseImage(&gray);
	delete[]SearchVector;
	m_pdistances.clear();
	//	SortData Tempss;
	//	sort(m_Distance.begin(), m_Distance.end(), Tempss); //sort
	return  m_cstrImgFileName[Index];
}


double CFaceMatching_PCA::f_matching_calcDistance(double* vector_1, double* vector_2, int starting_position, int ending_position, int measurement, double* eigenvalue)
{
	double distance = 0.0;
	double temp = 0.0;
	double temp_1 = 0.0;
	double temp_2 = 0.0;
	if (measurement == 0) // L1  ����ź �Ÿ�(Manhattan Distance)
	{
		for (int i = starting_position; i <= ending_position; i++)
		{
			distance += abs(vector_1[i] - vector_2[i]);
		}
	}
	else if (measurement == 1) // L2 ��Ŭ����� �Ÿ�(Euclidean Distance)
	{
		for (int i = starting_position; i <= ending_position; i++)
		{
			distance += (vector_1[i] - vector_2[i]) * (vector_1[i] - vector_2[i]);
		}
	}
	else if (measurement == 2) // Cosine
	{
		for (int i = starting_position; i <= ending_position; i++)
		{
			temp += vector_1[i] * vector_2[i];
			temp_1 += vector_1[i] * vector_1[i];
			temp_2 += vector_2[i] * vector_2[i];
		}
		distance = -temp / sqrt(temp_1 * temp_2);
	}
	else if (measurement == 3) // Mahalanobis Distance ���Ҷ��� �Ÿ�
	{
		for (int i = starting_position; i <= ending_position; i++)
		{
			distance += vector_1[i] * vector_2[i] * eigenvalue[i];
		}
		distance = -distance;
	}
	else if (measurement == 4) // L1 + Mahalanobis
	{
		for (int i = starting_position; i <= ending_position; i++)
		{
			distance += abs(vector_1[i] - vector_2[i]) * eigenvalue[i];
		}
	}
	else if (measurement == 5) // L2 + Mahalanobis
	{
		for (int i = starting_position; i <= ending_position; i++)
		{
			distance += (vector_1[i] - vector_2[i]) * (vector_1[i] - vector_2[i]) * eigenvalue[i];
		}
	}
	else if (measurement == 6) // Cosine + Mahalanobis
	{
		for (int i = starting_position; i <= ending_position; i++)
		{
			temp += vector_1[i] * vector_2[i] * eigenvalue[i];
			temp_1 += vector_1[i] * vector_1[i];
			temp_2 += vector_2[i] * vector_2[i];
		}
		distance = -temp / sqrt(temp_1 * temp_2);
	}
	return distance;
}

IplImage * CFaceMatching_PCA::f_matching_insertImage(IplImage* imgInput)
{
	facedt->setImgageByImage(imgInput);
	facedt->face_detection_running();
	IplImage * imgDetected = facedt->makeDetectedFace();
	if(imgDetected==NULL){
		printf("�� ���⿡�� ����!!\n");
		//	exit(-1);
		return NULL;
	}
	IplImage* imgResult = cvCreateImage(cvSize(80, 80), IPL_DEPTH_8U, 1);
	//	cvNamedWindow("imgDetected", CV_WINDOW_AUTOSIZE);   //!! test�̹��������ֱ�
	//	cvShowImage("imgDetected", imgDetected);
	//	cvWaitKey(0);
	cvResize(imgDetected, imgResult, CV_INTER_CUBIC);

	cvReleaseImage(&imgDetected);
	return imgResult;
}

CString CFaceMatching_PCA::f_matching_result(char * filepath)
{
	// 3. searching
	IplImage* imgOriginal = cvLoadImage(filepath, CV_LOAD_IMAGE_COLOR);
	IplImage* imgInput;
	CString str;
	//��ó�� gray, resize, dectection

	imgInput= this->f_matching_insertImage(imgOriginal);			//  f_matching_insertImage()
	if( imgInput == NULL){
		printf("..image detection failed.. f_matching_result()");
		CString strFail("./setting_files/faceImg_test/FAIL");
		return strFail;
	}
	cvEqualizeHist(imgInput,imgInput);
	// 	cvNamedWindow("imgInput", CV_WINDOW_AUTOSIZE);
	// 	cvShowImage("imgInput", imgInput);
	// 	cvWaitKey(0);
	str = this->f_matchingImage(imgInput);						//  f_matchingImage()
	//	cout <<  this->SearchImage(imgInput) << endl;

	return str;
}

