#include <stdbool.h>

#include "utils/vk_utils.h"

#include "AGE.h"

struct AGE_core_s {
    bool is_init;
    arrayable_t vk_req_exts;
};

static struct AGE_core_s AGE_core;

void init_vk_req_exts();
void term_vk_req_exts();

void AGE_init() {
    int glfw_init_result = glfwInit();
    if (glfw_init_result != GLFW_TRUE) goto failed;

    init_vk_req_exts();
    if (AGE_core.vk_req_exts.bytes == NULL) goto failed;

    AGE_core.is_init = true;
    return;

    failed:
    xerr(XERR_UNDEFINED);
    AGE_core.is_init = false;
}

void AGE_term() {
    free(AGE_core.vk_req_exts.bytes);

    glfwTerminate();
    AGE_core.is_init = false;
}

bool is_AGE_init() {
    return AGE_core.is_init;
}

str_t* AGE_get_vk_req_exts() {
    return (str_t*)AGE_core.vk_req_exts.bytes;
}

uint32_t AGE_get_vk_req_exts_len() {
    return (uint32_t)AGE_core.vk_req_exts.len;
}


void init_vk_req_exts() {
    arrayable_t* req_exts_p = &(AGE_core.vk_req_exts);
    str_t* req_ext_names = NULL;
    uint32_t req_exts_len = 0;

    uint32_t glfw_req_exts_len;
    str_t* glfw_req_exts = (str_t*)glfwGetRequiredInstanceExtensions(&glfw_req_exts_len);
    if (glfw_req_exts == NULL) goto failed;

    // for future in case any other extensions are to be required
    uint32_t other_exts_len = 0;
    str_t* other_exts = NULL;

    req_exts_len = glfw_req_exts_len + other_exts_len;
    req_ext_names = xmalloc(req_exts_len*sizeof(*req_ext_names));
    if (req_ext_names == NULL) goto failed;

    xmemcpy((xbyte_t*)glfw_req_exts, (xbyte_t*)req_ext_names, glfw_req_exts_len*sizeof(*glfw_req_exts));
    xmemcpy((xbyte_t*)other_exts, (xbyte_t*)req_ext_names, other_exts_len*sizeof(*other_exts));
    
    req_exts_p->bytes = (xbyte_t*)req_ext_names;
    req_exts_p->elem_size = sizeof(*req_ext_names);
    req_exts_p->len = req_exts_len;
    return;

    failed:
    xerr(XERR_UNDEFINED);
    free(req_ext_names);
    req_exts_p->bytes = NULL;
    req_exts_p->elem_size = 0;
    req_exts_p->len = 0;
}

void term_vk_req_exts() {
    free(AGE_core.vk_req_exts.bytes);
}
