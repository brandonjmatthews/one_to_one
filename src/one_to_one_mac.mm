//
//  main.m
//  one_to_one
//
//  Created by Brandon Matthews on 4/3/17.
//  Copyright © 2017 Brandon Matthews. All rights reserved.
//  Based on "lifesized.js" by Francois Laberge
//  https://github.com/francoislaberge/lifesized
//
#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

#include "nan.h"
#include "string.h"




v8::Local<v8::Object> getScreenSize(std::string displayID) {
    @autoreleasepool {
        
        long targetScreenID = atoi(displayID.c_str());
        long count = [[NSScreen screens] count];
        
        for (int i = 0; i < count; i++) {
            NSScreen *screen = [[NSScreen screens] objectAtIndex:i];
            NSDictionary *description = [screen deviceDescription];
            NSNumber* screenID = [description objectForKey:@"NSScreenNumber"];
            
            if([screenID longValue] == targetScreenID){
                
                NSSize displayPixelSize = [[description objectForKey:NSDeviceSize] sizeValue];
                
                CGSize displayPhysicalSize = CGDisplayScreenSize([screenID unsignedIntValue]);
               
                v8::Local<v8::Object> pixelSize = Nan::New<v8::Object>();
                pixelSize->Set(Nan::New("width").ToLocalChecked(), Nan::New((int) displayPixelSize.width));
                pixelSize->Set(Nan::New("height").ToLocalChecked(), Nan::New((int) displayPixelSize.height));

                v8::Local<v8::Object> physicalSize = Nan::New<v8::Object>();
                physicalSize->Set(Nan::New("width").ToLocalChecked(), Nan::New((int) displayPhysicalSize.width));
                physicalSize->Set(Nan::New("height").ToLocalChecked(), Nan::New((int) displayPhysicalSize.height));

                v8::Local<v8::Object> physicalPPCM = Nan::New<v8::Object>();
                physicalPPCM->Set(Nan::New("horizontal").ToLocalChecked(), Nan::New((int) (displayPixelSize.width / displayPhysicalSize.width) * 10));
                physicalPPCM->Set(Nan::New("vertical").ToLocalChecked(), Nan::New((int) (displayPixelSize.height / displayPhysicalSize.height) * 10));

                v8::Local<v8::Object> res = Nan::New<v8::Object>();
                res->Set(Nan::New("pixel").ToLocalChecked(), pixelSize);
                res->Set(Nan::New("physical").ToLocalChecked(), physicalSize);
                res->Set(Nan::New("ppcm").ToLocalChecked(), physicalPPCM);
                res->Set(Nan::New("success").ToLocalChecked(), Nan::New(true));

                return res;
            }
        }

        v8::Local<v8::Object> err = Nan::New<v8::Object>();
        err->Set(Nan::New("message").ToLocalChecked(), Nan::New("Screen not found").ToLocalChecked());
        return err;
    }
}