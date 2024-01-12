// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0


#pragma once

#include <message_api/ContentMacros.h>

#include <string>

namespace mcrt {

class ViewportMessage : public arras4::api::ObjectContent
{
public:
    ARRAS_CONTENT_CLASS(ViewportMessage, "42e8b99d-cd06-4b4a-b80f-cc9592768aac",0);
    ViewportMessage() : mX(0), mY(0), mW(0), mH(0) {}
    ViewportMessage(int x, int y, int w, int h) : mX(x), mY(y), mW(w), mH(h) {}

    void serialize(arras4::api::DataOutStream& to) const;
    void deserialize(arras4::api::DataInStream& from, unsigned version);

    int x() const { return mX; }
    int y() const { return mY; }
    int width() const { return mW; }
    int height() const { return mH; }

protected:
    int mX;
    int mY;
    int mW;
    int mH;
};

} 

