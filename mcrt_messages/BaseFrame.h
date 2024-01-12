// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0


#ifndef BASEFRAME_H
#define BASEFRAME_H

#include <message_api/ContentMacros.h>
#include <vector>
#include <memory>
#include <stdint.h>

namespace mcrt {

typedef std::shared_ptr<uint8_t> DataPtr;

/**
 * Creates a DataPtr directly from a uint8_t buffer, copying the source buffer
 * @param data The unsigned char buffer to copy
 * @param len The length of the buffer to copy
 * @return A shared pointer that owns a copy of the source buffer
 */
DataPtr makeCopyPtr(const uint8_t* data, size_t len);


/**
 * Creates a DataPtr directly from a uint8_t buffer, taking ownership of the buffer
 * @param data The unsigned char buffer to wrap
 * @return A shared pointer that takes ownership of the buffer
 */
DataPtr makeValPtr(uint8_t* data);

/**
 * Creates a DataPtr directly from a uint8_t buffer, *not* taking ownership of the buffer
 * @param data The unsigned char buffer to wrap
 * @return A shared pointer that takes *does not* take ownership of the buffer
 */
DataPtr makeRefPtr(uint8_t *data);

class BaseFrame : public arras4::api::ObjectContent
{
public:

    ARRAS_CONTENT_CLASS(BaseFrame, "a2c37bbf-51e9-45de-b9cd-ea5976908a03",0);
    BaseFrame();
    ~BaseFrame();

    void serialize(arras4::api::DataOutStream& to) const;
    void deserialize(arras4::api::DataInStream& from, unsigned version);

    // required for legacy message vhunking support
    size_t serializedLength() const;

    /**
     * Enumeration representing the different states the renderer is in that produced this frame
     */
    enum Status {
        STARTED = 0,  /// The first frame of a progressive render
        RENDERING,    /// Ongoing status of a progressively refining frame
        FINISHED,     /// The last frame of a progressive render
        CANCELLED,    /// Render was cancelled.  Image may be incomplete
        ERROR         /// Some error condition
    };

    /**
     * The image frame consists of a two dimensional grid of pixels whose origin is the
     * upper left corner of the image (0, 0) and spans to the bottom right corner
     * (mFrameWidth, mFrameHeight)
     * If an explicit Viewport or Region of Interest (ROI) is specified, it
     * is either equal to the size of the frame, or is strictly smaller
     * in one or more dimensions. Specifically:
     * mMinX >= 0, mMaxX <= FrameWidth, mMinY >= 0, mMaxY <= FrameHeight
     * All viewport information (Rezed/ROI) is defined by closed viewport definition which means
     * position (maxX, maxY) is a upper right corner pixel and it's inside viewport.
     *
     * (0, mFrameHeight) +------------------------------------+  (mFrameWidth, mFrameHeight)
     *                   |                                    |
     *                   | (mMinX, mMaxY)    (mMaxX, mMaxY)   |
     *                   |              +----+                |
     *                   |              |    |                |
     *                   |              +----+                |
     *                   |  (mMinX, mMinY)    (mMaxX, mMinY)  |
     *                   |                                    |
     *             (0,0) +------------------------------------+ (0, mFrameWidth)
     *
     * The width and height of the Viewport is always greater than zero.  This is to prevent divide by
     * zero errors. Specifically, width() > 0 and height() > 0
     *
     * Whether a Viewport is set or not can be inferred from the hasViewport() method. 
     * RezedViewport always should return hasViewport() = true.
     */
    struct Viewport
    {
        Viewport() : mMinX(0), mMinY(0), mMaxX(0), mMaxY(0), mHasViewport(false) {}
        ~Viewport() {}
        Viewport(int minX, int minY, int maxX, int maxY) : mMinX(0), mMinY(0), mMaxX(0), mMaxY(0), mHasViewport(false) {
            setViewport(minX, minY, maxX, maxY);
        }

        bool operator== (const Viewport& other) const;
        bool operator!= (const Viewport& other) const;

        void setViewport(int minX, int minY, int maxX, int maxY) {
            mMinX = minX; mMinY = minY; mMaxX = maxX; mMaxY = maxY; mHasViewport = true;
        }

        void reset() { mMinX = mMinY = mMaxX = mMaxY = 0; mHasViewport = false; }
        bool hasViewport() const { return mHasViewport; }

        int minX() const { return mMinX; }
        int minY() const { return mMinY; }
        int maxX() const { return mMaxX; }
        int maxY() const { return mMaxY; }
        int width() const { return mMaxX - mMinX + 1; }
        int height() const { return mMaxY - mMinY + 1; }

        int mMinX;
        int mMinY;
        int mMaxX;
        int mMaxY;
        bool mHasViewport;
    };

    // RenderedFrame specific image encodings
    enum ImageEncoding
    {
        ENCODING_UNKNOWN = 0,

        ENCODING_RGBA8,         /// RBGA (4 bytes)  1 byte per channel, includes alpha, 32 bits per pixel
        ENCODING_RGB888,        /// RGB  (3 bytes)  1 byte per channel, 24 bits per pixel
        ENCODING_LINEAR_FLOAT,  /// RGBA (16 bytes) 1 32-bit float per channel, 128 bits per pixel
        ENCODING_FLOAT,         /// X    (4 Bytes)  1 32-bit float channel, 32-bits per pixel
        ENCODING_FLOAT2,        /// XY   (8 bytes)  2 32-bit float channels, 64-bits per pixel
        ENCODING_FLOAT3,        /// XYZ  (12 bytes) 3 32-bit float channels, 96-bits per pixel

        ENCODING_MAX,
        ENCODING_SIZE=0xFFFFFFFF

    };

    struct DataBuffer {
        /**
         *  Initializes an empty buffer object
         */
        DataBuffer();

        /**
         * Creates a new buffer and copies aName
         */
        DataBuffer(DataPtr aData, size_t aLength, const char* aName, ImageEncoding aType);

        /**
         * Create a copy of the target buffer
         */
        DataBuffer(const DataBuffer& aCopy);

        /**
         * Support assignment/copying
         */
        DataBuffer& operator=(const DataBuffer& aCopy);

        ~DataBuffer();

        DataPtr mData;         /// Raw data buffer (wrapped)
        uint32_t mDataLength;  /// Size of data in bytes
        char* mName;           /// Human-readable or semantic name
        ImageEncoding mType;   /// Encoding type (if applicable) on data
    };

    /*
     * Metadata about this frame
     */
    struct Header
    {
        Header()
          : mNumBuffers(0)
          , mViewId(0)
          , mViewport()
          , mFrameId(0)
          , mStatus(ERROR)
          , mProgress(0.0f) {}

        Header(const Header& header);
        Header& operator = (const Header& Header);
        bool operator == (const Header& other) const;
        bool operator != (const Header& other) const;

        void setRezedViewport(const int minX, const int minY, const int maxX, const int maxY);

        // should be called after setRezedViewport()
        void setViewport(const int minX, const int minY, const int maxX, const int maxY);

        size_t mNumBuffers;          /// Number of buffers for this frame (should match mBuffers.size())
        size_t mViewId;              /// ID of the view, for supporting multiple views
        Viewport mRezedViewport;     /// Rezed viewport information.
        Viewport mViewport;          /// Viewport or Region-of-Interest, if applicable
        uint32_t mFrameId;           /// Unique ID per session for this frame type
        Status mStatus;              /// Current status for this Frame
        float mProgress;             /// Progress between 0.0 and 1.0 representing how complete the render is
    };

    Header mHeader;                   /// Header struct
    std::vector<DataBuffer> mBuffers; /// Array of buffers
 
    int getHeight() const { return mHeader.mRezedViewport.height(); }
    int getWidth() const { return mHeader.mRezedViewport.width(); }
    const Viewport &getRezedViewport() const { return mHeader.mRezedViewport; }
    bool hasViewport() const { return mHeader.mViewport.hasViewport(); }
    Viewport& getViewport() { return mHeader.mViewport; }
    const Viewport& getViewport() const { return mHeader.mViewport; }
    Status getStatus() const { return mHeader.mStatus; }
    float getProgress() const { return mHeader.mProgress; }

    /**
     * Adds a new, named DataBuffer to the list of buffers
     * @param aData Smart pointer wrapping the raw buffer
     * @param aLength Length of the buffer in bytes
     * @param aName Semantic name of the buffer
     * @param aType Image encoding applied to the buffer
     * @return the index of the buffer just added
     */
    size_t addBuffer(DataPtr aData, size_t aLength, const char* aName, ImageEncoding aType);

};

} 
#endif
