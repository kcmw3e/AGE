#include <stdbool.h>

#include "utils/vk_utils.h"

#include "AGE.h"

struct AGE_core_s {
    bool is_init;
};

static struct AGE_core_s AGE_core;


void AGE_init() {
    int glfw_init_result = glfwInit();
    if (glfw_init_result != GLFW_TRUE) goto AGE_init_failed;

    vk_init();
    if (!is_vk_init()) goto AGE_init_failed;

    AGE_core.is_init = true;
    return;

    AGE_init_failed:
    xerr(XERR_FAILURE);
    AGE_core.is_init = false;
}

void AGE_term() {
    vk_term();

    glfwTerminate();
    AGE_core.is_init = false;
}

bool is_AGE_init() {
    return AGE_core.is_init;
}