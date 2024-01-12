// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#include "ProgressiveFeedback.h"

namespace mcrt {

ARRAS_CONTENT_IMPL(ProgressiveFeedback);

void
ProgressiveFeedback::serialize(arras4::api::DataOutStream& out) const
{ 
    out.write(mFeedbackId);

    if (mMergeActionTrackerData.empty()) {
        out.write(static_cast<uint32_t>(0));
    } else {
        out.write(static_cast<uint32_t>(mMergeActionTrackerData.size()));
        out.write(mMergeActionTrackerData.c_str(), mMergeActionTrackerData.size());
    }

    bool flag = mProgressiveFrame.get() ? true : false;
    out.write(flag);
    if (mProgressiveFrame) {
        mProgressiveFrame->serialize(out);
    }
}

void
ProgressiveFeedback::deserialize(arras4::api::DataInStream& in, unsigned version)
{
    in.read(&mFeedbackId, sizeof(mFeedbackId));

    uint32_t size;
    in.read(&size, sizeof(uint32_t));
    if (!size) {
        mMergeActionTrackerData.clear();
    } else {
        mMergeActionTrackerData.resize(size);
        in.read(&mMergeActionTrackerData[0], size);
    }

    bool flag;
    in.read(&flag, sizeof(flag));
    if (flag) {
        mProgressiveFrame.reset(new mcrt::ProgressiveFrame);
        mProgressiveFrame->deserialize(in, version);
    } else {
        mProgressiveFrame.reset();
    }
}

size_t
ProgressiveFeedback::serializedLength() const
{
    uint32_t mergeActionTrackerDataSize = sizeof(uint32_t) + mMergeActionTrackerData.size();
    uint32_t progressiveFrameSize = sizeof(bool) + (mProgressiveFrame ? mProgressiveFrame->serializedLength() : 0);

    return sizeof(mFeedbackId) + mergeActionTrackerDataSize + progressiveFrameSize;
}

} // namespace mcrt
