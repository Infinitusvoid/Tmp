#include "Scene.h"



int main()
{
    std::cout << "Tmp\n";

    Scene_::unit_test_with_round_trip();

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

