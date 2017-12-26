#include "nan.h"

#ifdef _WIN32
#include "one_to_one_win.cc"
#elif __APPLE__
#include "one_to_one_mac.mm"
#else
#error "Unknown compiler"
#endif



NAN_METHOD(DetectScreenSize)
{
    v8::String::Utf8Value v8DisplayID(info[0]->ToString());
    std::string displayID = std::string(*v8DisplayID);

    v8::Local<v8::Object> results = getScreenSize(std::string(displayID));

    return info.GetReturnValue().Set(results);
}

void Initialize(v8::Local<v8::Object> exports)
{
    exports->Set(Nan::New("detectScreenSize").ToLocalChecked(), Nan::New<v8::FunctionTemplate>(DetectScreenSize)->GetFunction());
}

NODE_MODULE(OneToOne, Initialize);