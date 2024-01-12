// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include "ProgressiveFrame.h"

#include <stdint.h>

namespace mcrt {

class ProgressiveFeedback : public arras4::api::ObjectContent
{
public:

    typedef ProgressiveFrame ParentClass;

    ARRAS_CONTENT_CLASS(ProgressiveFeedback, "ebe739cc-b3a9-423a-809f-022137bd545b", 0);

    ~ProgressiveFeedback() {}

    void serialize(arras4::api::DataOutStream& to) const override;
    void deserialize(arras4::api::DataInStream& from, unsigned version) override;

    // required for legacy message chunking support
    size_t serializedLength() const override;

    uint32_t mFeedbackId {0};
    std::string mMergeActionTrackerData;
    ProgressiveFrame::Ptr mProgressiveFrame;
};

} // namespace mcrt
