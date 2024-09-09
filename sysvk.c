#include <dlfcn.h>
#include <hardware/hardware.h>
#include <hardware/hwvulkan.h>

int (*hw_get_module_handle)(const char *id, const struct hw_module_t *>

void _init_hw() {
  if (hw_get_module_handle != NULL) return;
  void *libhardware_handle = dlopen("libhardware.so", RTLD_LAZY);
  if (libhardware_handle == NULL) return;
  hw_get_module_handle = (int (*)(const char *, const struct hw_module>
}

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL
vk_icdGetInstanceProcAddr(VkInstance instance, const char *pName) {
  _init_hw();
  if (hw_get_module_handle == NULL) return NULL;
  hw_module_t *module;
  int ret =
      hw_get_module_handle(HWVULKAN_HARDWARE_MODULE_ID, (const hw_modu>
  if (ret) {
    return NULL;
  }
  hwvulkan_device_t *sysvk = NULL;
  module->methods->open(module, HWVULKAN_DEVICE_0, (hw_device_t **)&sy>
  if (!sysvk) {
    return NULL;
  }
  return sysvk->GetInstanceProcAddr(instance, pName);
}

PFN_vkVoidFunction vk_icdGetPhysicalDeviceProcAddr(VkInstance _instanc>
                                                   const char *pName) {
  return NULL;
}

VKAPI_ATTR VkResult VKAPI_CALL
vk_icdNegotiateLoaderICDInterfaceVersion(uint32_t *pSupportedVersion) {
#define MIN(i, j) (((i) < (j)) ? (i) : (j))
  *pSupportedVersion = MIN(*pSupportedVersion, 5u);
  return VK_SUCCESS;
}
