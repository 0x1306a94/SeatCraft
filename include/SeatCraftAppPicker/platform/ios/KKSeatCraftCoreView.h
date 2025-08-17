//
//  KKSeatCraftCoreView.h
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#import <UIKit/UIKit.h>

#import <SeatCraftAppPicker/KKSeatCraftCoreSeatStatusKey.h>
#import <SeatCraftAppPicker/defines.h>

NS_ASSUME_NONNULL_BEGIN

@class KKSeatCraftCoreSeatStatusSvgPathMap;
SEAT_CRAFT_API @interface KKSeatCraftCoreView : UIView
- (void)updateAreaSVGData:(NSData *)data;
- (void)updateAreaSVGPath:(NSString *)path;
- (void)updateSeatStatusSvgPathMap:(KKSeatCraftCoreSeatStatusSvgPathMap *)seatStatusSvgPathMap;
@end

NS_ASSUME_NONNULL_END
