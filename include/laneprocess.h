#ifndef __LANEPROCESS_H__
#define __LANEPROCESS_H__
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <fstream>
#include <glog/logging.h>

using namespace std; 
using namespace cv; 

class CLaneProcess
{
public: 
    CLaneProcess(void);
    ~CLaneProcess(void);
public: 
    bool GetformatList(std::string &cmd); 
    bool GetImagePoint(const cv::Mat matImageIn, int label , vector<vector<cv::Point>> &vecPoints);
    
public: 
    std::vector<std::string> fileList; 
    
}; 

bool run(const string Cmd, string Path_save);

#endif 
