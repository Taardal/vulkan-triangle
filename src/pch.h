#pragma once

// Standard library
#include <algorithm>
#include <exception>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <vector>

// 3rd-party
#include <vulkan/vulkan.h> // Vulkan must be included before GLFW
#include <GLFW/glfw3.h>

// App
#include "system/assert.h"
#include "system/error.h"
#include "system/error_signal.h"
#include "system/environment.h"
#include "system/log.h"
#include "system/numbers.h"
#include "system/ref.h"
#include "system/utils.h"
