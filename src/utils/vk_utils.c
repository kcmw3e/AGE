#include "utils/vk_utils.h"

struct vk_core_s {
    bool is_init;
    uint32_t req_exts_len;
    str_t* req_exts_a;
};

static struct vk_core_s vk_core;


void vk_init_req_exts();

bool qfams_match(VkQueueFamilyProperties* props_p, VkQueueFamilyProperties* match_props_p);

void vk_init() {
    if (vk_core.is_init) return;

    vk_init_req_exts();
    if (vk_core.req_exts_a == NULL) goto failed;

    vk_core.is_init = true;
    return;

    failed:
    xerr(XERR_UNDEFINED);
    vk_core.is_init = false;
}

void vk_term() {
    vk_core.is_init = false;
    
    free(vk_core.req_exts_a);
    vk_core.req_exts_a = NULL;
}

bool is_vk_init() {
    return vk_core.is_init;
}


void vk_init_req_exts() {
    str_t* req_exts_a = NULL;

    uint32_t glfw_req_exts_len;
    str_t* glfw_req_exts_a = (str_t*)glfwGetRequiredInstanceExtensions(&glfw_req_exts_len);
    if (glfw_req_exts_a == NULL) goto failed;

    // for future in case any other extensions are to be required
    uint32_t additional_exts_len = 0;
    str_t* additional_exts_a = NULL;

    uint32_t req_exts_len = glfw_req_exts_len + additional_exts_len;

    req_exts_a = xmalloc(req_exts_len*sizeof(*req_exts_a));
    if (req_exts_a == NULL) goto failed;

    for (uint32_t i = 0; i < glfw_req_exts_len; i++) req_exts_a[i] = glfw_req_exts_a[i];
    
    for (uint32_t i = 0; i < additional_exts_len; i++)
        req_exts_a[glfw_req_exts_len + i] = additional_exts_a[i];

    if (!vk_exts_supported(req_exts_a, req_exts_len, VK_MAX_EXTENSION_NAME_SIZE)) goto failed;

    vk_core.req_exts_len = req_exts_len;
    vk_core.req_exts_a = req_exts_a;

    return;

    failed:
    xerr(XERR_UNDEFINED);
    free(req_exts_a);
    vk_core.req_exts_len = 0;
    vk_core.req_exts_a = NULL;
}


bool vk_exts_supported(str_t* exts_a, uint32_t exts_len, size_t n) {
    VkExtensionProperties* supp_exts_props_a;
    str_t* supp_exts_a;
    
    bool exts_supported = false;

    uint32_t supp_exts_len;
    VkResult result;
    result = vkEnumerateInstanceExtensionProperties(NULL, &supp_exts_len, NULL);
    if (result != VK_SUCCESS) goto failed;

    supp_exts_props_a = xmalloc(supp_exts_len*sizeof(*supp_exts_props_a));
    if (supp_exts_props_a == NULL) goto failed;

    result = vkEnumerateInstanceExtensionProperties(NULL, &supp_exts_len, supp_exts_props_a);
    if (result != VK_SUCCESS) goto failed;

    supp_exts_a = xmalloc(supp_exts_len*sizeof(*supp_exts_a));
    if (supp_exts_a == NULL) goto failed;

    for (uint32_t i = 0; i < supp_exts_len; i++)
        supp_exts_a[i] = supp_exts_props_a[i].extensionName;

    for (uint32_t i = 0; i < exts_len; i++) {
        if (!str_in_strs(exts_a[i], supp_exts_a, supp_exts_len, MIN(VK_MAX_EXTENSION_NAME_SIZE, n)))
            goto cleanup; // [exts_supported] already false, so clean everything up and return
    }
    exts_supported = true;
    goto cleanup;

    failed:
    xerr(XERR_UNDEFINED);
    exts_supported = false;

    cleanup:
    free(supp_exts_a);
    free(supp_exts_props_a);
    return exts_supported;   
}

uint32_t vk_get_req_exts_len() {
    return vk_core.req_exts_len;
}

str_t* vk_get_req_exts() {
    return vk_core.req_exts_a;
}

uint32_t vk_pdev_get_qfam_props_len(VkPhysicalDevice pdev) {
    uint32_t len = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(pdev, &len, NULL);
    return len;
}

VkQueueFamilyProperties* vk_pdev_get_qfam_props(VkPhysicalDevice pdev) {
    uint32_t len = vk_pdev_get_qfam_props_len(pdev);

    VkQueueFamilyProperties* props_a = xmalloc(len*sizeof(*props_a));
    if (props_a == NULL) return NULL;

    vkGetPhysicalDeviceQueueFamilyProperties(pdev, &len, props_a);
    return props_a;
}

bool vk_pdev_has_qfam(VkPhysicalDevice pdev, VkQueueFamilyProperties* props_p) {
    uint32_t qfam_props_len = vk_pdev_get_qfam_props_len(pdev);

    VkQueueFamilyProperties* qfam_props_a = vk_pdev_get_qfam_props(pdev);
    if (qfam_props_a == NULL) goto failed;

    bool has_qfam = false;
    for (uint32_t i = 0; i < qfam_props_len; i++) {
        VkQueueFamilyProperties qfam_props = qfam_props_a[i];
        if (qfams_match(&qfam_props, props_p)) {
            has_qfam = true;
            break;
        }
    }

    goto cleanup;

    failed:
    xerr(XERR_UNDEFINED);
    has_qfam = false;

    cleanup:
    free(qfam_props_a);

    return has_qfam;
}


bool qfams_match(VkQueueFamilyProperties* props_p, VkQueueFamilyProperties* match_props_p) {
    // properties to check
    VkQueueFlags flags = props_p->queueFlags;
    uint32_t queue_count = props_p->queueCount;
    uint32_t timestamp_valid_bits = props_p->timestampValidBits;
    VkExtent3D min_img_transfer_granularity = props_p->minImageTransferGranularity;

    // properties to match
    VkQueueFlags match_flags = match_props_p->queueFlags;
    uint32_t match_queue_count = match_props_p->queueCount;
    uint32_t match_timestamp_valid_bits = match_props_p->timestampValidBits;
    VkExtent3D match_min_img_transfer_granularity = match_props_p->minImageTransferGranularity;

    // if any match property is not 0, check if present in test properties and return [false] if not
    if (match_flags != 0 && !(flags & match_flags)) return false;
    if (match_queue_count != 0 && queue_count != match_queue_count) return false;
    if (match_timestamp_valid_bits != 0
     && timestamp_valid_bits != match_timestamp_valid_bits)
        return false;
    if (match_min_img_transfer_granularity.width != 0
     && min_img_transfer_granularity.width != match_min_img_transfer_granularity.width)
        return false;
    if (match_min_img_transfer_granularity.height != 0
     && min_img_transfer_granularity.height != match_min_img_transfer_granularity.height)
        return false;
    if (match_min_img_transfer_granularity.depth != 0
     && min_img_transfer_granularity.depth != match_min_img_transfer_granularity.depth)
        return false;
    
    return true;
}
