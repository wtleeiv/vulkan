/* glfw will automatically: #include <vulkan/vulkan.h> */
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>

const int WIDTH = 800;
const int HEIGHT = 600;

typedef struct {
  GLFWwindow *window;
} State;

GLFWwindow* initWindow(int width, int height) {
  glfwInit();
  /* Don't use OpenGL */
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  /* Non-sizeable window for now */
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  return glfwCreateWindow(width, height, "Vulkan", NULL, NULL);
}

void mainLoop(GLFWwindow *window) {
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }
}

void cleanup(GLFWwindow *window) {
  glfwDestroyWindow(window);
  glfwTerminate();
}

void run() {
  State state;
  state.window = initWindow(WIDTH, HEIGHT);
  //initVulkan();
  mainLoop(state.window);
  cleanup(state.window);
};

int main() {
  run();
}
