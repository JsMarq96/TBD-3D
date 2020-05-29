#include "stage.h"

// Hashes from generating the gradients, from the code defined by Kim Perlin
	const int hashes[257] = {151, 160, 137, 91, 90, 15,
			131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
			190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
			88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
			77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
			102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
			135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
			5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
			223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
			129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
			251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51, 145, 235, 249, 14, 239, 107,
			49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
			138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180, 151};

// Todo: frustum coling
void sStage::render_stage(Camera *camera) {
    // First, we render the floor
    Mesh floor_mesh = Mesh();
    floor_mesh.createPlane(150);

    // Draw floor
    Shader *shad = Shader::Get("data/shaders/basic.vs", "data/shaders/phong.ps");
    shad->enable();
    shad->setUniform("u_texture", Texture::Get(floor_img_dir.c_str()));
    shad->setUniform("u_color", Vector4(1, 1, 1, 1));
    shad->setUniform("u_viewprojection", camera->viewprojection_matrix);
    shad->setUniform("u_model", Matrix44());
    shad->setUniform("light_pos", Vector3(250, 120, 250));
    shad->setUniform("camera_pos", camera->eye);
    shad->setUniform1Array("hash_lookup_table", hashes, 257);

    floor_mesh.render(GL_TRIANGLES);
    shad->disable();

    std::vector<sRenderEntity*>::iterator it;
    // Render elements
    for (it = render_elements.begin(); it < render_elements.end(); it++) {
        sRenderEntity* curr_item = *it;
        curr_item->render(camera);
    }
}

void sStage::add_instance(int type, Matrix44 model) {
    render_elements[type]->add_element(model);
}

int sStage::add_element(std::string mesh_name, std::string text_name, std::string shader_fs, std::string shader_vs, eColliderType col_type, bool is_instanced) {
    sRenderEntity* new_elem = new sRenderEntity(shader_fs, shader_vs, mesh_name, text_name, col_type, is_instanced);

    render_elements.insert(render_elements.end(), new_elem);
    
    return render_elements.size()-1;
}

int sStage::add_element(std::string elem_name) {
    int index = -1;
    std::cout << elem_name << std::endl;
    if (elem_name == "tree.obj") {
        index = add_element("data/meshes/tree.obj","data/textures/tree.png","data/shaders/phong.ps", "data/shaders/instanced.vs", CILINDER, true);
    } else if (elem_name == "house.obj") {
        index = add_element("data/house_f2.obj","data/textures/tree.png","data/shaders/flat.fs", "data/shaders/basic.vs", MESH, false);
    } 

    return index;
}

sStage::sStage(int n_x, int n_y, int n_width, int n_heigh) {
    x= n_x, y = n_y;
    width = n_width, heigth = n_heigh;

    // Set default light position
    light_pos = Vector3(250, 120, 250);
}

bool sStage::testStageCollisionsWith(Vector3 position, float radius, Vector3 &coll_pos, Vector3 &normal) {
    std::vector<sRenderEntity*>::iterator it;
    // Render elements
    for (it = render_elements.begin(); it < render_elements.end(); it++) {
        sRenderEntity* curr_item = *it;
        
        if (curr_item->testCollisionsWith(position, radius, coll_pos, normal)) {
            return true;
        }
    }
    return false;
}
