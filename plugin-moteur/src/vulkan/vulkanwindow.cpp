#include "vulkanwindow.h"

#include <QVulkanInstance>
#include "vulkanrenderer.h"

VulkanWindow::VulkanWindow()
{
#ifndef Q_OS_ANDROID
    inst.setLayers(QByteArrayList() << "VK_LAYER_LUNARG_standard_validation");
#else
    inst.setLayers(QByteArrayList()
                   << "VK_LAYER_GOOGLE_threading"
                   << "VK_LAYER_LUNARG_parameter_validation"
                   << "VK_LAYER_LUNARG_object_tracker"
                   << "VK_LAYER_LUNARG_core_validation"
                   << "VK_LAYER_LUNARG_image"
                   << "VK_LAYER_LUNARG_swapchain"
                   << "VK_LAYER_GOOGLE_unique_objects");
#endif

    if (!inst.create())
        qFatal("Failed to create Vulkan instance: %d", inst.errorCode());
    setVulkanInstance(&inst);
    resize(500, 500);
}

QVulkanWindowRenderer* VulkanWindow::createRenderer()
{
    return new VulkanRenderer(this, true);
}
