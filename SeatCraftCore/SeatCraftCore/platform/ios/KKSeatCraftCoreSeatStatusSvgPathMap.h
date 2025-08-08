//
//  KKSeatCraftCoreSeatStatusSvgPathMap.h.h
//  SeatCraftCore
//
//  Created by king on 2025/8/8.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef uint8_t KKSeatCraftCoreSeatStatusKey;
@interface KKSeatCraftCoreSeatStatusSvgPathMap : NSObject
- (void)add:(KKSeatCraftCoreSeatStatusKey)key path:(NSString *)path;
- (void)remove:(KKSeatCraftCoreSeatStatusKey)key;
@end

NS_ASSUME_NONNULL_END
