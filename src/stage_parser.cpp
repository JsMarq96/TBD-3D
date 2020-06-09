#include "stage_parser.h"

Matrix44 setTranslationMat(float x, float y, float z) {
    Matrix44 new_mat = Matrix44();
    new_mat.setTranslation(x, y, z);

    return new_mat;
}

// To do: reformatting
void parse_stage(sStage* to_fill, std::string file_name) {
    std::string curr_line;
    std::ifstream stage_file(file_name);

    assert(stage_file.is_open() && "Could not open file");
    std::unordered_map<int, int> element_index_map;

    // Get the floor image
    getline(stage_file, curr_line);
    to_fill->floor_img_dir = curr_line;
    //Texture *text = Texture::Get(curr_line.c_str());
    //to_fill->heigth

    // Parse the object definitions
    while(getline(stage_file, curr_line)) {
        if (curr_line == "define stage")
            break;

        // Get the elemtns's in-map id
        int delim_index = curr_line.find(" ");
        int elem_id = std::stoi(curr_line.substr(0, delim_index));
        std::string rest = curr_line.substr(delim_index);

        // get the Name of the element
        std::string elem_name = rest.substr(1);

        // Store the element and retrieve the in-stage id
        element_index_map[elem_id] = to_fill->add_element(elem_name);
        //element_index_map[elem_id] = to_fill->add_element(ele, shader_vs, shader_fs, text_name);
    }

    // Parse the object position and instances
    while(getline(stage_file, curr_line)) {
        if (curr_line == "end")
            break;

        std::cout << curr_line << std::endl;
        // Fetch the id
        int delim_index = curr_line.find(" ");
        int elem_id = std::stoi(curr_line.substr(0, delim_index));
        std::string rest = curr_line.substr(delim_index+1);
        
        // Fetch the x coord
        delim_index = rest.find(" ");
        float x = std::stof(rest.substr(0, delim_index));
        rest = rest.substr(delim_index + 1);

        // Fetch the y coord
        delim_index = rest.find(" ");
        float y = std::stof(rest.substr(0, delim_index));
        rest = rest.substr(delim_index + 1);

        // Fetch the z coord
        float z = std::stof(rest);
    
        // Add the instance to the stage, at the coordinates
        to_fill->add_enviorment_instance(element_index_map[elem_id], setTranslationMat(x,y,z));
    }
}
