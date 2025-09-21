#include "vulkan_pipeline.h"
#include "system/file.h"

namespace Game {
    VkShaderModule create_shader_module(VkDevice device, const std::filesystem::path& shader_path) {
        std::vector<char> shader_bytes = read_bytes(shader_path);

        VkShaderModuleCreateInfo shader_module_create_info{};
        shader_module_create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        shader_module_create_info.codeSize = shader_bytes.size();
        shader_module_create_info.pCode = reinterpret_cast<const u32*>(shader_bytes.data());

        VkShaderModule shader_module;
        if (vkCreateShaderModule(device, &shader_module_create_info, GM_VK_ALLOCATOR, &shader_module) != VK_SUCCESS) {
            GM_THROW("Could not create Vulkan shader module");
        }

        return shader_module;
    }

    void create_vulkan_pipeline(Vulkan& vulkan, const PipelineConfig& config) {

        //
        // Programmable stages
        //

        vulkan.vertex_shader = create_shader_module(vulkan.device, config.vertex_shader_path);
        vulkan.fragment_shader = create_shader_module(vulkan.device, config.fragment_shader_path);

        std::string vertex_shader_name = std::format("{} VertexShader", config.name.c_str());
        set_vulkan_object_name(vulkan.device, vulkan.vertex_shader, VK_OBJECT_TYPE_SHADER_MODULE, vertex_shader_name.c_str());

        std::string fragment_shader_name = std::format("{} FragmentShader", config.name.c_str());
        set_vulkan_object_name(vulkan.device, vulkan.fragment_shader, VK_OBJECT_TYPE_SHADER_MODULE, fragment_shader_name.c_str());

        VkPipelineShaderStageCreateInfo vertex_shader_stage_create_info{};
        vertex_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertex_shader_stage_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertex_shader_stage_create_info.module = vulkan.vertex_shader;
        vertex_shader_stage_create_info.pName = "main";

        VkPipelineShaderStageCreateInfo fragment_shader_stage_create_info{};
        fragment_shader_stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragment_shader_stage_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragment_shader_stage_create_info.module = vulkan.fragment_shader;
        fragment_shader_stage_create_info.pName = "main";

        VkPipelineShaderStageCreateInfo shader_stage_create_infos[] = {
            vertex_shader_stage_create_info,
            fragment_shader_stage_create_info,
        };

        //
        // Fixed function stages
        //

        std::vector<VkDynamicState> dynamic_states = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
        };

        VkPipelineDynamicStateCreateInfo dynamic_state_create_info{};
        dynamic_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamic_state_create_info.dynamicStateCount = static_cast<u32>(dynamic_states.size());
        dynamic_state_create_info.pDynamicStates = dynamic_states.data();

        VkPipelineVertexInputStateCreateInfo vertex_input_state_create_info{};
        vertex_input_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertex_input_state_create_info.vertexBindingDescriptionCount = 0;
        vertex_input_state_create_info.pVertexBindingDescriptions = nullptr;
        vertex_input_state_create_info.vertexAttributeDescriptionCount = 0;
        vertex_input_state_create_info.pVertexAttributeDescriptions = nullptr;

        VkPipelineInputAssemblyStateCreateInfo input_assembly_state_create_info{};
        input_assembly_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        input_assembly_state_create_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        input_assembly_state_create_info.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = (f32) vulkan.swap_chain_extent.width;
        viewport.height = (f32) vulkan.swap_chain_extent.height;
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor{};
        scissor.offset = {0, 0};
        scissor.extent = vulkan.swap_chain_extent;

        VkPipelineViewportStateCreateInfo viewport_state_create_info{};
        viewport_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewport_state_create_info.viewportCount = 1;
        viewport_state_create_info.pViewports = &viewport;
        viewport_state_create_info.scissorCount = 1;
        viewport_state_create_info.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterization_state_create_info{};
        rasterization_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterization_state_create_info.depthClampEnable = VK_FALSE;
        rasterization_state_create_info.rasterizerDiscardEnable = VK_FALSE;
        rasterization_state_create_info.polygonMode = VK_POLYGON_MODE_FILL;
        rasterization_state_create_info.lineWidth = 1.0f;
        rasterization_state_create_info.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterization_state_create_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterization_state_create_info.depthBiasEnable = VK_FALSE;

        VkPipelineMultisampleStateCreateInfo multisample_state_create_info{};
        multisample_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisample_state_create_info.sampleShadingEnable = VK_FALSE;
        multisample_state_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        multisample_state_create_info.minSampleShading = 1.0f;
        multisample_state_create_info.pSampleMask = nullptr;
        multisample_state_create_info.alphaToCoverageEnable = VK_FALSE;
        multisample_state_create_info.alphaToOneEnable = VK_FALSE;

        // VkPipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo{};
        // depthStencilStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        // depthStencilStateCreateInfo.depthTestEnable = config.depthTestEnabled;
        // depthStencilStateCreateInfo.depthWriteEnable = config.depthTestEnabled;
        // depthStencilStateCreateInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        // depthStencilStateCreateInfo.depthBoundsTestEnable = VK_FALSE;
        // depthStencilStateCreateInfo.stencilTestEnable = VK_FALSE;

        VkPipelineColorBlendAttachmentState color_blend_attachment_state{};
        color_blend_attachment_state.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        color_blend_attachment_state.blendEnable = VK_TRUE;
        color_blend_attachment_state.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        color_blend_attachment_state.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        color_blend_attachment_state.colorBlendOp = VK_BLEND_OP_ADD;
        color_blend_attachment_state.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        color_blend_attachment_state.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        color_blend_attachment_state.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo color_blend_state_create_info{};
        color_blend_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        color_blend_state_create_info.logicOpEnable = VK_FALSE;
        color_blend_state_create_info.logicOp = VK_LOGIC_OP_COPY;
        color_blend_state_create_info.attachmentCount = 1;
        color_blend_state_create_info.pAttachments = &color_blend_attachment_state;
        color_blend_state_create_info.blendConstants[0] = 0.0f;
        color_blend_state_create_info.blendConstants[1] = 0.0f;
        color_blend_state_create_info.blendConstants[2] = 0.0f;
        color_blend_state_create_info.blendConstants[3] = 0.0f;

        //
        // Creation
        //

        VkPipelineLayoutCreateInfo pipeline_layout_create_info{};
        pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipeline_layout_create_info.setLayoutCount = 0;
        pipeline_layout_create_info.pSetLayouts = nullptr;
        pipeline_layout_create_info.pushConstantRangeCount = 0;
        pipeline_layout_create_info.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(vulkan.device, &pipeline_layout_create_info, GM_VK_ALLOCATOR, &vulkan.pipeline_layout) != VK_SUCCESS) {
            GM_THROW("Could not create Vulkan pipeline layout");
        }

        std::string pipeline_layout_name = std::format("{} Layout", config.name.c_str());
        set_vulkan_object_name(vulkan.device, vulkan.pipeline_layout, VK_OBJECT_TYPE_PIPELINE_LAYOUT, pipeline_layout_name.c_str());

        VkGraphicsPipelineCreateInfo pipeline_create_info{};
        pipeline_create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipeline_create_info.stageCount = 2;
        pipeline_create_info.pStages = shader_stage_create_infos;
        pipeline_create_info.pVertexInputState = &vertex_input_state_create_info;
        pipeline_create_info.pInputAssemblyState = &input_assembly_state_create_info;
        pipeline_create_info.pViewportState = &viewport_state_create_info;
        pipeline_create_info.pRasterizationState = &rasterization_state_create_info;
        pipeline_create_info.pMultisampleState = &multisample_state_create_info;
        pipeline_create_info.pDepthStencilState = nullptr;
        pipeline_create_info.pColorBlendState = &color_blend_state_create_info;
        pipeline_create_info.pDynamicState = &dynamic_state_create_info;
        pipeline_create_info.layout = vulkan.pipeline_layout;
        pipeline_create_info.renderPass = vulkan.swap_chain_render_pass;
        pipeline_create_info.subpass = 0;
        pipeline_create_info.basePipelineHandle = nullptr;
        pipeline_create_info.basePipelineIndex = -1;

        i32 create_info_count = 1;
        VkPipelineCache pipeline_cache = nullptr;
        if (vkCreateGraphicsPipelines(vulkan.device, pipeline_cache, create_info_count, &pipeline_create_info, GM_VK_ALLOCATOR, &vulkan.pipeline) != VK_SUCCESS) {
            GM_THROW("Could not create pipeline");
        }

        set_vulkan_object_name(vulkan.device, vulkan.pipeline, VK_OBJECT_TYPE_PIPELINE, config.name.c_str());
    }

    void destroy_vulkan_pipeline(const Vulkan& vulkan) {
        vkDestroyPipeline(vulkan.device, vulkan.pipeline, GM_VK_ALLOCATOR);
        vkDestroyPipelineLayout(vulkan.device, vulkan.pipeline_layout, GM_VK_ALLOCATOR);
        vkDestroyShaderModule(vulkan.device, vulkan.fragment_shader, GM_VK_ALLOCATOR);
        vkDestroyShaderModule(vulkan.device, vulkan.vertex_shader, GM_VK_ALLOCATOR);
    }
}
