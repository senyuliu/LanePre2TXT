#include "laneprocess.h"
#include "glog/logging.h"

int main(int grgc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    FLAGS_log_dir    = "../log/"; 

    string Path      = "/media/ibd01/Elements/Q52/1002-1-20009-190404/Imgprocess"; 
    string Path_save = "/media/ibd01/Elements/Q52/1002-1-20009-190404/ImgLanetxt"; 
    string Cmd       = "ls " + Path + "/*.png"; 
    LOG(INFO) << "cmd: " << Cmd; 
    
    return run(Cmd, Path_save);
}

