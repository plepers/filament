/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef TNT_FILAMENT_BACKEND_PLATFORMS_VULKANPLATFORM_H
#define TNT_FILAMENT_BACKEND_PLATFORMS_VULKANPLATFORM_H

#include <backend/Platform.h>

#include <bluevk/BlueVK.h>
#include <utils/FixedCapacityVector.h>

#include <unordered_set>

namespace filament::backend {

/**
 * A Platform interface that creates a Vulkan backend.
 */

class VulkanPlatform : public Platform {
public:
    ~VulkanPlatform() override;

    Driver* createDriver(void* const sharedContext,
            Platform::DriverConfig const& driverConfig) noexcept override;

    int getOSVersion() const noexcept override { return 0; }


    /**
     * Clean up any resources owned by the Platform. For example, if the Vulkan instance handle was
     * generated by the platform, we need to clean it up in this method.
     */
    virtual void terminate();

    /**
     * @return The instance (VkInstance) for the Vulkan backend.
     */
    inline VkInstance getInstance() const noexcept { return mInstance; }

    /**
     * @return The logical device (VkDevice) that was selected as the backend device.
     */
    inline VkDevice getDevice() const noexcept { return mDevice; }

    /**
     * @return The physical device (i.e gpu) that was selected as the backend physical device.
     */
    inline VkPhysicalDevice getPhysicalDevice() const noexcept { return mPhysicalDevice; }

    /**
     * @return The family index of the graphics queue selected for the Vulkan backend.
     */
    inline uint32_t getGraphicsQueueFamilyIndex() const noexcept {
        return mGraphicsQueueFamilyIndex;
    }

    /**
     * @return The index of the graphics queue (if there are multiple graphics queues)
     *         selected for the Vulkan backend.
     */
    inline uint32_t getGraphicsQueueIndex() const noexcept { return mGraphicsQueueIndex; }

    /**
     * @return The queue that was selected for the Vulkan backend.
     */
    inline VkQueue getGraphicsQueue() const noexcept { return mGraphicsQueue; }

    // TODO: move this to private when swapchains are created via platform.
    struct SurfaceBundle {
        void* surface;
        // On certain platforms, the extent of the surface cannot be queried from Vulkan. In those
        // situations, we allow the frontend to pass in the extent to use in creating the swap
        // chains. Platform implementation should set extent to 0 if they do not expect to set the
        // swap chain extent.
        uint32_t width;
        uint32_t height;
    };
    static SurfaceBundle createVkSurfaceKHR(void* nativeWindow, VkInstance instance,
            uint64_t flags) noexcept;

private:
    using ExtensionSet = std::unordered_set<std::string_view>;
    static ExtensionSet getRequiredInstanceExtensions();

protected:
    VkInstance mInstance = VK_NULL_HANDLE;
    VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
    VkDevice mDevice = VK_NULL_HANDLE;
    uint32_t mGraphicsQueueFamilyIndex = 0xFFFFFFFF;
    uint32_t mGraphicsQueueIndex = 0;

    VkQueue mGraphicsQueue = VK_NULL_HANDLE;
};

}// namespace filament::backend

#endif//TNT_FILAMENT_BACKEND_PLATFORMS_VULKANPLATFORM_H
