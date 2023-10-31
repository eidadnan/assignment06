
/*
 * array of columns
 * pan camera around z axis
 *
 */

#include "GLM/gtc/type_ptr.hpp"
#include <glad/gl.h>

#include "learnopengl/shader_4722.h"

#include "cs4722/artifact.h"
#include "cs4722/view.h"
#include "cs4722/texture_utilities.h"
#include "callbacks.h"
#include "cs4722/window.h"
#include "cs4722/buffer_utilities.h"
//#include "../artifact_rt2.h"


static cs4722::view *the_view;
static Shader *shader;
static std::vector<cs4722::artifact*> artifact_list;

//#include "STB/stb_image.h"




void init()
{

    the_view = new cs4722::view();
    the_view->enable_logging = false;




    shader = new Shader("vertex_shader.glsl","fragment_shader.glsl");
    shader->use();

    glEnable(GL_DEPTH_TEST);



    auto* block = new cs4722::block();
    block->color_set_ = std::vector<cs4722::color>({cs4722::x11::forest_green, cs4722::x11::sienna});
    auto* cylinder = new cs4722::cylinder();
    cylinder->color_set_ = std::vector<cs4722::color>({cs4722::x11::grey70, cs4722::x11::grey100});
    auto* torus = new cs4722::torus();
    auto* sphere = new cs4722::sphere();

    auto number = 25;
    auto radius = .25;
    auto height = 5.0 * radius;
    auto gap = 4.0 * radius;
    auto inter_center = 2.0 * radius + gap;
    auto edge_gap = 0.5 * gap;
    auto base = glm::vec4(edge_gap, edge_gap, 0, 1);
    auto ground_width = inter_center * (number - 1) + 2 * edge_gap + 2 * radius;
    auto ground_depth = ground_width;



    the_view->camera_position = glm::vec3(0, 0, radius);
    the_view->camera_up = glm::vec3(0,0,1);
    the_view->camera_forward = glm::normalize(glm::vec3(1, 1, 0));
    the_view->camera_left = glm::cross(the_view->camera_up, the_view->camera_forward);


    for(int x = 0; x < number; x++) {
        for(int y = 0; y < number; y++) {
            auto artf = new cs4722::artifact_rotating();
            artf->the_shape = cylinder;
            artf->world_transform.scale = glm::vec3(radius, height, radius);
            auto where = base + glm::vec4(x * inter_center, y*inter_center, 0, 0);
            artf->world_transform.translate = where + glm::vec4(0,0,height / 2,0);
            artf->world_transform.rotation_angle = M_PI / 2;
            artf->world_transform.rotation_axis = glm::vec3(1,0,0);
//            artf->rotation_rate = M_PI / 3;
            artf->animation_transform.rotation_center = glm::vec3(number, number, 0);
            artf->animation_transform.rotation_center =
                    artf->world_transform.matrix() * glm::vec4(1,0,0,1);
            auto ca = 200;
            auto cb = 255 - ca;

            artifact_list.push_back(artf);

            artf = new cs4722::artifact_rotating();
            artf->the_shape = block;
            artf->world_transform.scale = glm::vec3(radius, inter_center , radius);
            artf->world_transform.translate = where + glm::vec4(0, 0, height + radius/2 , 0);
            artf->animation_transform.rotation_axis = glm::vec3(0,0,1);
            artf->animation_transform.rotation_center = where;
            if(x % 2 == 0) {
                if (y % 2 == 0) {
                    artf->animation_transform.rotation_angle = M_PI;
                } else {
                    artf->animation_transform.rotation_angle = M_PI / 2;
                }
            } else {
                if (y % 2 == 0) {
                    artf->animation_transform.rotation_angle = 3* M_PI / 2;
                } else {
                    artf->animation_transform.rotation_angle = 0;
                }
            }

            artf->rotation_rate = M_PI/3;
            artifact_list.push_back(artf);


        }
    }

    auto artf = new cs4722::artifact_rotating;
    auto bl = new cs4722::block();
    bl->texture_scale = number;
    artf->the_shape = bl;
    artf->world_transform.scale = glm::vec3(ground_width, ground_depth, .01);
    artf->world_transform.translate = glm::vec3(ground_width / 2, ground_depth / 2, 0);
    artifact_list.push_back(artf);

    cs4722::init_buffers(shader->ID, artifact_list, "b_position", "b_color", "b_texture_coord");

}


void render()
{

    auto view_transform = glm::lookAt(the_view->camera_position,
                                      the_view->camera_position + the_view->camera_forward,
                                      the_view->camera_up);
    auto projection_transform = glm::infinitePerspective(the_view->perspective_fovy,
                                                         the_view->perspective_aspect, the_view->perspective_near);
    auto vp_transform = projection_transform * view_transform;


    static auto last_time = 0.0;
    auto time = glfwGetTime();
    auto delta_time = time - last_time;
    last_time = time;


    for (auto artf : artifact_list) {

        artf->animate(time, delta_time);

        auto model_transform = artf->animation_transform.matrix() * artf->world_transform.matrix();
        auto transform = vp_transform * model_transform;
        shader->setMat4("u_transform", transform);
        /*
         * Set the texture unit to use for this artf
         */
        shader->setInt("u_sampler", artf->texture_unit);
        shader->setInt("u_surface_effect", artf->surface_effect);
        shader->setVec4("u_diffuse_color", artf->surface_material.diffuse_color.as_float_array());

        glDrawArrays(GL_TRIANGLES, artf->the_shape->buffer_start, artf->the_shape->buffer_size);
    }
}


int
main(int argc, char** argv)
{
    glfwInit();

    cs4722::set_opengl_43();

    GLFWwindow *window = cs4722::setup_window_9_16_9("Texturing");
    gladLoadGL(glfwGetProcAddress);

    init();
    the_view->perspective_aspect = cs4722::get_aspect_ratio(window);

    glfwSetWindowUserPointer(window, the_view);
    glfwSetKeyCallback(window, general_key_callback);
    glfwSetCursorPosCallback(window, move_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    glfwSetWindowUserPointer(window, the_view);
	
    while (!glfwWindowShouldClose(window))
    {
        glClearBufferfv(GL_COLOR, 0, cs4722::x11::gray50.as_float_array());
        glClear(GL_DEPTH_BUFFER_BIT);

        render();
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    glfwDestroyWindow(window);

    glfwTerminate();
}
