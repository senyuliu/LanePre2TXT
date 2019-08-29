#include "laneprocess.h"
#include "glog/logging.h"

int main(int argc, char* argv[])
{
    google::InitGoogleLogging(argv[0]);
    FLAGS_log_dir    = "../log/";

    //paramerer
    if(argc != 3)
    {
        LOG(INFO) << "usage: exe   path to image   path to save txt!"; 
        return 0;
    }
   
    //path 
    string Path      = argv[1];   
    string Path_save = argv[2];

    string Cmd       = "ls " + Path + "/*.png"; 
    LOG(INFO) << "cmd: " << Cmd; 
    
    return run(Cmd, Path_save);
}

