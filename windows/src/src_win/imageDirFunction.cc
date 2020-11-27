#include "image.h"


// 根据直线角度矫正图像，适合文档，表格等文字较多的东西
int ImageRecify(const char *pInFileName, const char *pOutFileName)
{
  double degree;
  Mat src = imread(pInFileName);
  if (src.empty())
  {
    // cout << "can not find image";
    return image_empty;
  };
  Mat dst;
  //倾斜角度矫正
  degree = CalcDegree(src, dst);
  if (degree == ERROR)
  {
    // cout << "CalcDegree failed" << endl;
    return no_line;
  };
  Mat translateImage;
  translateTransformSize(src, translateImage, int(sqrt(src.cols * src.cols + src.rows * src.rows) - src.cols), int(sqrt(src.cols * src.cols + src.rows * src.rows) - src.rows));
  rotateImage(translateImage, dst, degree);
  // 下面调整图片压缩比，否则默认损失5%画质
  // vector<int> compression_params;
  // compression_params.push_back(IMWRITE_JPEG_QUALITY); //选择jpeg
  // compression_params.push_back(100);                  //在这个填入你要的图片质量，100为最小压缩，其实还是有压缩
  //Mat resulyImage = dst(Rect(0, 0, dst.cols, 500)); //裁剪个锤子
  CropImage(dst);
  imwrite(pOutFileName, dst);
  return success;
}

// 根据边框来矫正图像，适合身份证银行卡文字不多的东西
int GetContoursPic(const char *pSrcFileName, const char *pDstFileName)
{
  Mat srcImg = imread(pSrcFileName);
  if (srcImg.empty())
  {
    // cout << "srcImg.empty()";
    return image_empty;
  };
  // imshow("原始图", srcImg);
  Mat gray;
  Mat binImg;
  cvtColor(srcImg, gray, COLOR_RGB2GRAY);           // 灰度
  threshold(gray, binImg, 100, 200, THRESH_BINARY); // 二值化
  vector<vector<Point>> contours;
  vector<Rect> boundRect(contours.size());
  //只检索外框
  findContours(binImg, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE); //找轮廓
  Mat image;
  for (int i = 0; i < contours.size(); i++)
  {
    //需要获取的坐标
    Point2f rectpoint[4];
    RotatedRect rect = minAreaRect(Mat(contours[i]));

    rect.points(rectpoint); //获取4个顶点坐标
    //与水平线的角度
    float angle = rect.angle;
    // cout << "与水平线的角度 " << angle << endl;

    int line1 = sqrt((rectpoint[1].y - rectpoint[0].y) * (rectpoint[1].y - rectpoint[0].y) + (rectpoint[1].x - rectpoint[0].x) * (rectpoint[1].x - rectpoint[0].x));
    int line2 = sqrt((rectpoint[3].y - rectpoint[0].y) * (rectpoint[3].y - rectpoint[0].y) + (rectpoint[3].x - rectpoint[0].x) * (rectpoint[3].x - rectpoint[0].x));
    //rectangle(binImg, rectpoint[0], rectpoint[3], Scalar(255), 2);
    //过滤小面积
    if (line1 * line2 < 600)
    {
      continue;
    };
    //为了让正方形横着放，所以旋转角度是不一样的。竖放的，给他加90度，翻过来
    if (line1 > line2)
    {
      angle = 90 + angle;
    };

    //新建一个感兴趣的区域图，大小跟原图一样大
    Mat RoiSrcImg(srcImg.rows, srcImg.cols, CV_8UC3); //注意这里必须选CV_8UC3
    RoiSrcImg.setTo(0);                               //颜色都设置为黑色
    //对得到的轮廓填充一下
    drawContours(binImg, contours, -1, Scalar(255), FILLED);
    //抠图到RoiSrcImg
    srcImg.copyTo(RoiSrcImg, binImg);
    //创建一个旋转后的图像
    Mat RatationedImg(RoiSrcImg.rows, RoiSrcImg.cols, CV_8UC1);
    RatationedImg.setTo(0);
    //对RoiSrcImg进行旋转
    Point2f center = rect.center;                                                //中心点
    Mat M2 = getRotationMatrix2D(center, angle, 1);                              //计算旋转加缩放的变换矩阵
    warpAffine(RoiSrcImg, RatationedImg, M2, RoiSrcImg.size(), 1, 0, Scalar(0)); //仿射变换
    image = RatationedImg;
  };
  CropImage(image);
  imwrite(pDstFileName, image);
  return success;
}

// 自动透视变换矫正，使用于图像畸变
int CornerCheck(const char *pSrcFileName, const char *pDstFileName) {
  Mat image = imread(pSrcFileName);
  if (image.empty())
  {
    // cout << "srcImg.empty()";
    return image_empty;
  };
	Mat gray, binImg, midImage;
	cvtColor(image, gray, COLOR_RGB2GRAY);
	Canny(gray, midImage, 50, 200, 3);
	int maxCorners = 100;
	//double qualityLevel = 0.1;
	double minDistance = 50.0;//(image.rows) / 6;
	vector<Point2f>corners;
	Mat cornerStrength;
	cornerHarris(gray, cornerStrength, 10, 5, 0.01);
	double maxStrength;
	double minStrength;
	// 找到图像中的最大、最小值
	minMaxLoc(cornerStrength, &minStrength, &maxStrength);
	Mat dilated;
	Mat locaMax;
	// 膨胀图像，最找出图像中全部的局部最大值点
	dilate(cornerStrength, dilated, Mat());
	// compare是一个逻辑比较函数，返回两幅图像中对应点相同的二值图像
	compare(cornerStrength, dilated, locaMax, CMP_EQ);
	Mat cornerMap;
	double qualityLevel = 0.01;
	double th = qualityLevel * maxStrength; // 阈值计算
	threshold(cornerStrength, cornerMap, th, 255, THRESH_BINARY);
	cornerMap.convertTo(cornerMap, CV_8U);
	// 逐点的位运算
	bitwise_and(cornerMap, locaMax, cornerMap);
	cv::Mat_<uchar>::const_iterator it = cornerMap.begin<uchar>();
	cv::Mat_<uchar>::const_iterator itd = cornerMap.end<uchar>();
	for (int i = 0; it != itd; it++, i++)
	{
		if (*it)
			corners.push_back(Point2f(i % image.cols, i / image.cols));
	}
	RNG rng(10086);
	Point2f topLeft(image.cols, image.rows), topRight(0.0, image.rows), bottomLeft(image.cols, 0.0), bottomRight(0.0, 0.0);
	double A1, B1, C1;
	A1 = image.rows;
	B1 = image.cols;
	C1 = -(image.rows * image.cols);
	double A2, B2, C2;
	A2 = -image.rows;
	B2 = image.cols;
	C2 = 0.0;
	for (int i = 0; i < corners.size(); i++) {
		if ((A1 * corners[i].x + B1 * corners[i].y + C1) / sqrt(A1 * A1 + B1 * B1) < (A1 * topLeft.x + B1 * topLeft.y + C1) / sqrt(A1 * A1 + B1 * B1))
			topLeft = corners[i];

		if ((A1 * corners[i].x + B1 * corners[i].y + C1) / sqrt(A1 * A1 + B1 * B1) > (A1 * bottomRight.x + B1 * bottomRight.y + C1) / sqrt(A1 * A1 + B1 * B1))
			bottomRight = corners[i];

		if ((A2 * corners[i].x + B2 * corners[i].y + C2) / sqrt(A2 * A2 + B2 * B2) < (A2 * topRight.x + B2 * topRight.y + C2) / sqrt(A2 * A2 + B2 * B2))
			topRight = corners[i];

		if ((A2 * corners[i].x + B2 * corners[i].y + C2) / sqrt(A2 * A2 + B2 * B2) > (A2 * bottomLeft.x + B1 * bottomLeft.y + C2) / sqrt(A2 * A2 + B2 * B2))
			bottomLeft = corners[i];
	}
	Point2f src_coners[4];
	Point2f dst_coners[4];
	src_coners[0] = topLeft;
	src_coners[1] = bottomRight;
	src_coners[2] = topRight;
	src_coners[3] = bottomLeft;
	dst_coners[0] = Point2f(0.0, 0.0);
	dst_coners[1] = Point2f(image.cols, image.rows);
	dst_coners[2] = Point2f(image.cols, 0.0);
	dst_coners[3] = Point2f(0.0, image.rows);
	Mat warpMatrix = getPerspectiveTransform(src_coners, dst_coners);
	Mat dst;
	warpPerspective(image, dst, warpMatrix, dst.size(), INTER_LINEAR, BORDER_CONSTANT);
	imwrite(pDstFileName, dst);
  return success;
}
