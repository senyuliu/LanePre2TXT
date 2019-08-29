#include "laneprocess.h"

CLaneProcess::CLaneProcess(void)
{
    LOG(INFO) << "CLaneProcess obj create finished!"; 
    fileList.resize(0);
}
CLaneProcess::~CLaneProcess(void)
{
    
}

bool CLaneProcess::GetImagePoint(const cv::Mat matImageIn,int label, vector<vector<cv::Point>> &vvecPoints)
{
    //parameter
    if(matImageIn.rows < 16)
    {
        LOG(ERROR) << "rows: " << matImageIn.rows << " :Mat input is too short!";
        return false; 
    }
    if(vvecPoints.size()> 0)
    {
        LOG(WARNING) << "vvecPoint is not empty!" ; 
    }
    //binary
    cv::Mat  imgin = matImageIn.clone(); 
    int rows   = imgin.rows; 
    int cols   = imgin.cols; 
    int step   = imgin.step; 
    uchar*data = (uchar*)imgin.data; 
    
    for(int i = 0 ; i < rows ; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            int value = data[i * step + j]; 
            if(label==value)
            {
                data[i * step + j] = 255; 
            }
            else 
            {
                data[i * step + j] = 0; 
            }
        }
    }

    //findcontour
    Mat imginc = imgin.clone();
    vector<vector<cv::Point>> contours;
    vector<vector<cv::Point>> contoursout;
    vector<Vec4i> hierarchy;

    findContours(imginc,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE);

    for(int i=0;i<(int)contours.size();i++)
    {
        int area = contours[i].size(); //contourArea(contours[i]);
        if(area>1000)
        {
            vvecPoints.push_back(contours[i]); 
        }
    }

    return true; 
    /*std::cout<< "vvecPoints: " << vvecPoints.size() << std::endl;
    cv::Mat imgsw(rows, cols, CV_8UC1, Scalar::all(0) ); 
    uchar* datasw = (uchar*)imgsw.data;

    for(int i = 0 ; i < vvecPoints.size() ; i++)
    {
        vector<cv::Point> vecPoint = vvecPoints[i];
        for(int j = 0; j < vecPoint.size()-1 ; j++)
        {
            cv::Point pt = vecPoint[j];
            datasw[pt.y * step + pt.x] = 255; 
        }
        vecPoint.clear(); 
    }  
    namedWindow("contour", 2); 
    imshow("contour", imgsw);
    waitKey(0);
    destroyWindow("contour");*/

    /*drawContours(imgin_copy,contoursout,-1,Scalar(255),CV_FILLED);

    namedWindow("contour", 2); 
    imshow("contour", imgin_copy);
    waitKey(1000);
    destroyWindow("contour");*/
}
    
bool CLaneProcess::GetformatList(std::string &cmd)
{
    if(cmd.size() < 3)
    {
        LOG(ERROR) << cmd << " cmd is  too short, please check !"; 
        return false; 
    }
    else 
    {
        LOG(INFO) << cmd ; 
    }

    FILE *fpin = popen(cmd.c_str(), "r");
    if(NULL == fpin)
    {
	LOG(ERROR) << "fail to open file!"<<std::endl;
        return false;
    }

    const size_t maxLine = 1000;
    char result[maxLine];
    while(1)
    {
        if( NULL == fgets(result, maxLine, fpin))
            break;
        std::string tmpString = result;
        if(tmpString[tmpString.size() - 1] == 10)
        {
            fileList.push_back(tmpString.substr(0, tmpString.size() - 1));
        }
        else
        {

            fileList.push_back(tmpString);
	}
    }
    if(0 != pclose(fpin))
    {
        LOG(ERROR) << "pclose failed!"<<std::endl;
        return false;
    }
    return true;

}

bool run(string Cmd, const string Path_save)
{
    //parameters
    if(Cmd.size() < 3)
    {
        LOG(ERROR) << Cmd << " Cmd is  too short, please check !"; 
        return false; 
    }
    else 
    {
        LOG(INFO) << Cmd ; 
    }
    if(Path_save.size() < 3)
    {
        LOG(ERROR) << Path_save << " Path_save is  too short, please check !"; 
        return false; 
    }
    else 
    {
        LOG(INFO) << Path_save ; 
    }
    //getlist
    CLaneProcess laneProcessObj; 
    bool result = laneProcessObj.GetformatList(Cmd);

    if( false == result )
    {
        LOG(ERROR) << "laneProcessObj.GetformatList return unkown error , please check!"; 
        return false; 
    }
    
    for(int i = 0 ;i < laneProcessObj.fileList.size() ; i++)
    {
        string  sImgPath       = laneProcessObj.fileList[i]; 
        cv::Mat matImageIn     = imread(sImgPath, 0);

        if(matImageIn.rows < 16)
        {
            LOG(ERROR) << "Mat read failed, please check!";
            return false; 
        }
        
        vector< vector< vector< cv::Point> > > vecTribPoints; 
        vector< int > veclabels; 
        for (int label   = 1 ; label < 3 ; label++)
        {  

            vector<vector<cv::Point>> vvecPoints; 
            int results = laneProcessObj.GetImagePoint(matImageIn,label, vvecPoints); 
            if(false == results)
            {
                LOG(ERROR) << "laneProcessObj.GetImagePoint error, please check!";
                return false;  
            }            

            if(vvecPoints.size() < 1)
            {
                continue; 
            }
            vecTribPoints.push_back(vvecPoints); 
            veclabels.push_back(label);             

            vvecPoints.clear(); 
        }

        LOG(INFO) << "vecTribPoints.size: " << vecTribPoints.size() << std::endl; 
        if( 0 == vecTribPoints.size())
        {
            LOG(INFO) << "get no result !" << std::endl; 
            continue; 
        }

        int    pos   = sImgPath.find_last_of("/");
        int    cnt   = sImgPath.size() - pos; 
 
        string name  = sImgPath.substr(pos, cnt-4);
        string patho = Path_save + name + ".txt";

        LOG(INFO) << "patho: " << patho << std::endl; 
        fstream file(patho.c_str(), ios::out);

        if(!file.is_open())
        {
            LOG(INFO) << "file open failed !"<<endl; 
            continue; 
        }
        if(veclabels.size() != vecTribPoints.size())
        {
            LOG(INFO) << "size is not equal !" << std::endl; 
            continue; 
        }

        for(int s = 0; s < vecTribPoints.size() ; s++)
        {
            vector<vector<cv::Point>> vvecPoints = vecTribPoints[s];
            int label  = veclabels[s]; 

            for(int i = 0 ; i < vvecPoints.size() ; i++)
            {
                file << label << ","; 
                vector<cv::Point> vecPoint = vvecPoints[i];

                for(int j = 0; j < vecPoint.size()-2 ; j++)
                {
                    cv::Point pt = vecPoint[j];
                    file << pt.x << "," << pt.y<<","; 
                }
                file<< vecPoint[vecPoint.size()-1].x << "," <<vecPoint[vecPoint.size()-1].y << std::endl; 
                vecPoint.clear(); 
            } 
            
            vvecPoints.clear(); 
        }
        file.close();
        vecTribPoints.clear(); 
        
    }

    google::ShutdownGoogleLogging(); 
    return true; 
}
