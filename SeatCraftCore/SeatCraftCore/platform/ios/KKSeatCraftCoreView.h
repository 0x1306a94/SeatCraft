//
//  KKSeatCraftCoreView.h
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@class KKSeatCraftCoreSeatStatusSvgPathMap;

@interface KKSeatCraftCoreView : UIView
- (void)updateAreaSVG:(NSString *)path;
- (void)updateSeatStatusSvgPathMap:(KKSeatCraftCoreSeatStatusSvgPathMap *)seatStatusSvgPathMap;
@end

NS_ASSUME_NONNULL_END
