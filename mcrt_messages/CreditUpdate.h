// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0

#ifndef MCRT_MESSAGES_CREDIT_UPDATE_H
#define MCRT_MESSAGES_CREDIT_UPDATE_H

#include <message_api/ContentMacros.h>
#include <string>

namespace mcrt {

    enum class CreditUpdateMode { Update, Set, Reset };

    class CreditUpdate : public arras4::api::ObjectContent
    {
    public:
        ARRAS_CONTENT_CLASS(CreditUpdate, "f0fcbb5e-48d3-44ca-a4b7-e5390cfd3f1c",0);
        ~CreditUpdate();

        // Message implementation
        void serialize(arras4::api::DataOutStream& to) const;
        void deserialize(arras4::api::DataInStream& from, unsigned version);

        const std::string& name() const { return mName; }
        std::string& name() { return mName; }
        CreditUpdateMode mode() const { return mMode; }
        CreditUpdateMode& mode() { return mMode; }
        int value() const { return mValue; }
        int& value() { return mValue; }

        void applyTo(int& counter, int initial) const;

    private:
        std::string mName;
        CreditUpdateMode mMode = CreditUpdateMode::Update;
        int mValue = 0;
    };
}

#endif 

