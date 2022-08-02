#pragma once

// nnabla core
#include <IsNdArray/context.hpp>
#include <IsNdArray/global_context.hpp>
#include <IsNdArray/memory/cpu_memory.hpp>
#include <IsNdArray/memory/allocator.hpp>
#include <IsNdArray/memory/naive_allocator.hpp>
#include <IsNdArray/memory/caching_allocator_with_buckets.hpp>
#include <IsNdArray/array.hpp>
#include <IsNdArray/array_registry.hpp>
#include <IsNdArray/synced_array.hpp>
#include <IsNdArray/nd_array.hpp>
#include <IsNdArray/nd_array_extra.hpp>

#include <iostream>
#include <string>
#include <cstdio>
#include <iomanip>
#include <cstring>

using namespace is::nbla;