//
//  KKSeatCraftCoreBackendView.m
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#import "KKSeatCraftCoreBackendView.h"

@implementation KKSeatCraftCoreBackendView
#if DEBUG
- (void)dealloc {
    NSLog(@"[%@ dealloc]", NSStringFromClass(self.class));
}
#endif

+ (Class)layerClass {
    return [CAEAGLLayer class];
}

- (void)layoutSubviews {
    [super layoutSubviews];
    [self updateSize];
}

- (void)setBounds:(CGRect)bounds {
    CGRect oldBounds = self.bounds;
    [super setBounds:bounds];
    if (oldBounds.size.width != bounds.size.width || oldBounds.size.height != bounds.size.height) {
        [self updateSize];
    }
}

- (void)setFrame:(CGRect)frame {
    CGRect oldRect = self.frame;
    [super setFrame:frame];
    if (oldRect.size.width != frame.size.width || oldRect.size.height != frame.size.height) {
        [self updateSize];
    }
}

- (void)setContentScaleFactor:(CGFloat)scaleFactor {
    CGFloat oldScaleFactor = self.contentScaleFactor;
    [super setContentScaleFactor:scaleFactor];
    if (oldScaleFactor != scaleFactor) {
        [self updateSize];
    }
}

- (void)updateSize {
    [self.delegate seatCraftCoreBackendViewDidUpdateSize:self];
}

@end
