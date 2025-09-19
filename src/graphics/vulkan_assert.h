#pragma once

#define GM_ASSERT_VK_SUCCESS(expression, message) GM_ASSERT(expression == VK_SUCCESS, message)
#define GM_ASSERT_THROW_VK_SUCCESS(expression, message) GM_ASSERT_THROW(expression == VK_SUCCESS, message)