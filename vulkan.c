/* glfw will automatically: #include <vulkan/vulkan.h> */
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

const char *NAME = "Hello Triangle";
const int WIDTH = 800;
const int HEIGHT = 600;

typedef struct {
  GLFWwindow *window;
  VkInstance instance;
} State;

GLFWwindow* initWindow(const int width, const int height, const char *name) {
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW.\n");
    exit(EXIT_FAILURE);
  }
  /* Don't use OpenGL */
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  /* Non-sizeable window for now */
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  GLFWwindow *window = glfwCreateWindow(width, height, name, NULL, NULL);
  if (!window) {
    fprintf(stderr, "Failed to create a window.\n");
    exit(EXIT_FAILURE);
  }
  return window;
}

// TODO: try making this const
void mainLoop(GLFWwindow *window) {
  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }
}

bool vetReqdExts(uint32_t glfwExtCount, const char **glfwExts) {
  /* find available extensions */
  uint32_t extCount = 0;
  vkEnumerateInstanceExtensionProperties(NULL, &extCount, NULL);
  VkExtensionProperties extProps[extCount];
  vkEnumerateInstanceExtensionProperties(NULL, &extCount, extProps);
  /* ensure all required extensions are present */
  bool allFound = true;
  for (uint32_t i = 0; i < glfwExtCount; ++i) {
    bool currFound = false;
    for (uint32_t j = 0; j < extCount; ++j) {
      if (strcmp(glfwExts[i], extProps[j].extensionName)) {
	currFound = true;
	break;
      }
    }
    if (!currFound) {
      fprintf(stderr, "Required extension %s is not available.\n", glfwExts[i]);
      allFound = false;
    }
  }
  return allFound;
}

VkInstance createInstance(const char *name) {
  /* optional struct */
  VkApplicationInfo appInfo = {
    .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
    .pApplicationName = name,
    .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
    .pEngineName = "No Engine",
    .engineVersion = VK_MAKE_VERSION(1, 0, 0),
    /* only version available */
    .apiVersion = VK_API_VERSION_1_0
  };
  /* get glfw extension info */
  unsigned int glfwExtensionCount = 0;
  const char **glfwExtensions;
  glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  /* ensure all required extensions are present */
  if (!vetReqdExts(glfwExtensionCount, glfwExtensions)) {
    return NULL;
  }
  /* required struct */
  VkInstanceCreateInfo createInfo = {
    .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
    .pApplicationInfo = &appInfo,
    .enabledExtensionCount = glfwExtensionCount,
    .ppEnabledExtensionNames = glfwExtensions,
    .enabledLayerCount = 0
  };
  /* NOTE: must create vulkan instance, then pass ref to vkCreateInstance */
  /* just creating and passing an instance pointer leads to a segfault */
  /* also NOTE: must check against VK_SUCCESS */
  /* just checking !vkCreateInstance is the opposite you would expect */
  /* since Vk_SUCCESS is an enum bound to 0 -> false */
  VkInstance instance;
  if (vkCreateInstance(&createInfo, NULL, &instance) != VK_SUCCESS) {
    fprintf(stderr, "Failed to create instance.\n");
    return NULL;
  }
  return instance;
}

VkInstance initVulkan(const char *name) {
  return createInstance(name);
}

void cleanup(GLFWwindow *window, VkInstance instance) {
  vkDestroyInstance(instance, NULL);
  glfwDestroyWindow(window);
  glfwTerminate();
}

void run() {
  State state;
  state.window = initWindow(WIDTH, HEIGHT, NAME);
  state.instance = initVulkan(NAME);
  if (state.instance) {
    mainLoop(state.window);
  }
  cleanup(state.window, state.instance);
};

int main() {
  run();
}
