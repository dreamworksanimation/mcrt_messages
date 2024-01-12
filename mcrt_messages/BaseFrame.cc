// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0


#include "BaseFrame.h"

#include <algorithm>
#include <cassert>
#include <cstring>
#include <stdint.h>

#if defined(__INTEL_COMPILER)
    #define ARRAS_NO_FP_EQUALITY_WARNING_BEGIN \
        _Pragma("warning (push)")    \
        _Pragma("warning (disable:1572)")
    #define ARRAS_NO_FP_EQUALITY_WARNING_END \
        _Pragma("warning (pop)")
#else
    // For GCC, #pragma GCC diagnostic ignored "-Wfloat-equal"
    // isn't working until gcc 4.2+,
    // Trying
    // #pragma GCC system_header
    // creates other problems, most notably "warning: will never be executed"
    // in from templates, unsure of how to work around.
    // If necessary, could use integer based comparisons for equality
    #define ARRAS_NO_FP_EQUALITY_WARNING_BEGIN
    #define ARRAS_NO_FP_EQUALITY_WARNING_END
#endif

namespace mcrt {

ARRAS_CONTENT_IMPL(BaseFrame);

DataPtr makeCopyPtr(const uint8_t* data, size_t len) {
    uint8_t* copy = new uint8_t[len];
    std::memcpy(copy, data, len);
    return makeValPtr(copy);
}
DataPtr makeValPtr(uint8_t* data) { return DataPtr(data, std::default_delete<uint8_t[]>()); }
DataPtr makeRefPtr(uint8_t *data) { return DataPtr(data, [](uint8_t* /* data */){}); }

BaseFrame::DataBuffer::DataBuffer()
    : mData()
    , mDataLength(0)
    , mName(nullptr)
    , mType(ENCODING_UNKNOWN)
{
}

BaseFrame::DataBuffer::DataBuffer(DataPtr aData, size_t aLength, const char* aName, ImageEncoding aType)
    : mData(aData)
    , mDataLength(static_cast<uint32_t>(aLength))
    , mName(nullptr)
    , mType(aType)
{

	// we are only going to take the first 255 plus EOL characters
	// for the buffer names. If the buffer name is longer we truncate.
    uint32_t nameLen = static_cast<uint32_t>(strlen(aName) + 1);
    uint32_t maxBuffer = std::numeric_limits<uint8_t>::max();
    uint32_t usableLength =  std::min(nameLen, maxBuffer);
    this->mName = new char[usableLength];
    std::strncpy(this->mName, aName, usableLength);
    this->mName[usableLength-1] = 0;
}


BaseFrame::DataBuffer&
BaseFrame::DataBuffer::operator=(const DataBuffer& aCopy)
{
    if (this == &aCopy) {
        return *this;
    }

    delete [] this->mName;

    this->mData = aCopy.mData;
    this->mDataLength = aCopy.mDataLength;
    this->mName = new char[strlen(aCopy.mName) + 1];
    std::strcpy(this->mName, aCopy.mName);

    this->mType = aCopy.mType;

    return *this;
}

BaseFrame::DataBuffer::~DataBuffer()
{
    delete [] mName;
}

BaseFrame::DataBuffer::DataBuffer(const DataBuffer& aCopy)
    : mData(aCopy.mData)
    , mDataLength(aCopy.mDataLength)
    , mName(nullptr)
    , mType(aCopy.mType)
{
    this->mName = new char[strlen(aCopy.mName) + 1];
    std::strcpy(this->mName, aCopy.mName);
}


bool
BaseFrame::Viewport::operator== (const BaseFrame::Viewport& other) const
{
    return this->mMinX == other.mMinX &&
           this->mMinY == other.mMinY &&
           this->mMaxX == other.mMaxX &&
           this->mMaxY == other.mMaxY &&
           this->mHasViewport == other.mHasViewport;
}

bool
BaseFrame::Viewport::operator!= (const BaseFrame::Viewport& other) const
{
    return !(*this == other);
}


BaseFrame::Header::Header(const Header& header)
{
    *this = header;
}

BaseFrame::Header&
BaseFrame::Header::operator= (const BaseFrame::Header& other)
{
    this->mNumBuffers = 0; 
    this->mViewId = other.mViewId;
    this->mRezedViewport = other.mRezedViewport;
    this->mViewport = other.mViewport;
    this->mFrameId = other.mFrameId;
    this->mStatus = other.mStatus;
    this->mProgress = other.mProgress;
    return *this;
}

bool
BaseFrame::Header::operator == (const BaseFrame::Header& other) const
{  
    ARRAS_NO_FP_EQUALITY_WARNING_BEGIN
    return (this->mNumBuffers == other.mNumBuffers &&
            this->mViewId == other.mViewId &&
            this->mRezedViewport == other.mRezedViewport &&
            this->mViewport == other.mViewport &&
            this->mFrameId == other.mFrameId &&
            this->mStatus == other.mStatus &&
            this->mProgress == other.mProgress);
    ARRAS_NO_FP_EQUALITY_WARNING_END
}

bool
BaseFrame::Header::operator != (const BaseFrame::Header& other) const
{
    return !(*this == other);
}

void
BaseFrame::Header::setRezedViewport(const int minX, const int minY, const int maxX, const int maxY)
{
    int currMinX = std::min(minX, maxX);
    int currMinY = std::min(minY, maxY);
    int currMaxX = std::max(minX, maxX);
    int currMaxY = std::max(minY, maxY);
    mRezedViewport.setViewport(currMinX, currMinY, currMaxX, currMaxY);
}
void
BaseFrame::Header::setViewport(const int minX, const int minY, const int maxX, const int maxY)
{
    int currMinX = std::max(std::min(minX, maxX), 0);
    int currMinY = std::max(std::min(minY, maxY), 0);
    int currMaxX = std::min(std::max(minX, maxX), mRezedViewport.mMaxX - mRezedViewport.mMinX);
    int currMaxY = std::min(std::max(minY, maxY), mRezedViewport.mMaxY - mRezedViewport.mMinY);
    mViewport.setViewport(currMinX, currMinY, currMaxX, currMaxY);
}

BaseFrame::BaseFrame()
{
}

BaseFrame::~BaseFrame()
{
}

size_t
BaseFrame::serializedLength() const
{
    size_t s = sizeof(Header);
    for (auto const& buffer: mBuffers) {
        // mDataLength + mData
        s += sizeof(buffer.mDataLength) + buffer.mDataLength;
        // len(mName) + mName
        s += sizeof(uint8_t) + strlen(buffer.mName);
        // mType
        s += sizeof(buffer.mType);
    }
    return s;
}    

void
BaseFrame::serialize(arras4::api::DataOutStream& out) const
{
    assert(mHeader.mNumBuffers == mBuffers.size());

    out.write(&mHeader, sizeof(Header));

    for (auto const& buffer: mBuffers) {
    	uint8_t len = static_cast<uint8_t>(strlen(buffer.mName));
        out.write(&buffer.mDataLength, sizeof(buffer.mDataLength));
        out.write(buffer.mData.get(), buffer.mDataLength);
        out.write(&len, sizeof(len));
        out.write(buffer.mName, strlen(buffer.mName));
        out.write(&buffer.mType, sizeof(buffer.mType));
    }
}

void
BaseFrame::deserialize(arras4::api::DataInStream& in, unsigned)
{
    in.read(&mHeader, sizeof(Header));

    mBuffers.clear();

    uint8_t len;
    DataPtr data;
    uint8_t* rawData = nullptr;
    uint32_t dataLength = 0;
    char* name = nullptr;
    ImageEncoding type;

    size_t numBuffers = mHeader.mNumBuffers;
    for (size_t i = 0; i < numBuffers; ++i) {
        in.read(&dataLength, sizeof(dataLength));
        rawData = new uint8_t[dataLength];
        in.read(static_cast<void*>(rawData), dataLength);
        in.read(&len, sizeof(len));
        name = new char[len + 1]; // Additional character for '\0'
        name[len] = 0;
        in.read(static_cast<void*>(name), len);
        in.read(&type, sizeof(type));

        data = makeValPtr(rawData); // wrap our pointer
        addBuffer(data, dataLength, name, type);

        // cleanup
        delete [] name;
    }
}

size_t
BaseFrame::addBuffer(DataPtr aData, size_t aLength, const char* aName, ImageEncoding aType)
{
    mBuffers.emplace_back(aData, aLength, aName, aType);
    mHeader.mNumBuffers = mBuffers.size();
    return (mBuffers.size() - 1);
}

}

