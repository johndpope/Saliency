

//#include "ImageSpaceManager.h"
//#include "WindowEvaluator.h"
#include "GenericObjectDetector.h"
#include "ShapeAnalyzer.h"
#include "ImgVisualizer.h"
#include "DataManager/DatasetManager.h"
#include "DataManager/NYUDepth2DataMan.h"
#include <string>
#include "ObjectSegmentor.h"
#include "a9wins/A9Window.h"
#include "Saliency/Composition/SalientRegionDetector.h"
#include "ObjectTester.h"
#include "Saliency/Depth/DepthSaliency.h"
using namespace std;

int main()
{
	//ObjectTester tester;
	//tester.TestObjectRanking(DB_BERKELEY3D);
	////tester.RunVideoDemo();
	//return 0;

	ShapeAnalyzer shaper;
	GenericObjectDetector detector;
	DatasetManager dbMan;
	dbMan.Init(DB_VOC07);
	visualsearch::ImageSegmentor segmentor;
	Berkeley3DDataManager b3dman;
	NYUDepth2DataMan nyuman;

	// process
	if( !detector.InitBingObjectness() )
		return -1;

	string datadir = "E:\\Datasets\\RGBD_Dataset\\NYU\\Depth2\\";
	string imgfn = "151.jpg";
	Mat timg = imread(datadir + imgfn);
	if(timg.empty())
		return 0;

	Mat dimg;
	string dmapfn = datadir + "151_d.txt";
	nyuman.LoadDepthData(dmapfn, dimg);
	
	//resize(timg, timg, Size(200,200));
	imshow("input img", timg);
	visualsearch::ImgVisualizer::DrawFloatImg("dmap", dimg, Mat());
	waitKey(10);
	//Mat normimg;
	//normalize(timg, timg, 0, 255, NORM_MINMAX);

	double start_t = cv::getTickCount();

	vector<ImgWin> boxes;
	detector.GetObjectsFromBing(timg, boxes, 500);

	Mat objectnessmap;
	//detector.CreateScoremapFromWins(timg.cols, timg.rows, boxes, objectnessmap);
	//visualsearch::ImgVisualizer::DrawFloatImg("objmap", objectnessmap, objectnessmap);

	std::cout<<"Bing time: "<<(cv::getTickCount()-start_t) / cv::getTickFrequency()<<"s."<<std::endl;
	
	// make images
	vector<Mat> imgs(boxes.size());
	for (int i=0; i<boxes.size(); i++)
	{
		imgs[i] = timg(boxes[i]);
	}

	Mat dispimg;
	visualsearch::ImgVisualizer::DrawImgCollection("objectness", imgs, 100, 15, dispimg);
	imshow("objectness", dispimg);
	waitKey(10);
	
	// rank windows with CC
	/*SalientRegionDetector salDetector;
	salDetector.Init(timg);

	start_t = getTickCount();

	salDetector.RankWins(boxes);
	*/
	// depth ranking
	DepthSaliency dsal;
	dsal.RankWins(dimg, boxes);

	Mat salmap;
	//detector.CreateScoremapFromWins(timg.cols, timg.rows, boxes, salmap);
	//visualsearch::ImgVisualizer::DrawFloatImg("salmap", salmap, salmap);

	std::cout<<"Saliency time: "<<(cv::getTickCount()-start_t) / cv::getTickFrequency()<<"s."<<std::endl;

	// make images
	vector<ImgWin> topBoxes;
	for (int i=0; i<boxes.size(); i++)
	{
		cout<<boxes[i].score<<endl;
		imgs[i] = timg(boxes[i]);
		if(i<20)
			topBoxes.push_back(boxes[i]);
	}

	visualsearch::ImgVisualizer::DrawImgCollection("objectness", imgs, 100, 15, dispimg);
	imshow("rank by CC", dispimg);
	visualsearch::ImgVisualizer::DrawImgWins("ddet", dimg, topBoxes);
	waitKey(0);

	cv::destroyAllWindows();

	//getchar();

	return 0;
}