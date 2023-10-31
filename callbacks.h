//
// Created by Ben on 7/22/2022.
//  modified 8/27/22
//

#pragma once



float lr_scale = .1f;
float fb_scale = .1f;
float ud_scale = .1f;

float lr_pan_scale = .002f;
float ud_pan_scale = lr_pan_scale;


/*
 * These variables are used to hold the window height and width and position to use when
 *  exiting full screen.
 */
static int old_width, old_height, old_xpos, old_ypos;

void general_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto* view = static_cast<cs4722::view*>(glfwGetWindowUserPointer(window));
    // the_scene->the_view->key_callback(window, key, scancode, action, mods);
    // std::cout << "key " << key << std::endl;

    if(view->enable_logging) {
        view->display_parameters();
    }

    auto z0 = view->camera_position.z;


    auto shift_scale = 1.0f;
    if(mods & GLFW_MOD_SHIFT) {
        shift_scale = 15.0f;
    }

    if (action != GLFW_PRESS && action != GLFW_REPEAT)
    {
        // only process key presses and repeats
    }
    else if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT)
    {
        view->camera_position += lr_scale * shift_scale * view->camera_left;
    }
    else if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT)
    {
        view->camera_position -= lr_scale * shift_scale * view->camera_left;
    }
    else if (key == GLFW_KEY_W)
    {
        view->camera_position += fb_scale * shift_scale * view->camera_forward;
    }
    else if (key == GLFW_KEY_S)
    {
        view->camera_position -= fb_scale * shift_scale * view->camera_forward;
    }
    else if (key == GLFW_KEY_UP)
    {
        view->camera_position += ud_scale * shift_scale * view->camera_up;
    }
    else if (key == GLFW_KEY_DOWN)
    {
        view->camera_position -= ud_scale * shift_scale * view->camera_up;
    }
    else if (key == GLFW_KEY_ESCAPE)
    {
        /*
         * If the escape key is pressed, then the following function will set the GLFW
         * flag variable 'windowShouldClose' to true.
         * Then the event loop, in function main, and the program will terminate.
         * This is the proper way to terminate the program since it gives GLFW an opportunity to
         *      clean up the resources it is using.
         */
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    else if (key == GLFW_KEY_F11 && action == GLFW_PRESS)
    {
        /*
         * If the F11 key is pressed, but not a repeat generated by a continued press,
         * then toggle between full screen and windowed.
         */

        /*
         * If the monitor attribute of the window is set, not NULL, then
         *  the window is currently in full screen according to GLFW conventions.
         *
         */
        if (!glfwGetWindowMonitor(window)) {
            /*
             * This code is executed if the monitor attribute of the window is NULL.
             * That means that the window is currently not full screen.
             * So, execute code to take it to full screen.
             */

            /*
             * Save the window position and size.
             * The variables used to save these values are defined just above the callback function header.
             * A pointer to each variable is passed (&old_xpos is the address in memory of the variable old_xpos).
             * In this way the function can change the values of those variables.
             */
            glfwGetWindowPos(window, &old_xpos, &old_ypos);
            glfwGetWindowSize(window, &old_width, &old_height);
            /*
             * Get an object representing the primary monitor.
             *
             * While it would be nicer to figure out which monitor is being used on a multiple monitor setup,
             *      that doesn't add much to the example.
             */
            auto monitor = glfwGetPrimaryMonitor();
            /*
             * The mode of a monitor in GLFW is an object that contains attributes of the monitor, such as
             * its resolution.
             */
            const auto* mode = glfwGetVideoMode(monitor);
            /*
             * Now switch to full-screen.
             * The second parameter is a monitor object, not NULL, so that indicates full-screen.
             * It isn't necessary to use the resolution of the monitor for width and height.
             *      However, using other values will force a change of resolution
             */
            glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        }
        else
        {
            /*
             * This code is reached if F11 was pressed while the application is in full-screen mode.
             * Using nullptr as the second argument signals that the application should go into windowed mode.
             * The size and position of the window should be the ones stored when going into full-screen,
             *      so the old window will be restored.
             */
            glfwSetWindowMonitor(window, nullptr, old_xpos, old_ypos, old_width, old_height, 0);

        }
    }
    else
    {

    }
//    view->camera_position.z = z0;
}

/*
 * This is a callback registered in function main.
 * This function is called when the size of the window changes.
 */
void window_size_callback(GLFWwindow* window, int width, int height)
{
    /*
     * grab the view since we will be changing the perspective aspect.
     */
    auto* view = static_cast<cs4722::view*>(glfwGetWindowUserPointer(window));
    /*
     * Find out the width and height of the framebuffer for the window.
     * This is evidently the best way to find out how big the window is.
     *      The framebuffer is sized to match the window size.
     * Notice that the addresses of the width and height variables are passed to the
     *      glfwGetFramebufferSize function.
     */
    int widthfb, heightfb;
    glfwGetFramebufferSize(window, &widthfb, &heightfb);
    /*
     * The viewport is the amount of the window to use to display the rendered scene.
     * The parameters used here to set the viewport cause the viewport to cover the whole window.
     */
    glViewport(0, 0, widthfb, heightfb);
    /*
     * Change the perspective aspect.
     * Otherwise, the scene will be distorted if the aspect ratio is changed.
     */
    view->perspective_aspect =
            (static_cast<float>(widthfb) / static_cast<float>(heightfb));

}

// used in mouse motion
static double last_x = 0.0;
static double last_y = 0.0;

void move_callback(GLFWwindow* window, double xpos, double ypos)
{
    static auto last_x = 0.0f;
    static auto last_y = 0.0f;
    auto* view = static_cast<cs4722::view*>(glfwGetWindowUserPointer(window));
    auto dx = xpos - last_x;
    auto dy = ypos - last_y;
    last_x = xpos;
    last_y = ypos;

    if(view->enable_logging) {
        view->display_parameters();
    }


    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT))
    {
        // std::cout << "(" << xpos << "," << ypos << ")" << std::endl;
        // auto const rot_lr = glm::mat3(glm::rotate(glm::identity<glm::mat4>(),
        //     static_cast<float>(-dx * lr_pan_scale), camera_up_));
//        auto const rot_lr = glm::mat3(glm::rotate(glm::identity<glm::mat4>(),
//                                                  static_cast<float>(-dx * lr_pan_scale), glm::vec3(0, 1, 0)));
        auto const rot_lr = glm::mat3(glm::rotate(glm::identity<glm::mat4>(),
                              static_cast<float>(-dx * lr_pan_scale), glm::vec3(0, 0, 1)));
        view->camera_left = rot_lr * view->camera_left;
        view->camera_forward = rot_lr * view->camera_forward;
        view->camera_up = rot_lr * view->camera_up;

        auto const rot_ud = glm::mat3(glm::rotate(glm::identity<glm::mat4>(),
                                                  static_cast<float>(dy * ud_pan_scale), view->camera_left));
        view->camera_up = rot_ud * view->camera_up;
        view->camera_forward = rot_ud * view->camera_forward;
    }
}
