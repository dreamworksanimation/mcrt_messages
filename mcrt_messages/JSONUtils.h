// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#ifndef JSONUtils_h_INCLUDED
#define JSONUtils_h_INCLUDED

#include <json/value.h>
#include <string>
#include <vector>

typedef std::vector<std::string> StringVector;

namespace mcrt {

template <class T> struct Is {};
template <class T> struct To {};

#define JSON_TYPE(type, isTypeFunction, asTypeFunction)        \
    template <>                                                         \
    struct Is<type> {                                                   \
        Is (const Json::Value& value)                                   \
            : mValue(value) {}                                          \
                                                                        \
        bool operator ()()                                              \
        {                                                               \
            return mValue.isTypeFunction();                             \
        }                                                               \
                                                                        \
    private:                                                            \
    const Json::Value& mValue;                                          \
    };                                                                  \
                                                                        \
    template <>                                                         \
    struct To <type> {                                                  \
        To (const Json::Value& value)                                   \
            : mValue(value) {}                                          \
                                                                        \
        type operator ()()                                              \
        {                                                               \
            return mValue.asTypeFunction();                             \
        }                                                               \
                                                                        \
    private:                                                            \
    const Json::Value& mValue;                                          \
    }

JSON_TYPE(int, isInt, asInt);
JSON_TYPE(std::string, isString, asString);
JSON_TYPE(double, isDouble, asDouble);
JSON_TYPE(bool, isBool, asBool);
JSON_TYPE(const char*, isString, asCString);

// asFloat not available for jsoncpp-0.5.0
// JSON_TYPE(float, isDouble, asFloat);


class ValueVector
{
public:
    ValueVector (const Json::Value& value);

    template <class T>
    operator std::vector<T> ()
    {
        using ReturnType = std::vector<T>;
        ReturnType rv;
        if (!mValue.isArray()) {
            return rv;
        }

        Json::Value::ArrayIndex numItems = mValue.size();
        for (Json::Value::ArrayIndex idx = 0; idx < numItems; ++idx) {
            Json::Value value = mValue[idx];
            if (!Is<T>(value)()) {
                return ReturnType();
            }
            rv.push_back (To<T>(value)());
        }

        return rv;
    }

private:
    const Json::Value& mValue;
};

}
#endif /* JSONUtils_h_INCLUDED */

