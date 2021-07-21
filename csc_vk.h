/*
SPDX-License-Identifier: GPL-2.0
SPDX-FileCopyrightText: 2021 Johan Söderlind Åström <johan.soderlind.astrom@gmail.com>
*/
#pragma once

#include <vulkan/vulkan.h>
#include <string.h>
#include <stdio.h>
#include "csc_assert.h"


struct csc_vk_device
{
	VkPhysicalDevice phys;
	VkDevice logical;
	uint32_t family_gfx;
	uint32_t family_present;
	uint32_t family_transfer;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
};


struct csc_vk_swapchain
{
	VkSwapchainKHR swapchain;
	VkImage * images;
	VkImageView * imageviews;
	VkFramebuffer * framebuffers;
	VkCommandBuffer * commandbuffers;
	uint32_t count;
	VkFormat format;
	VkExtent2D extent;
};


struct csc_vk_pipeline
{
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
};


struct csc_vk_renderpass
{
	VkRenderPass renderpass;
};


#define CSC_VK_LAYER_COUNT 32
int csc_vk_layer_exist (char const * layername)
{
	uint32_t count = CSC_VK_LAYER_COUNT;
	VkLayerProperties available [CSC_VK_LAYER_COUNT];
	vkEnumerateInstanceLayerProperties (&count, available);
	for (uint32_t i = 0; i < count; i ++)
	{
		TRACEF ("%s : %s", available [i].layerName, available [i].description);
		int diff = strcmp (layername, available [i].layerName);
		if (diff == 0) {return 1;}
	}
	return 0;
}

int csc_vk_layers_exist (char const * layername [], int n)
{
	for (int i = 0; i < n; i ++)
	{
		if (csc_vk_layer_exist (layername [i]) == 0)
		{
			return 0;
		}
	}
	return 1;
}


void csc_vk_pinfo (void const * item)
{
	struct csc_vk_struct
	{
		VkStructureType sType;
	};
	struct csc_vk_struct const * p = (struct csc_vk_struct *) item;
	switch (p->sType)
	{
	case VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO:{
		VkInstanceCreateInfo const * info = (VkInstanceCreateInfo const *)item;
		for (uint32_t i = 0; i < info->enabledExtensionCount; ++i)
		{
			printf ("ppEnabledLayerNames %s\n", info->ppEnabledExtensionNames [i]);
		}
		break;}
	default:
		break;
	}
}

void csc_vk_pinfo_qflags (VkQueueFlags flags)
{
	if (flags & VK_QUEUE_GRAPHICS_BIT) {printf ("VK_QUEUE_GRAPHICS_BIT ");}
	if (flags & VK_QUEUE_COMPUTE_BIT) {printf ("VK_QUEUE_COMPUTE_BIT ");}
	if (flags & VK_QUEUE_TRANSFER_BIT) {printf ("VK_QUEUE_TRANSFER_BIT ");}
	if (flags & VK_QUEUE_SPARSE_BINDING_BIT) {printf ("VK_QUEUE_SPARSE_BINDING_BIT ");}
	if (flags & VK_QUEUE_PROTECTED_BIT) {printf ("VK_QUEUE_PROTECTED_BIT ");}
	if (flags & VK_QUEUE_FLAG_BITS_MAX_ENUM) {printf ("VK_QUEUE_FLAG_BITS_MAX_ENUM ");}
}


void csc_vk_pinfo_qfp (VkQueueFamilyProperties * qfp)
{
	printf ("%30s ", "queueFlags");
	csc_vk_pinfo_qflags (qfp->queueFlags);
	printf ("\n");
	printf ("%30s %10i\n", "queueCount", qfp->queueCount);
}


void csc_vk_pinfo_qf (VkPhysicalDevice dev, VkSurfaceKHR s)
{
	VkQueueFamilyProperties q [10];
	uint32_t n = 10;
	vkGetPhysicalDeviceQueueFamilyProperties (dev, &n, NULL);
	vkGetPhysicalDeviceQueueFamilyProperties (dev, &n, q);
	uint32_t i = UINT32_MAX;
	for (i = 0; i < n; ++i)
	{
		csc_vk_pinfo_qfp (q + i);
		VkBool32 psupport = VK_FALSE;
		vkGetPhysicalDeviceSurfaceSupportKHR (dev, i, s, &psupport);
		printf ("%30s %10i\n", "presentSupport", psupport);
	}
}


void csc_vk_find_famqueue3
(VkPhysicalDevice dev, VkSurfaceKHR s, uint32_t * fam_gfx, uint32_t * fam_present, uint32_t * fam_transfer)
{
	ASSERT (dev);
	VkQueueFamilyProperties q [10];
	uint32_t n = 10;
	vkGetPhysicalDeviceQueueFamilyProperties (dev, &n, NULL);
	vkGetPhysicalDeviceQueueFamilyProperties (dev, &n, q);
	uint32_t i = UINT32_MAX;
	for (i = 0; i < n; ++i)
	{
		if (q [i].queueCount == 0) {continue;}

		if (fam_present && s && (*fam_present) == UINT32_MAX)
		{
			VkBool32 present = VK_FALSE;
			vkGetPhysicalDeviceSurfaceSupportKHR (dev, i, s, &present);
			if (present == VK_TRUE)
			{
				(*fam_present) = i;
			}
		}

		if (fam_gfx && (*fam_gfx) == UINT32_MAX && (q [i].queueFlags & VK_QUEUE_GRAPHICS_BIT))
		{
			(*fam_gfx) = i;
		}

		if (fam_transfer && (*fam_transfer) == UINT32_MAX && (q [i].queueFlags & (VK_QUEUE_TRANSFER_BIT|VK_QUEUE_GRAPHICS_BIT)) == VK_QUEUE_TRANSFER_BIT)
		{
			(*fam_transfer) = i;
		}
	}
}
