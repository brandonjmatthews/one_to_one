//
//  main.m
//  one_to_one
//
//  Created by Brandon Matthews on 4/3/17.
//  Copyright © 2017 Brandon Matthews. All rights reserved.
//  Based on "lifesized.js" by Francois Laberge
//  https://github.com/francoislaberge/lifesized
//

#import <Foundation/Foundation.h>
@import AppKit;

int main(int argc, const char * argv[]) {
    @autoreleasepool {
        // Check for display ID parameter
        if (argc != 2) {
            printf("one_to_one requires a screenID as the first argument");
            return -1;
        }
        
        long targetScreenID = atoi(argv[1]);
        long count = [[NSScreen screens] count];
        
        for (int i = 0; i < count; i++) {
            NSScreen *screen = [[NSScreen screens] objectAtIndex:i];
            NSDictionary *description = [screen deviceDescription];
            NSNumber* screenID = [description objectForKey:@"NSScreenNumber"];
            
            if([screenID longValue] == targetScreenID){
                
                NSSize displayPixelSize = [[description objectForKey:NSDeviceSize] sizeValue];
                
                CGSize displayPhysicalSize = CGDisplayScreenSize([screenID unsignedIntValue]);
                
                printf("%0.2f", displayPixelSize.width);
                printf(",");
                printf("%0.2f", displayPixelSize.height);
                printf(",");
                printf("%0.2f", displayPhysicalSize.width);
                printf(",");
                printf("%0.2f", displayPhysicalSize.height);
                printf(",");
                printf("%0.2f",(displayPixelSize.width / displayPhysicalSize.width));
                printf(",");
                printf("%0.2f",(displayPixelSize.height / displayPhysicalSize.height));

                // Exit successfully
                return 0;
            }

        }
    }
    return 0;
}
