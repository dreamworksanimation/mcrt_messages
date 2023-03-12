// Copyright 2023 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0


#pragma once

#include "BaseFrame.h"

#include <stdint.h>

namespace mcrt {

class ProgressiveFrame : public BaseFrame
{
public:

    typedef BaseFrame ParentClass;

    ARRAS_CONTENT_CLASS(ProgressiveFrame, "115d0e01-1ae5-4e0d-aa1c-ab26a421c16e",0);
    ProgressiveFrame() {}
    ~ProgressiveFrame();

    void serialize(arras4::api::DataOutStream& to) const;
    void deserialize(arras4::api::DataInStream& from, unsigned version);

    // required for legacy message chunking support
    size_t serializedLength() const;

    int mMachineId;
    uint32_t mSnapshotId; // unique increment snapshot id per one rendering
    uint64_t mSnapshotStartTime; // time of snapshot start
    int mCoarsePassStatus;       // 0:coarsePass 1:CoarsePassDone 2:unknown
    std::string mDenoiserAlbedoInputName;
    std::string mDenoiserNormalInputName;
};

}

