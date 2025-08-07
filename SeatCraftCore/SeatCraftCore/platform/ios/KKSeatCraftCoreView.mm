//
//  KKSeatCraftCoreView.m
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#import "KKSeatCraftCoreView.h"

#import "KKSeatCraftCoreBackendView.h"

#import "../../core/SeatCraftCoreApp.hpp"
#import "./renderer/IOSSeatCraftCoreRenderer.h"

@interface KKSeatCraftCoreView () <KKSeatCraftCoreBackendViewDelegate, UIGestureRecognizerDelegate>
@property (nonatomic, strong) KKSeatCraftCoreBackendView *backendView;
@property (nonatomic, strong) UIScrollView *scrollView;
@property (nonatomic, strong) UIView *zoomContentView;

@property (nonatomic, assign) CGFloat minimumZoomScale;
@property (nonatomic, assign) CGFloat maximumZoomScale;

@property (nonatomic, assign) CGFloat zoomScale;
@property (nonatomic, assign) CGPoint contentOffset;

@property (nonatomic, assign) CGFloat currentZoom;
@property (nonatomic, assign) CGPoint currentPanOffset;
@property (nonatomic, assign) CGPoint currentPinchOffset;
@property (nonatomic, assign) CGPoint pinchCenter;
@property (nonatomic, assign) BOOL isTapEnabled;
@property (nonatomic, strong) CADisplayLink *displayLink;
@property (nonatomic, assign) CGSize contentSize;

@end

@implementation KKSeatCraftCoreView {
    std::unique_ptr<kk::SeatCraftCoreApp> app;
    std::shared_ptr<kk::renderer::IOSSeatCraftCoreRenderer> renderer;
}

- (void)dealloc {
    [self.displayLink invalidate];

#if DEBUG
    NSLog(@"[%@ dealloc]", NSStringFromClass(self.class));
#endif
}

- (void)didMoveToWindow {
    [super didMoveToWindow];
    if (self.window) {
        [self setupDisplayLink];
    } else {
        [self.displayLink invalidate];
        self.displayLink = nil;
    }
}
#pragma mark - life cycle
- (instancetype)initWithFrame:(CGRect)frame {
    if ((self = [super initWithFrame:frame])) {
        [self commonInit];
    }
    return self;
}

- (instancetype)initWithCoder:(NSCoder *)aDecoder {
    if ((self = [super initWithCoder:aDecoder])) {
        [self commonInit];
    }
    return self;
}

#pragma mark - Initial Methods
- (void)commonInit {
    /*custom view u want draw in here*/
    self.backgroundColor = [UIColor whiteColor];

    self.zoomScale = 1.0f;
    self.minimumZoomScale = 1.0f;
    self.maximumZoomScale = 1.0f;
    self.contentOffset = CGPointZero;
    self.currentZoom = 1.0f;
    self.currentPanOffset = CGPointZero;
    self.currentPinchOffset = CGPointZero;
    self.pinchCenter = CGPointZero;
    self.isTapEnabled = true;

    app = std::make_unique<kk::SeatCraftCoreApp>();

    [self setupViews];
    [self setupGesture];
}

- (void)setupViews {
    _backendView = [[KKSeatCraftCoreBackendView alloc] initWithFrame:self.bounds];
    _backendView.translatesAutoresizingMaskIntoConstraints = NO;
    _backendView.contentScaleFactor = UIScreen.mainScreen.scale;
    _backendView.delegate = self;

    [self addSubview:_backendView];

    [NSLayoutConstraint activateConstraints:@[
        [_backendView.leadingAnchor constraintEqualToAnchor:self.leadingAnchor],
        [_backendView.topAnchor constraintEqualToAnchor:self.topAnchor],
        [_backendView.trailingAnchor constraintEqualToAnchor:self.trailingAnchor],
        [_backendView.bottomAnchor constraintEqualToAnchor:self.bottomAnchor],
    ]];
}

- (void)setupGesture {

    UITapGestureRecognizer *tap =
        [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleBackendViewClicked:)];
    [self.backendView addGestureRecognizer:tap];

    UIPanGestureRecognizer *pan =
        [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handleBackendViewPan:)];
    pan.delegate = self;
    [self.backendView addGestureRecognizer:pan];

    UIPinchGestureRecognizer *pinch =
        [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(handleBackendViewPinch:)];
    pinch.delegate = self;
    [self.backendView addGestureRecognizer:pinch];
}

- (void)handleBackendViewClicked:(UITapGestureRecognizer *)gesture {
}

- (void)handleBackendViewPan:(UIPanGestureRecognizer *)gesture {
    CGPoint translation = [gesture translationInView:self.backendView];
    if (gesture.state == UIGestureRecognizerStateBegan) {
        self.currentPanOffset = translation;
        self.isTapEnabled = false;
    }
    if (gesture.state == UIGestureRecognizerStateEnded) {
        self.isTapEnabled = true;
        return;
    }
    CGFloat contentScaleFactor = self.backendView.contentScaleFactor;
    self.contentOffset =
        CGPointMake(self.contentOffset.x +
                        (translation.x - self.currentPanOffset.x) * contentScaleFactor,
                    self.contentOffset.y +
                        (translation.y - self.currentPanOffset.y) * contentScaleFactor);
    self.currentPanOffset = translation;
    [self updateZoom:self.zoomScale contentOffset:self.contentOffset];
}

- (void)handleBackendViewPinch:(UIPinchGestureRecognizer *)gesture {
    self.isTapEnabled = false;

    CGPoint center = [gesture locationInView:self.backendView];
    center.x *= self.backendView.contentScaleFactor;
    center.y *= self.backendView.contentScaleFactor;

    if (gesture.state == UIGestureRecognizerStateBegan) {
        self.currentZoom = self.zoomScale;
        self.currentPinchOffset = self.contentOffset;
        self.pinchCenter = center;
    }

    if (gesture.state == UIGestureRecognizerStateEnded) {
        self.isTapEnabled = true;
        return;
    }

    if (gesture.numberOfTouches != 2) {
        return;
    }

    CGFloat scale = MAX(self.minimumZoomScale, MIN(self.maximumZoomScale, self.currentZoom * gesture.scale));
    CGPoint offset;
    offset.x = (self.currentPinchOffset.x - self.pinchCenter.x) * scale / self.currentZoom + center.x;
    offset.y = (self.currentPinchOffset.y - self.pinchCenter.y) * scale / self.currentZoom + center.y;

    self.zoomScale = scale;
    self.contentOffset = offset;
    [self updateZoom:self.zoomScale contentOffset:self.contentOffset];
}

- (void)setupDisplayLink {
    if (self.displayLink) {
        self.displayLink = nil;
    }

    self.displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(update:)];
    [self.displayLink addToRunLoop:[NSRunLoop mainRunLoop] forMode:NSRunLoopCommonModes];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(appDidEnterBackground:)
                                                 name:UIApplicationDidEnterBackgroundNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(appWillEnterForeground:)
                                                 name:UIApplicationWillEnterForegroundNotification
                                               object:nil];
}

- (void)appDidEnterBackground:(NSNotification *)notification {
    [self.displayLink setPaused:YES];
}

- (void)appWillEnterForeground:(NSNotification *)notification {
    [self.displayLink setPaused:NO];
}

- (void)update:(CADisplayLink *)displayLink {
    if (renderer == nullptr) {
        renderer = std::make_shared<kk::renderer::IOSSeatCraftCoreRenderer>(app.get(), (CAEAGLLayer *)self.backendView.layer);
    }

    if (renderer == nullptr) {
        return;
    }

    renderer->draw();
}

- (void)updateZoom:(CGFloat)zoomScale contentOffset:(CGPoint)contentOffset {
    if (app == nullptr) {
        return;
    }

    auto changed = app->updateZoomAndOffset(zoomScale, tgfx::Point(contentOffset.x, contentOffset.y));
    if (changed && renderer) {
        renderer->invalidateContent();
    }
}

- (void)updateMaxMinZoomScalesForCurrentBounds {

    CGSize boundsSize = self.bounds.size;
    CGSize contentViewSize = self.zoomContentView.frame.size;

    CGFloat widthScale = boundsSize.width / contentViewSize.width;
    CGFloat heightScale = boundsSize.height / contentViewSize.height;
    CGFloat minScale = fmin(fmin(widthScale, heightScale), 1.0);
    CGFloat maxScale = boundsSize.width / 126.0;

    if (UIDevice.currentDevice.userInterfaceIdiom == UIUserInterfaceIdiomPad) {
        maxScale = 4.0;
    }

    self.minimumZoomScale = minScale;
    self.maximumZoomScale = maxScale;
    self.zoomScale = minScale;

    [self updateZoom:self.zoomScale contentOffset:self.contentOffset];
}

- (void)configureWithSize:(CGSize)size {
    self.scrollView.contentSize = size;
    [self updateMaxMinZoomScalesForCurrentBounds];
}

- (void)updateContentSize:(CGSize)contentSize {
    self.contentSize = contentSize;

    CGRect frame = CGRectMake(0, 0, contentSize.width, contentSize.height);
    self.zoomContentView.frame = frame;
    [self configureWithSize:contentSize];
}

#pragma mark - KKSeatCraftCoreBackendViewDelegate
- (void)seatCraftCoreBackendViewDidUpdateSize:(KKSeatCraftCoreBackendView *)backendView {
    CALayer *layer = backendView.layer;
    auto width = static_cast<int>(roundf(layer.bounds.size.width * layer.contentsScale));
    auto height = static_cast<int>(roundf(layer.bounds.size.height * layer.contentsScale));
    auto sizeChanged = app->updateScreen(width, height, layer.contentsScale);
    if (sizeChanged && renderer) {
        renderer->updateSize();
    }
}

#pragma mark - UIGestureRecognizerDelegate
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer {
    return YES;
}
@end
