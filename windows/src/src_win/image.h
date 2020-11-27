  #include "opencv2/imgproc.hpp"
  #include "opencv2/highgui.hpp"
  #include <node_api.h>
  #include <string>
  #include <iostream>
  #include <vector>

// using namespace cv;
// using namespace v8;
// 命名空间有三个，亲测会污染，单独列出来，主要是cv和v8会污染，std到没发现，但是哈哈哈哈，我又没用v8了
using std::cout;
using std::endl;
using std::string;
using std::vector;

// 用于debug
using cv::imshow;      // 创建窗口的东西，node用不到
using cv::namedWindow; // 创建窗口的东西，node用不到

using cv::BORDER_CONSTANT;
using cv::CHAIN_APPROX_NONE;
using cv::CMP_EQ;
using cv::COLOR_BGR2GRAY;
using cv::COLOR_RGB2GRAY;
using cv::cvtColor;
using cv::FILLED;
using cv::imdecode;
using cv::imread;
using cv::IMREAD_UNCHANGED;
using cv::IMWRITE_JPEG_QUALITY;
using cv::INTER_LINEAR;
using cv::LINE_AA;
using cv::Mat;
using cv::Point;
using cv::Point2f;
using cv::Rect;
using cv::RETR_EXTERNAL;
using cv::RNG;
using cv::RotatedRect;
using cv::Scalar;
using cv::Size;
using cv::THRESH_BINARY;
using cv::Vec2f;
using cv::Vec3b;

// 常量定义
const float MIN_ERROR = (6.0 / 9.0 / 2.0) * CV_PI;
const float MAX_ERROR = (12.0 / 9.0 / 2.0) * CV_PI;
// 常用错误定义
#define ERROR 1234
const int success = 0;
const int image_empty = 1;
const int no_line = 2;

// ========================= 参数类定义，便于传递管理，其实结构体会比较舒服
class argumentsDir
{
public:
  std::string imageSrc;
  std::string outPutSrc;

public:
  argumentsDir(std::string imageSrc, std::string outPutSrc) : imageSrc(imageSrc), outPutSrc(outPutSrc){};
  // ~argumentsDir(){};
};

class argumentsBase64
{
public:
  std::string base64Data;
  std::string type;

public:
  argumentsBase64(std::string base64Data, std::string type) : base64Data(base64Data), type(type){};
  // ~argumentsBase64(){};
};

// ================================ 自定义异常类，因为JS胶水层已经做了大部分参数校验，所以c++的异常应该由c++处理，不返回了
class MyException
{
public:
  MyException()
  {
  }
  MyException(char *str)
  {
    msg = str;
  }
  MyException(MyException &rhs)
  {
    this->msg = rhs.msg;
  }
  std::string msg;
};

// ================== debug函数，用于将图片输出或打印
void printImage(const cv::Mat &img);
void printImage(const std::string base64);

// ================== 常用工具函数定义
std::string base64Decode(const char *Data, int DataByte);                   // 解码
std::string base64Encode(const unsigned char *Data, int DataByte);          // 编码
std::string MatToBase64(const cv::Mat &img, std::string imgType);           // mat转base
cv::Mat BaseToMat(std::string &base64_data);                                // base转mat
double DegreeTrans(double theta);                                           // 度数弧度转换
argumentsDir *GetArgumentsDir(napi_env env, napi_callback_info args);       // JS传进来的参数提取
argumentsBase64 *GetArgumentsBase64(napi_env env, napi_callback_info args); // 用来处理base64的参数
void returnValue(int result);                                               // 检测函数返回值，抛出异常的
void rotateImage(Mat src, Mat &img_rotate, double degree);                  // 逆时针旋转图像degree角度
double CalcDegree(const Mat &srcImage, Mat &dst);                           // 通过霍夫变换计算角度
void translateTransformSize(Mat const &src, Mat &dst, int dx, int dy);      // 平移图片，避免内容丢失
int CropImage(Mat &image);                                                  // 裁剪图像

// ================== 返回给JS的函数
// 接收图片地址的函数， 输出也会直接在磁盘写入图片，较慢
int GetContoursPic(const char *pSrcFileName, const char *pDstFileName); // 根据外边框矫正，只能矫正外边框明显的
int ImageRecify(const char *pInFileName, const char *pOutFileName);     // 文字矫正根函数
int CornerCheck(const char *pSrcFileName, const char *pDstFileName);    // 畸变矫正函数

// ================== 接收64编码的函数，直接重载上面的函数
int GetContoursPic(std::string &base64_data, std::string &result, std::string imgType); // 根据外边框矫正，只能矫正外边框明显的
int ImageRecify(std::string &base64_data, std::string &result, std::string imgType);    // 文字矫正根函数
int CornerCheck(std::string &base64_data, std::string &result, std::string imgType);    // 畸变矫正函数