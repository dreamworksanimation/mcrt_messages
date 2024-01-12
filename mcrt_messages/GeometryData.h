// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0


#pragma once

#include "GeometryObjects.h"

#include <message_api/ContentMacros.h>

#include <string>
#include <vector>

namespace mcrt {

class GeometryData : public arras4::api::ObjectContent
{
public:
    ARRAS_CONTENT_CLASS(GeometryData, "e742897a-da18-44dd-a94c-75890b2f7bbf",0);
 
    GeometryData() : mFrame(-1) {}
    ~GeometryData() {}

    void serialize(arras4::api::DataOutStream& to) const;
    void deserialize(arras4::api::DataInStream& from, unsigned version);

    std::vector<moonray::ObjectData> mObjectData;

    int mFrame;
};

}

