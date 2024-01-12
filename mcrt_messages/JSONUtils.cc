// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#include "JSONUtils.h"

using namespace mcrt;

ValueVector::ValueVector (const Json::Value& value)
    : mValue(value)
{
}

