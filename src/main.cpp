#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <algo/shader.h>
#include <algo/camera.h>
#include <algo/model.h>
#include <algo/cube.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 pointPosition(-5.0f, 5.0f, 0.0f);

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

float current_scale = 1.0f;
float current_omega = glm::radians(50.0f);
float current_theta = 0.0f;

float current_fai = 0.0f;
float dfai_dt = glm::radians(30.f);

const auto axis = glm::normalize(glm::vec3 {-1, 1, 0});

struct draw_context {
    Model &model;
    Shader &shader;
    const glm::mat4 &view;
    const glm::mat4 &projection;
    glm::vec3 position;
    float scale;
    float theta;
};

void set_light(Shader& lightingShader);
void draw_model(const draw_context &context);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // tell stb_image.h to flip loaded texture's on the y-axis (before loading model).
    stbi_set_flip_vertically_on_load(true);

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile shaders
    // -------------------------
    Shader ourShader("colors.vs", "colors.fs");
    Shader pointShader("light_cube.vs", "light_cube.fs");

    // load models
    // -----------
    Model ourModel("model/model.obj");
    cube point(pointPosition);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        auto currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        current_theta += current_omega * deltaTime;
        current_fai   += dfai_dt * deltaTime;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        const glm::mat4 view = camera.GetViewMatrix();
        const auto r = 3.0f;

        draw_context context{ourModel, ourShader, view, projection};

        context.position = glm::vec3(0.0f);
        context.theta    = current_fai;
        context.scale    = current_scale;
        draw_model(context);

        const auto position = glm::rotate(glm::mat4(1.0f), current_theta, axis) * glm::vec4(0, 0, 1, 1);
//        context.position = glm::vec3(r * cos(current_theta), r * sin(current_theta), 0.0f);
        context.position = position * r;
        context.theta    = current_fai;
        context.scale    = 0.5f;
        draw_model(context);

        point.Draw(pointShader, view, projection);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    const auto stride = 1.0f;
    const auto rstride = 2.0f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) {
        current_scale = min(2.0f, current_scale + stride * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        current_scale = max(0.3f, current_scale - stride * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) {
        current_omega = min(4.0f, current_omega + rstride * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        current_omega = max(-4.0f, current_omega - rstride * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        dfai_dt = min(4.0f, dfai_dt + rstride * deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
        dfai_dt = max(-4.0f, dfai_dt - rstride * deltaTime);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    auto xpos = static_cast<float>(xposIn);
    auto ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void set_light(Shader& lightingShader) {
    lightingShader.setVec3("light.position", camera.Position);
    lightingShader.setVec3("light.direction", camera.Front);
    lightingShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
    lightingShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));
    lightingShader.setVec3("viewPos", camera.Position);

    // light properties
    lightingShader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
    // we configure the diffuse intensity slightly higher; the right lighting conditions differ with each lighting method and environment.
    // each environment and lighting type requires some tweaking to get the best out of your environment.
    lightingShader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader.setVec3("light.specular", 0.5f, 0.5f, 0.5f);
    lightingShader.setFloat("light.constant", 1.0f);
    lightingShader.setFloat("light.linear", 0.09f);
    lightingShader.setFloat("light.quadratic", 0.032f);


    lightingShader.setVec3("point.position", pointPosition);
    lightingShader.setVec3("point.ambient", 0.05f, 0.05f, 0.05f);
    lightingShader.setVec3("point.diffuse", 0.8f, 0.8f, 0.8f);
    lightingShader.setVec3("point.specular", 1.0f, 1.0f, 1.0f);
    lightingShader.setFloat("point.constant", 1.0f);
    lightingShader.setFloat("point.linear", 0.045f);
    lightingShader.setFloat("point.quadratic", 0.0075f);
}

void draw_model(const draw_context &context) {
    // don't forget to enable shader before setting uniforms
    context.shader.use();
    set_light(context.shader);

    context.shader.setMat4("projection", context.projection);
    context.shader.setMat4("view", context.view);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, context.position); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(context.scale));	// it's a bit too big for our scene, so scale it down
    model = glm::rotate(model, context.theta, glm::vec3(0.0f, 1.0f, 0.0f));
    context.shader.setMat4("model", model);

    context.model.Draw(context.shader);
}
