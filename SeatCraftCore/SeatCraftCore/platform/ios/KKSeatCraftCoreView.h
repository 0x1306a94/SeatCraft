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
@property (nonatomic, strong, readonly) KKSeatCraftCoreSeatStatusSvgPathMap *seatStatusSvgPathMap;
- (void)updateContentSize:(CGSize)contentSize;
- (void)updateAreaSVG:(NSString *)path;
@end

NS_ASSUME_NONNULL_END
