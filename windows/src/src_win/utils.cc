#include "image.h"

// 转换JS传递的四个参数，图片地址、图片地址长度、输出地址、输出地址长度
argumentsDir *GetArgumentsDir(napi_env env, napi_callback_info args)
{
  // 获取参数
  size_t argc = 4;    // 	参数个数
  napi_value argv[4]; // 参数存放数组
  napi_status status_get_arguments = napi_get_cb_info(env,
                                                      args,
                                                      &argc,
                                                      argv,
                                                      0,
                                                      0);
  if (status_get_arguments != napi_ok || argc != 4)
  {
    napi_throw_type_error(env, "EBADARGS", "Wrong number of arguments");
    return 0;
  };
  // 分离参数
  int32_t imageSrcLength, outPutSrcLength;
  napi_status status_get_imageSrc_length = napi_get_value_int32(env,
                                                                argv[1],
                                                                &imageSrcLength);
  if (status_get_imageSrc_length != napi_ok || imageSrcLength <= 0)
  {
    napi_throw_type_error(env, "EBADARGS", "imageSrcLength is Err");
    return 0;
  };

  napi_status status_get_outPutSrc_length = napi_get_value_int32(env,
                                                                 argv[3],
                                                                 &outPutSrcLength);
  if (status_get_outPutSrc_length != napi_ok || outPutSrcLength <= 0)
  {
    napi_throw_type_error(env, "EBADARGS", "outPutSrcLength is Err");
    return 0;
  };

  size_t imageSrcBytes;
  char *imageSrc = new char[imageSrcLength + 1];
  napi_status status_get_imageSrc = napi_get_value_string_utf8(env, argv[0], imageSrc, imageSrcLength + 1, &imageSrcBytes);
  if (status_get_imageSrc != napi_ok || imageSrcBytes <= 0)
  {
    napi_throw_type_error(env, "EBADARGS", "Wrong imageSrc get failed");
    return 0;
  }

  size_t outPutSrcBytes;
  char *outPutSrc = new char[outPutSrcLength + 1];
  napi_status status_get_outPutSrc = napi_get_value_string_utf8(env, argv[2], outPutSrc, outPutSrcLength + 1, &outPutSrcBytes);
  if (status_get_outPutSrc != napi_ok || outPutSrcBytes <= 0)
  {
    napi_throw_type_error(env, "EBADARGS", "Wrong outPutSrc get failed");
    return 0;
  }
  // 参数处理成string
  string _imageSrc = imageSrc;
  string _outPutSrc = outPutSrc;

  // 此时需要释放内存空间咯
  delete[] imageSrc;
  delete[] outPutSrc;
  cout << "print from c++" << endl
       << _imageSrc << endl
       << _outPutSrc << endl;
  argumentsDir *MyArgs = new argumentsDir(_imageSrc, _outPutSrc);
  return MyArgs;
}

argumentsBase64 *GetArgumentsBase64(napi_env env, napi_callback_info args)
{
  // 获取参数
  size_t argc = 4;    // 	参数个数
  napi_value argv[4]; // 参数存放数组
  napi_status status_get_arguments = napi_get_cb_info(env,
                                                      args,
                                                      &argc,
                                                      argv,
                                                      0,
                                                      0);
  if (status_get_arguments != napi_ok || argc != 4)
  {
    napi_throw_type_error(env, "EBADARGS", "Wrong number of arguments");
    return 0;
  };
  // 分离参数
  int32_t base64DataLength;
  int typeLength;
  napi_status status_get_base64Data_length = napi_get_value_int32(env,
                                                                argv[1],
                                                                &base64DataLength);
  if (status_get_base64Data_length != napi_ok || base64DataLength <= 0)
  {
    napi_throw_type_error(env, "EBADARGS", "base64DataLength is Err");
    return 0;
  };
  napi_status status_get_type_length = napi_get_value_int32(env,
                                                                argv[3],
                                                                &typeLength);
  if (status_get_type_length != napi_ok || typeLength <= 0)
  {
    napi_throw_type_error(env, "EBADARGS", "typeLength is Err");
    return 0;
  };
  size_t base64DataBytes;
  // cout<<base64DataBytes<<endl;
  char *base64Data = new char[base64DataLength + 1];
  napi_status status_get_base64Data = napi_get_value_string_utf8(env, argv[0], base64Data, base64DataLength + 1, &base64DataBytes);
  if (status_get_base64Data != napi_ok || base64DataBytes <= 0)
  {
    napi_throw_type_error(env, "EBADARGS", "Wrong base64Data get failed");
    return 0;
  }
  size_t typeBytes;
  char *type = new char[typeLength + 1];
  napi_status status_get_type = napi_get_value_string_utf8(env, argv[2], type, typeLength + 1, &typeBytes);
  if (status_get_type != napi_ok || typeBytes <= 0)
  {
    napi_throw_type_error(env, "EBADARGS", "Wrong type get failed");
    return 0;
  }
    // 参数处理成string
  string _base64Data = base64Data;
  string _type = type;
  delete[] base64Data;
  delete[] type;
  // cout << "print from c++" << endl
  //      << _imageSrc << endl
  //      << _outPutSrc << endl;
  argumentsBase64 *MyArgs = new argumentsBase64(_base64Data, _type);
  return MyArgs;
}

// 抛出c++异常的函数，一般图片是否存在等校验在胶水层和GetArguments函数中就已经做过了，这里只是抛出算法的异常，基本用于调试
void returnValue(int result)
{
  switch (result)
  {
  case success:
  {
    return;
    break;
  }
  case image_empty:
  {
    throw MyException("image is empty");
    break;
  }
  case no_line:
  {
    throw MyException("line is empty");
    break;
  }
  default:
  {
    throw MyException("C++ ERROR");
    break;
  }
  }
}

// 角度弧度转换函数
double DegreeTrans(double theta)
{
  double res = theta / CV_PI * 180;
  return res;
}

// 图片裁剪函数
int CropImage(Mat &image)
{
  vector<vector<Point>> contours2;
  Mat gray;
  //SecondFindImg.setTo(0);
  cvtColor(image, gray, COLOR_BGR2GRAY);                           // 灰度化
  threshold(gray, gray, 80, 200, THRESH_BINARY);                   // 二值化
  findContours(gray, contours2, RETR_EXTERNAL, CHAIN_APPROX_NONE); // 找边框
  double maxArea = 0.0;
  if (contours2.size())
  {
    Mat dstImg;
    for (int j = 0; j < contours2.size(); j++)
    {
      //这时候其实就是一个长方形了，所以获取最大rect
      Rect rect = boundingRect(Mat(contours2[j]));
      if (rect.area() < maxArea)
      {
        continue;
      };
      maxArea = rect.area();
      dstImg = image(rect);
    };
    image = dstImg;
    return success;
  }
  else
  {
    return no_line;
  }
}

// 图像旋转函数
void rotateImage(Mat src, Mat &img_rotate, double degree)
{
  //旋转中心为图像中心
  Point2f center;
  center.x = float(src.cols / 2.0);
  center.y = float(src.rows / 2.0);
  int length = sqrt(src.cols * src.cols + src.rows * src.rows);
  //计算二维旋转的仿射变换矩阵
  Mat M = getRotationMatrix2D(center, degree, 1);
  warpAffine(src, img_rotate, M, Size(length, length), 1, 0, Scalar(0, 0, 0)); //仿射变换，背景色填充为黑色
}

//图像平移，避免图像内容丢失
void translateTransformSize(Mat const &src, Mat &dst, int dx, int dy)
{
  const int rows = src.rows + abs(dy); //输出图像的大小
  const int cols = src.cols + abs(dx);
  dst.create(rows, cols, src.type());
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
    {
      int x = j - dx;
      int y = i - dy;
      if (x >= 0 && y >= 0 && x < src.cols && y < src.rows)
        dst.ptr<Vec3b>(i)[j] = src.ptr<Vec3b>(y)[x];
    }
  }
}

// 通过直线检测计算角度
double CalcDegree(const Mat &srcImage, Mat &dst)
{
  Mat midImage, dstImage;

  Canny(srcImage, midImage, 50, 200, 3);
  //cvtColor(midImage, dstImage, COLOR_RGB2GRAY);

  //通过霍夫变换检测直线
  vector<Vec2f> lines;
  HoughLines(midImage, lines, 1, CV_PI / 180, 300, 0, 0); //第5个参数就是阈值，阈值越大，检测精度越高

  //由于图像不同，阈值不好设定，因为阈值设定过高导致无法检测直线，阈值过低直线太多，速度很慢
  //所以根据阈值由大到小设置了三个阈值，如果经过大量试验后，可以固定一个适合的阈值。

  if (!lines.size())
  {
    HoughLines(midImage, lines, 1, CV_PI / 180, 200, 0, 0);
  };

  if (!lines.size())
  {
    HoughLines(midImage, lines, 1, CV_PI / 180, 150, 0, 0);
  };
  if (!lines.size())
  {
    cout << "print from c++: can not testing lings" << endl;
    return ERROR;
  };

  //依次画出每条线段
  int size = 0;
  vector<vector<float>> storageAngle;
  for (size_t i = 0; i < lines.size(); i++)
  {
    // float rho = lines[i][0];
    float theta = lines[i][1];
    // 此时表格可能有竖着的和横着的线条，夹角一般在π/2左右，我们可以定好一个误差范围，我们取第一个角度为基准
    if (storageAngle.size())
    {
      bool flag = false;
      for (int j = 0; j < storageAngle.size(); j++)
      {
        double Difference = abs((DegreeTrans(theta) - DegreeTrans(storageAngle[j][0])));
        if (Difference < 10)
        {
          storageAngle[j][1] += theta;
          storageAngle[j][2]++;
          flag = true;
          break;
        }
      }
      if (!flag)
      {
        vector<float> temp = {theta, theta, 1.0};
        storageAngle.push_back(temp);
      }
    }
    else
    {
      vector<float> temp = {theta, theta, 1.0};
      storageAngle.push_back(temp);
    }
  };
  float sum = 0;
  float count = 0;
  for (int i = 0; i < storageAngle.size(); i++)
  {
    if (storageAngle[i][2] > count)
    {
      sum = storageAngle[i][1];
      count = storageAngle[i][2];
    }
  }
  float average = sum / count; //取相似直线最多的一组平均
  double angle = DegreeTrans(average);
  if (abs(angle) > 90.0)
    angle = angle > 0.0 ? angle - 90.0 : angle + 90.0;
  //rotateImage(midImage, dst, angle);
  return angle;
}
