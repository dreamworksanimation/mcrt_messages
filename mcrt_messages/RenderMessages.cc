// Copyright 2023-2024 DreamWorks Animation LLC
// SPDX-License-Identifier: Apache-2.0


#include "RenderMessages.h"

namespace mcrt {

const char* RenderMessages::RENDER_CONTROL_ID = "AE5F4403-5A4F-408D-8057-A4E42039A5E8";
const char* RenderMessages::RENDER_CONTROL_NAME = "Render Control";
const char* RenderMessages::RENDER_CONTROL_PAYLOAD_OPERATION = "operation";
const char* RenderMessages::RENDER_CONTROL_PAYLOAD_OPERATION_START = "start";
const char* RenderMessages::RENDER_CONTROL_PAYLOAD_OPERATION_STOP = "stop";

const char* RenderMessages::PICK_MESSAGE_ID = "21B91B93-85AA-446B-A17E-AF50A1DC221A";
const char* RenderMessages::PICK_MESSAGE_NAME = "Pick";
const char* RenderMessages::PICK_MESSAGE_PAYLOAD_PIXEL = "pixel";
const char* RenderMessages::PICK_MESSAGE_PAYLOAD_MODE = "mode";

const char* RenderMessages::PICK_DATA_MESSAGE_ID = "E40B902A-8E7E-4C85-9256-F5539A36FB16";
const char* RenderMessages::PICK_DATA_MESSAGE_NAME = "Pick Data";
const char* RenderMessages::PICK_DATA_MESSAGE_PAYLOAD_LIGHTS = "lights";
const char* RenderMessages::PICK_DATA_MESSAGE_PAYLOAD_LIGHT_CONTRIBUTIONS = "contributions";
const char* RenderMessages::PICK_DATA_MESSAGE_PAYLOAD_GEOMETRY = "geometry";
const char* RenderMessages::PICK_DATA_MESSAGE_PAYLOAD_GEOMETRY_PARTS = "geometry parts";
const char* RenderMessages::PICK_DATA_MESSAGE_PAYLOAD_MATERIALS = "materials";

const char* RenderMessages::LOGGING_CONFIGURATION_MESSAGE_ID = "3D7DE315-C30A-4829-9FA6-2DFB74DCDBF0";
const char* RenderMessages::LOGGING_CONFIGURATION_MESSAGE_NAME = "Logging Configuration";

const char* RenderMessages::LOGGING_MESSAGE_ID = "663c5375-e12a-4314-a0ed-22eec0763dda";
const char* RenderMessages::LOGGING_MESSAGE_NAME = "Log Message";
const char* RenderMessages::LOGGING_MESSAGE_PAYLOAD_STRING = "string";
const char* RenderMessages::LOGGING_MESSAGE_PAYLOAD_LEVEL = "log level";

const char* RenderMessages::SET_ROI_OPERATION_NAME = "ROI";
const char* RenderMessages::SET_ROI_OPERATION_ID = "BFA7F4B1-B3EC-4166-BFA6-C38B543620B6";

const char* RenderMessages::SET_ROI_STATUS_OPERATION_NAME = "ROI Status";
const char* RenderMessages::SET_ROI_STATUS_OPERATION_ID = "F9B499DC-9637-461C-BA53-B14ABB9EF6F4";

const char* RenderMessages::INVALIDATE_RESOURCES_ID = "728FA0D1-33FD-4CFB-ACCF-24D9E7907AFB";
const char* RenderMessages::INVALIDATE_RESOURCES_MESSAGE_NAME = "Invalidate Resources";
const char* RenderMessages::INVALIDATE_RESOURCES_PAYLOAD_LIST = "resources";

const char* RenderMessages::RENDER_SETUP_ID = "9f186a7f-93d5-4f7d-8b5c-09f0e062c304";
const char* RenderMessages::RENDER_SETUP_NAME = "Render Setup";

const char*RenderMessages:: RENDER_FILE_MESSAGE_ID = "01f62844-966c-417e-b3a9-646a80dca832";
const char* RenderMessages::RENDER_FILE_MESSAGE_NAME = "Render File";
const char* RenderMessages::RENDER_FILE_PAYLOAD_FORCE_RELOAD = "forceReload";
const char* RenderMessages::RENDER_FILE_PAYLOAD_FILE = "file";
const char* RenderMessages::RENDER_FILE_PAYLOAD_FILES = "files";

/*static*/ JSONMessage::Ptr 
RenderMessages::createPickDataMessage(int x, int y, 
                                      const std::string& clientData)
{
    JSONMessage::Ptr message = JSONMessage::create(PICK_DATA_MESSAGE_ID, PICK_DATA_MESSAGE_NAME);

    Json::Value& msg = message->mRoot[JSONMessage::getMessageRoot()];
    Json::Value coords;
    coords.append(x);
    coords.append(y);
    msg[JSONMessage::getMessagePayload()][PICK_MESSAGE_PAYLOAD_PIXEL] = coords;
    message->mClientData = clientData;
    return message;
}

/*static*/ JSONMessage::Ptr 
RenderMessages::createLogMessage(const int level, 
                                 const std::string& message)
{ 
    JSONMessage::Ptr json = JSONMessage::create(LOGGING_MESSAGE_ID, LOGGING_MESSAGE_NAME);
    Json::Value& msg = json->mRoot[JSONMessage::getMessageRoot()];
    Json::Value val;
    val[LOGGING_MESSAGE_PAYLOAD_STRING] = message;
    val[LOGGING_MESSAGE_PAYLOAD_LEVEL] = level;
    msg[JSONMessage::getMessagePayload()] = val;
    return json;
}

/*static*/ JSONMessage::Ptr 
RenderMessages::createControlMessage(bool stop)
{ 
    JSONMessage::Ptr json = JSONMessage::create(RENDER_CONTROL_ID, RENDER_CONTROL_NAME);
    Json::Value& msg = json->mRoot[JSONMessage::getMessageRoot()];
    Json::Value val;
    val[RENDER_CONTROL_PAYLOAD_OPERATION] = stop ? 
        RENDER_CONTROL_PAYLOAD_OPERATION_STOP : 
        RENDER_CONTROL_PAYLOAD_OPERATION_START ;
    msg[JSONMessage::getMessagePayload()] = val;
    return json;
}

/*static*/ JSONMessage::Ptr 
RenderMessages::createRenderSetupMessage()
{ 
    JSONMessage::Ptr json = JSONMessage::create(RENDER_SETUP_ID, RENDER_SETUP_NAME);
    // the payload of a render setup message is obsolete. Now it simply clears
    // the current render
    return json;
}

/* static */ JSONMessage::Ptr
RenderMessages::createRoiStatusMessage(bool status)
{
    JSONMessage::Ptr message = JSONMessage::create(SET_ROI_STATUS_OPERATION_ID, SET_ROI_STATUS_OPERATION_NAME);

    Json::Value& msg = message->mRoot[JSONMessage::getMessageRoot()];
    Json::Value val;
    val.append(status);

    msg[JSONMessage::getMessagePayload()] = val;
    return message;
}

/* static */ JSONMessage::Ptr
RenderMessages::createRoiMessage(int minx, int miny, int maxx, int maxy)
{
    JSONMessage::Ptr message = JSONMessage::create(SET_ROI_OPERATION_ID, SET_ROI_OPERATION_NAME);

    Json::Value& msg = message->mRoot[JSONMessage::getMessageRoot()];
    Json::Value val;
    val.append(minx);
    val.append(miny);
    val.append(maxx);
    val.append(maxy);

    msg[JSONMessage::getMessagePayload()] = val;
    return message;
}

/* static */ JSONMessage::Ptr
RenderMessages::createInvalidateResourcesMessage(const std::vector<std::string>& resources)
{
    JSONMessage::Ptr json = JSONMessage::create (INVALIDATE_RESOURCES_ID, INVALIDATE_RESOURCES_MESSAGE_NAME);

    Json::Value& msg = json->mRoot[JSONMessage::getMessageRoot()];
    Json::Value val;
    for(const std::string& resource: resources) {
        val.append(resource);
    }
    msg[JSONMessage::getMessagePayload()][INVALIDATE_RESOURCES_PAYLOAD_LIST] = val;
    return json;
}

/* static */ JSONMessage::Ptr
RenderMessages::createRenderFileMessage(bool forceReload,
                                        const std::string& file)
{
    JSONMessage::Ptr json = JSONMessage::create (RENDER_FILE_MESSAGE_ID, RENDER_FILE_MESSAGE_NAME);
    Json::Value& msg = json->mRoot[JSONMessage::getMessageRoot()];
    Json::Value val;
    val[RENDER_FILE_PAYLOAD_FORCE_RELOAD] = forceReload;
    val[RENDER_FILE_PAYLOAD_FILE] = file;
    msg[JSONMessage::getMessagePayload()] = val;
    return json;
}

/* static */ JSONMessage::Ptr
RenderMessages::createRenderFileMessage(bool forceReload,
                                        const std::vector<std::string>& files)
{
    JSONMessage::Ptr json = JSONMessage::create (RENDER_FILE_MESSAGE_ID, RENDER_FILE_MESSAGE_NAME);
    Json::Value& msg = json->mRoot[JSONMessage::getMessageRoot()];
    Json::Value val;
    val[RENDER_FILE_PAYLOAD_FORCE_RELOAD] = forceReload;
    Json::Value filesVal;
    for(const std::string& file: files) {
        filesVal.append(file);
    }
    val[RENDER_FILE_PAYLOAD_FILES] = filesVal;
    msg[JSONMessage::getMessagePayload()] = val;
    return json;
}

}
