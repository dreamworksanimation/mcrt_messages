// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#ifndef RENDEREDFRAME_H
#define RENDEREDFRAME_H

#include "BaseFrame.h"

namespace mcrt {

class RenderedFrame : public BaseFrame
{
public:

    typedef BaseFrame ParentClass;

    ARRAS_CONTENT_CLASS(RenderedFrame, "885db5ea-02bf-4075-8409-e52b6015695c",0);
    RenderedFrame();
    ~RenderedFrame();

    // required for legacy MessageChunking support
    size_t serializedLength() const;

    // Message implementation
    void serialize(arras4::api::DataOutStream& to) const;
    void deserialize(arras4::api::DataInStream& from, unsigned version);

    /**
     * Enumeration representing which eye to render was rendered for stereoscopic rendering
     */
    enum RenderedEye
    {
        CENTER_EYE = 0,  /// Center eye (Default for non-stereo images)
        RIGHT_EYE = 1,   /// Right eye in a stereo image
        LEFT_EYE = 2     /// Left eye in a stereo image
    };

    RenderedEye mEye;

    uint64_t mSnapshotStartTime; // time of snapshot start
};

}

#endif // RENDEREDFRAME_H

