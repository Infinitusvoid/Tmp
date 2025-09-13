#include "CppCommponents/Folder.h"
#include "CppCommponents/File.h"



#include <iostream>

#include "Scene.h"


const std::string folder_ouput = "C:/Users/Cosmos/Desktop/output/tmp/";
const std::string folder_output_shaders = folder_ouput + "shaders/";
const std::string folder_output_frames = folder_ouput + "frames/";

int main(int argc, char* argv[])
{
    std::cout << "Tmp\n";
    
    Folder::create_folder_if_does_not_exist_already(folder_ouput);
    Folder::create_folder_if_does_not_exist_already(folder_output_shaders);
    Folder::create_folder_if_does_not_exist_already(folder_output_frames);
    
    
    


    return 0;
}

