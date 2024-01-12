// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0


#pragma once

#include "BaseFrame.h"

#include <stdint.h>

namespace mcrt {

class PartialFrame : public BaseFrame
{
public:

    typedef BaseFrame ParentClass;

    ARRAS_CONTENT_CLASS(PartialFrame, "fad570d9-3dd2-4770-a5eb-4f4157480b11",0);
    PartialFrame() {}
    ~PartialFrame();

    void serialize(arras4::api::DataOutStream& to) const;
    void deserialize(arras4::api::DataInStream& from, unsigned version);

    int mMachineId;
    uint64_t mSnapshotStartTime; // time of snapshot start
};

}

