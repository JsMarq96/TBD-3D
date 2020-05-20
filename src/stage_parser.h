#ifndef _STAGE_PARSER_H_
#define _STAGE_PARSER_H_

#include <string>
#include <fstream>
#include <unordered_map>
#include "stage.h"

struct sElements {
    std::string mesh;
    std::string fs_shader;
    std::string vs_shader;
    std::string texture;
};

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

    getline(stage_file, curr_line);

    assert(curr_line != "define items/n" && "Stage file format issue");
    std::unordered_map<int, int> element_index_map;

    // Parse the object definitions
    while(getline(stage_file, curr_line)) {
        if (curr_line == "define stage/n")
            break;

        // Get thhe elemtns's in-map id
        int delim_index = curr_line.find(" ");
        std::string rest = curr_line.substr(1, delim_index);
        int elem_id = std::stoi(curr_line.substr(0, delim_index));

        // get the Mesh of the element
        delim_index = rest.find(" ");
        std::string mesh_name = rest.substr(0, delim_index);
        rest = rest.substr(1, delim_index);
        
        // Get the 1st shader
        delim_index = rest.find(" ");
        std::string shader_vs = rest.substr(0, delim_index);
        rest = rest.substr(1, delim_index);

        // Get the 2nd shader
        delim_index = rest.find(" ");
        std::string shader_fs = rest.substr(0, delim_index);
        rest = rest.substr(1, delim_index);

        // Get the textrue name
        delim_index = rest.find(" ");
        std::string text_name = rest.substr(0, delim_index);
        rest = rest.substr(1, delim_index);

        // Store the element and retrieve the in-stage id
        element_index_map[elem_id] = to_fill->add_element(mesh_name, shader_vs, shader_fs, text_name);
    }

    // Parse the object position and instances
    while(getline(stage_file, curr_line)) {
        if (curr_line == "end/n")
            break;

        // Fetch the id
        int delim_index = curr_line.find(" ");
        int elem_id = std::stoi(curr_line.substr(0, delim_index));
        std::string rest = curr_line.substr(1, delim_index);

        // Fetch the x coord
        int delim_index = rest.find(" ");
        float x = std::stof(rest.substr(0, delim_index));
        rest = rest.substr(1, delim_index);

        // Fetch the y coord
        delim_index = rest.find(" ");
        float y = std::stof(rest.substr(0, delim_index));
        rest = rest.substr(1, delim_index);

        // Fetch the z coord
        float z = std::stof(rest);
        
        // Add the instance to the stage, at the coordinates
        to_fill->add_instance(element_index_map[elem_id], setTranslationMat(x,y,z));
    }
}

#endif