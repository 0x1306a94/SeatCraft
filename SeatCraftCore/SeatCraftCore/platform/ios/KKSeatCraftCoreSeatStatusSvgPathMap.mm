//
//  KKSeatCraftCoreSeatStatusSvgPathMap.m
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#import "KKSeatCraftCoreSeatStatusSvgPathMap.h"

#import "KKSeatCraftCoreSeatStatusSvgPathMap+Private.h"

@interface KKSeatCraftCoreSeatStatusSvgPathMap ()

@end

@implementation KKSeatCraftCoreSeatStatusSvgPathMap {
    kk::SeatStatusSVGPathMap _pathMap;
}

- (void)dealloc {

#if DEBUG
    NSLog(@"[%@ dealloc]", NSStringFromClass(self.class));
#endif
}

- (void)add:(KKSeatCraftCoreSeatStatusKey)key path:(NSString *)path {
    if (path == nil || path.length == 0) {
        _pathMap.erase(key);
        return;
    }
    _pathMap[key] = std::string(path.UTF8String);
}

- (void)remove:(KKSeatCraftCoreSeatStatusKey)key {
    _pathMap.erase(key);
}

- (kk::SeatStatusSVGPathMap)getMap {
    return _pathMap;
}
@end
