// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#include "ProgressiveFrame.h"
#include <arras4_log/Logger.h>

namespace mcrt {

ARRAS_CONTENT_IMPL(ProgressiveFrame);

ProgressiveFrame::~ProgressiveFrame()
{

}

void
ProgressiveFrame::serialize(arras4::api::DataOutStream& out) const
{ 
    auto outString = [&](const std::string& str) {
        if (str.empty()) {
            out.write(static_cast<uint32_t>(0));
        } else {
            out.write(static_cast<uint32_t>(str.size()));
            out.write(str.c_str(), str.size()); 
        }
    };
   
    ParentClass::serialize(out);
    out.write(mMachineId);
    out.write(mSnapshotId);
    out.write(mSendImageActionId);
    out.write(mSnapshotStartTime); 
    out.write(mCoarsePassStatus); 
    outString(mDenoiserAlbedoInputName);
    outString(mDenoiserNormalInputName);
}

void
ProgressiveFrame::deserialize(arras4::api::DataInStream& in, unsigned version)
{
    auto inString = [&](std::string& str) {
        uint32_t size;
        in.read(&size, sizeof(uint32_t));
        if (!size) {
            str.clear();
        } else {
            str.resize(size);
            in.read(&str[0], size);
        }
    };

    ParentClass::deserialize(in,version);   
    
    in.read(&mMachineId, sizeof(mMachineId)); 
    in.read(&mSnapshotId, sizeof(mSnapshotId));
    in.read(&mSendImageActionId, sizeof(mSendImageActionId));
    in.read(&mSnapshotStartTime, sizeof(mSnapshotStartTime));
    in.read(&mCoarsePassStatus, sizeof(mCoarsePassStatus));
    inString(mDenoiserAlbedoInputName);
    inString(mDenoiserNormalInputName);
}

size_t
ProgressiveFrame::serializedLength() const
{
    auto sizeString = [](const std::string& str) {
        if (str.empty()) {
            return sizeof(uint32_t);
        } else {
            return sizeof(uint32_t) + str.size();
        }
    };

    return (ParentClass::serializedLength() +
            sizeof(mMachineId) +
            sizeof(mSnapshotId) +
            sizeof(mSendImageActionId) +
            sizeof(mSnapshotStartTime) +
            sizeof(mCoarsePassStatus) +
            sizeString(mDenoiserAlbedoInputName) +
            sizeString(mDenoiserNormalInputName));
}

}
