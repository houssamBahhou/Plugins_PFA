#ifndef VULKANWINDOW_H
#define VULKANWINDOW_H

#include <QVulkanWindow>

class VulkanWindow : public QVulkanWindow
{
    QVulkanInstance inst;
public:
    VulkanWindow();

    QVulkanWindowRenderer *createRenderer() override;
};

#endif // VULKANWINDOW_H
