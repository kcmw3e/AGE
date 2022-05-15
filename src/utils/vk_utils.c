#include "core/AGE_err.h"

#include "utils/vk_utils.h"

bool check_exts(arrayable_t sup_exts, arrayable_t exts);
compare_result_t qfams_match(xbyte_t* arg_props_p, xbyte_t* arg_match_props_p, void* args);
compare_result_t ext_prop_str_match(xbyte_t* prop, xbyte_t* str, void* args);
compare_result_t ext_props_match(xbyte_t* ext1, xbyte_t* ext2, void* args);

void vk_get_sup_exts(arrayable_t* sup_exts_p) {
    VkResult result;
    
    VkExtensionProperties* exts = NULL;
    uint32_t exts_len = 0;

    result = vkEnumerateInstanceExtensionProperties(NULL, &exts_len, NULL);
    if (result != VK_SUCCESS) goto failed;

    exts = xmalloc(exts_len*sizeof(*exts));
    if (exts == NULL) goto failed;

    result = vkEnumerateInstanceExtensionProperties(NULL, &exts_len, exts);
    if (result != VK_SUCCESS) goto failed;

    sup_exts_p->bytes = (xbyte_t*)exts;
    sup_exts_p->elem_size = sizeof(*exts);
    sup_exts_p->len = (size_t)exts_len;
    return;

    failed:
    free(exts);
    xerr(XERR_UNDEFINED);
    sup_exts_p->bytes = NULL;
    sup_exts_p->elem_size = 0;
    sup_exts_p->len = 0;
}

void vk_get_req_exts(arrayable_t* req_exts_p) {
    str_t* req_exts = NULL;
    uint32_t req_exts_len = 0;

    uint32_t glfw_req_exts_len;
    str_t* glfw_req_exts = (str_t*)glfwGetRequiredInstanceExtensions(&glfw_req_exts_len);
    if (glfw_req_exts == NULL) goto failed;

    // for future in case any other extensions are to be required
    uint32_t other_exts_len = 0;
    str_t* other_exts = NULL;

    req_exts_len = glfw_req_exts_len + other_exts_len;
    req_exts = xmalloc(req_exts_len*sizeof(*req_exts));
    if (req_exts == NULL) goto failed;

    xmemcpy((xbyte_t*)glfw_req_exts, (xbyte_t*)req_exts, glfw_req_exts_len*sizeof(*glfw_req_exts));
    xmemcpy((xbyte_t*)other_exts, (xbyte_t*)req_exts, other_exts_len*sizeof(*other_exts));
    
    req_exts_p->bytes = (xbyte_t*)req_exts;
    req_exts_p->elem_size = sizeof(*req_exts);
    req_exts_p->len = req_exts_len;
    return;

    failed:
    xerr(XERR_UNDEFINED);
    free(req_exts);
    req_exts_p->bytes = NULL;
    req_exts_p->elem_size = 0;
    req_exts_p->len = 0;
}

void vk_pdev_get_qfam_props(VkPhysicalDevice pdev, arrayable_t* qfam_props_p) {
    VkQueueFamilyProperties* props = NULL;
    uint32_t props_len = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(pdev, &props_len, NULL);

    props = xmalloc(props_len*sizeof(*props));
    if (props == NULL) goto failed;

    vkGetPhysicalDeviceQueueFamilyProperties(pdev, &props_len, props);

    qfam_props_p->bytes = (xbyte_t*)props;
    qfam_props_p->elem_size = sizeof(*props);
    qfam_props_p->len = (size_t)props_len;
    return;

    failed:
    free(props);
    xerr(XERR_UNDEFINED);
    qfam_props_p->bytes = NULL;
    qfam_props_p->elem_size = 0;
    qfam_props_p->len = 0;
}

void vk_pdev_get_sup_exts(VkPhysicalDevice pdev, arrayable_t* sup_exts_p) {
    VkResult result;

    VkExtensionProperties* exts = NULL;
    uint32_t exts_len = 0;

    result = vkEnumerateDeviceExtensionProperties(pdev, NULL, &exts_len, NULL);
    if (result != VK_SUCCESS) goto failed;

    exts = xmalloc(exts_len*sizeof(*exts));
    if (exts == NULL) goto failed;

    result = vkEnumerateDeviceExtensionProperties(pdev, NULL, &exts_len, exts);
    if (result != VK_SUCCESS) goto failed;

    sup_exts_p->bytes = (xbyte_t*)exts;
    sup_exts_p->elem_size = sizeof(*exts);
    sup_exts_p->len = exts_len;
    return;

    failed:
    free(exts);
    xerr(XERR_UNDEFINED);
    sup_exts_p->bytes = NULL;
    sup_exts_p->elem_size = 0;
    sup_exts_p->len = 0;
}

bool vk_exts_supported(arrayable_t exts) {
    arrayable_t sup_exts = QUICKRAY_EMPTY(NULL, 0);

    vk_get_sup_exts(&sup_exts);

    if (sup_exts.bytes == NULL) {
        xerr(XERR_UNDEFINED);
        return false;
    }

    bool exts_supported = check_exts(sup_exts, exts);
    free(sup_exts.bytes);
    return exts_supported;
}

bool vk_pdev_has_qfam(VkPhysicalDevice pdev, VkQueueFamilyProperties props) {
    arrayable_t qfam_props;
    vk_pdev_get_qfam_props(pdev, &qfam_props);

    if (qfam_props.bytes == NULL) {
        xerr(XERR_UNDEFINED);
        return false;
    }

    bool has_qfam = array_has(qfam_props, (xbyte_t*)(&props), &qfams_match, NULL);
    free(qfam_props.bytes);
    return has_qfam;
}

bool vk_pdev_exts_supported(VkPhysicalDevice pdev, arrayable_t exts) {
    arrayable_t sup_exts = QUICKRAY_EMPTY(NULL, 0);
    
    vk_pdev_get_sup_exts(pdev, &sup_exts);

    if (sup_exts.bytes == NULL) {
        xerr(XERR_UNDEFINED);
        return false;
    }

    bool exts_supported = check_exts(sup_exts, exts);
    free(sup_exts.bytes);
    return exts_supported;
}


bool check_exts(arrayable_t sup_exts, arrayable_t exts) {
    compare_fn* compare_fn = NULL;
    void* args = NULL;

    size_t max_ext_name_len = VK_MAX_EXTENSION_NAME_SIZE;

    switch(exts.elem_size) {
        case sizeof(str_t):
            compare_fn = &ext_prop_str_match;
            args = &max_ext_name_len;
            break;
        case sizeof(VkExtensionProperties):
            compare_fn = &ext_props_match;
            args = NULL;
            break;
        default:
            xerr(XERR_UNDEFINED);
            return false;
    }

    return array_hasall(sup_exts, exts, compare_fn, args);
}

compare_result_t qfams_match(xbyte_t* props_p, xbyte_t* match_props_p, void* args) {
    UNUSED(args);

    VkQueueFamilyProperties props = *((VkQueueFamilyProperties*)props_p);
    VkQueueFamilyProperties match_props = *((VkQueueFamilyProperties*)match_props_p);

    // properties to check
    VkQueueFlags flags = props.queueFlags;
    uint32_t queue_count = props.queueCount;
    uint32_t timestamp_valid_bits = props.timestampValidBits;
    VkExtent3D min_img_transfer_granularity = props.minImageTransferGranularity;

    // properties to match
    VkQueueFlags match_flags = match_props.queueFlags;
    uint32_t match_queue_count = match_props.queueCount;
    uint32_t match_timestamp_valid_bits = match_props.timestampValidBits;
    VkExtent3D match_min_img_transfer_granularity = match_props.minImageTransferGranularity;

    // if any match property is not 0, check if present in test properties and return
    //  [COMPARISON_NOT_EQUAL] if not
    if (match_flags != 0
     && !(flags & match_flags))
            return COMPARISON_NOT_EQUAL;

    if (match_queue_count != 0
     && queue_count != match_queue_count)
            return COMPARISON_NOT_EQUAL;
     
    if (match_timestamp_valid_bits != 0
     && timestamp_valid_bits != match_timestamp_valid_bits)
            return COMPARISON_NOT_EQUAL;

    if (match_min_img_transfer_granularity.width != 0
     && min_img_transfer_granularity.width != match_min_img_transfer_granularity.width)
            return COMPARISON_NOT_EQUAL;

    if (match_min_img_transfer_granularity.height != 0
     && min_img_transfer_granularity.height != match_min_img_transfer_granularity.height)
            return COMPARISON_NOT_EQUAL;

    if (match_min_img_transfer_granularity.depth != 0
     && min_img_transfer_granularity.depth != match_min_img_transfer_granularity.depth)
            return COMPARISON_NOT_EQUAL;
    
    return COMPARISON_EQUAL;
}

compare_result_t ext_prop_str_match(xbyte_t* prop_p, xbyte_t* str_p, void* max_name_len_p) {
    VkExtensionProperties prop = *((VkExtensionProperties*)prop_p);
    str_t str = *((str_t*)str_p);
    size_t max_name_len = *((size_t*)max_name_len_p);

    bool eq = str_eq(prop.extensionName, str, max_name_len);
    if (eq) return COMPARISON_EQUAL;
    return COMPARISON_NOT_EQUAL;
}

compare_result_t ext_props_match(xbyte_t* prop1_p, xbyte_t* prop2_p, void* args) {
    UNUSED(args);

    VkExtensionProperties prop1 = *((VkExtensionProperties*)prop1_p);
    VkExtensionProperties prop2 = *((VkExtensionProperties*)prop2_p);

    bool eq = str_eq(prop1.extensionName, prop2.extensionName, VK_MAX_EXTENSION_NAME_SIZE);
    if (eq) return COMPARISON_EQUAL;
    return COMPARISON_NOT_EQUAL;
}
