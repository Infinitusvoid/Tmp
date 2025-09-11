#include "Scene.h"

#include "CppCommponents/Folder.h"
#include "CppCommponents/File.h"

#include "DefaultShaderSourceCode.h"



void scene_init()
{
    // writing default shaders into tmp
    {
        Folder::create_folder_if_does_not_exist_already("tmp");
        Folder::create_folder_if_does_not_exist_already("tmp/default_shaders");

        // Vertex Shader
        {
            File::writeFileIfNotExists("tmp/default_shaders/vertex_shader.glsl", DefaultShaderSourceCode_::GLSL_Vertex);
        }

        // Fragment shader
        {
            File::writeFileIfNotExists("tmp/default_shaders/fragment_shader.glsl", DefaultShaderSourceCode_::GLSL_Fragment);
        }

        // bloom_blur_fs.glsl
        {
            File::writeFileIfNotExists("tmp/default_shaders/bloom_blur_fs.glsl", DefaultShaderSourceCode_::GLSL_bloom_blur_fs);
        }

        // bloom_bright_fs.glsl
        {
            File::writeFileIfNotExists("tmp/default_shaders/bloom_bright_fs.glsl", DefaultShaderSourceCode_::GLSL_bloom_bright_fs);
        }

        // le_combine_fs.gls
        {
            File::writeFileIfNotExists("tmp/default_shaders/le_combine_fs.glsl", DefaultShaderSourceCode_::GLSL_le_combine_fs);
        }

        // le_fullscreen_vs.glsl
        {
            File::writeFileIfNotExists("tmp/default_shaders/le_fullscreen_vs.glsl", DefaultShaderSourceCode_::GLSL_le_fullscreen_vs);
        }

        // le_present_fs.glsl
        {
            File::writeFileIfNotExists("tmp/default_shaders/le_present_fs.glsl", DefaultShaderSourceCode_::GLSL_le_present_fs);
        }


    }

    // Well here we can be sure we have the valid shader that we can compile if the other ones are not finee
    

    {
        bool we_have_an_argument = true;

        if (we_have_an_argument)
        {
            //if (File::exists("path_to_commands/command.txt"))
            {
                // we read commands 
                bool commands_error = true;
                if (commands_error)
                {
                    // std::cout << "error with commands\n";
                    return;
                }
            }
        }
        else
        {
            if(false)// if (File::exists("commands.txt"))
            {
                // we read commands 
                bool commands_error = true;
                if (commands_error)
                {
                    // std::cout << "error with commands\n";
                    return;
                }
            }
            else
            {
                // we write the commands file
                // with a starting shaders as well

            }
        }
    }

    // well here we are sure we have valid commands 
    
}



void load_commmands_no_argument()
{
    std::string filepath = "commands.txt";
    std::optional<Scene_::Scene> scene = Scene_::load(filepath);

    if (scene.has_value())
    {
        Scene_::Scene safe_scene = scene.value();

        std::cout << "commands.txt loaded\n";

        safe_scene.print();

    }
    else
    {
        // write default commands 

        load_commmands_no_argument();
    }
    
    return;
}


int main(int argc, char* argv[])
{
    std::cout << "Tmp\n";

    Scene_::unit_test_with_round_trip();

    scene_init();

    if (argc < 2)
    {
        std::cout << "no arguments";
        // load commands.txt
        load_commmands_no_argument();

        
    }
    else
    {
        std::string firstArg = argv[1];
        std::cout << "First argument as std::string: " << firstArg << std::endl;


    }
    
    

    return 0;

    // std::string filepath = "commands.txt";

    

    Scene_::Scene scene;

    scene.set_width(1000);
    scene.set_height(1000);
    

    scene.set_camera_start_x(0.0f);
    scene.set_camera_start_y(0.0f);
    scene.set_camera_start_z(0.0f);
    scene.set_camera_start_fov(45.0f);
    scene.set_camera_start_pitch(0.0);
    scene.set_camera_start_yaw(0.0);
    
    

    scene.add_shader("vertex_file_path", "fragment_file_path");
    scene.add_instance(0);

    scene.set_instance_uniform_start(0, 0, 0, 0.0f);
    scene.set_instance_uniform_end(0, 0, 0, 1.0f);
    
    std::cout << "--- scene loading ---\n";
    std::optional<Scene_::Scene> scene_from_file = Scene_::load("commands_new.txt");
    if (scene_from_file.has_value())
    {
        std::cout << "--- scene printing ---\n";
        scene_from_file.value().print();

        
        Scene_::save(scene_from_file.value(), "exported_commands.txt");
    }
    
    
    
    

    


    return 0;
}

