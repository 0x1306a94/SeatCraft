//
//  KKSeatCraftCoreSeatStatusSvgPathMap.h.h
//  SeatCraftCore
//
//  Created by king on 2025/8/8.
//

#import <Foundation/Foundation.h>

#import "core/SeatStatusKey.h"

NS_ASSUME_NONNULL_BEGIN

@interface KKSeatCraftCoreSeatStatusSvgPathMap ()

- (std::unordered_map<kk::SeatStatusKey, std::string>)getMap;
@end

NS_ASSUME_NONNULL_END
