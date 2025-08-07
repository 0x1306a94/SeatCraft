//
//  KKSeatCraftCoreBackendView.h
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@protocol KKSeatCraftCoreBackendViewDelegate;
@interface KKSeatCraftCoreBackendView : UIView
@property (nonatomic, weak) id<KKSeatCraftCoreBackendViewDelegate> delegate;
@end

@protocol KKSeatCraftCoreBackendViewDelegate <NSObject>

- (void)seatCraftCoreBackendViewDidUpdateSize:(KKSeatCraftCoreBackendView *)backendView;

@end

NS_ASSUME_NONNULL_END
