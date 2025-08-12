//
//  KKSeatCraftCoreView.m
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#import <SeatCraftAppPicker/KKSeatCraftCoreSeatStatusSvgPathMap.h>
#import <SeatCraftAppPicker/KKSeatCraftCoreView.h>

#import "KKSeatCraftCoreBackendView.h"

#import "KKSeatCraftCoreSeatStatusSvgPathMap+Private.h"

#import "../../core/SeatCraftCoreApp.hpp"
#import "../../core/renderer/SeatCraftCoreRenderer.hpp"
#import "../../core/ui/ElasticZoomPanController.hpp"

#import "./renderer/IOSRendererBackend.h"

#import <SeatCraft/common/common_macro.h>

#define USE_UISCROLLVIEW_TRICK 1

#if USE_UISCROLLVIEW_TRICK
@interface KKSeatCraftCoreAuxiliaryScrollView : UIScrollView
@property (nonatomic, strong, nullable) void (^didLayoutSubviews)(KKSeatCraftCoreAuxiliaryScrollView *scrollView);
@end

@implementation KKSeatCraftCoreAuxiliaryScrollView

- (void)layoutSubviews {
    [super layoutSubviews];
    if (self.didLayoutSubviews) {
        self.didLayoutSubviews(self);
    }
}

//- (void)setZoomScale:(CGFloat)zoomScale {
//    [super setZoomScale:zoomScale];
//    if (self.didLayoutSubviews) {
//        self.didLayoutSubviews(self);
//    }
//}
//
//- (void)setContentOffset:(CGPoint)contentOffset {
//    [super setContentOffset:contentOffset];
//    if (self.didLayoutSubviews) {
//        self.didLayoutSubviews(self);
//    }
//}

@end
#endif

@interface KKSeatCraftCoreView () <KKSeatCraftCoreBackendViewDelegate, UIGestureRecognizerDelegate, UIScrollViewDelegate>
@property (nonatomic, strong) KKSeatCraftCoreBackendView *backendView;
#if USE_UISCROLLVIEW_TRICK
@property (nonatomic, strong) KKSeatCraftCoreAuxiliaryScrollView *scrollView;
@property (nonatomic, strong) UIView *zoomContentView;
#endif

/// svg 缩放比例， 默认 1.0
@property (nonatomic, assign) float svgScale;

/// 多个缩放级别
@property (nonatomic, assign) float zoomScale9;
@property (nonatomic, assign) float zoomScale18;
@property (nonatomic, assign) float zoomScale30;
@property (nonatomic, assign) float zoomScale50;

@property (nonatomic, assign) BOOL isTapEnabled;
@property (nonatomic, strong) CADisplayLink *displayLink;

@property (nonatomic, strong) NSString *areaSVGPath;
@property (nonatomic, strong) KKSeatCraftCoreSeatStatusSvgPathMap *seatStatusSvgPathMap;
@end

@implementation KKSeatCraftCoreView {
    std::shared_ptr<kk::SeatCraftCoreApp> _app;
    std::unique_ptr<kk::ui::ElasticZoomPanController> _zoomPanController;
    std::unique_ptr<kk::renderer::SeatCraftCoreRenderer> _renderer;
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
        _renderer->invalidateContent();
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
    self.backgroundColor = [UIColor whiteColor];

    self.svgScale = 1.0;
    self.seatStatusSvgPathMap = [[KKSeatCraftCoreSeatStatusSvgPathMap alloc] init];

    [self setupViews];
    [self setupGesture];

    _app = std::make_shared<kk::SeatCraftCoreApp>();
    auto backend = std::make_unique<kk::renderer::IOSRendererBackend>((CAEAGLLayer *)self.backendView.layer);
    _renderer = std::make_unique<kk::renderer::SeatCraftCoreRenderer>(_app, std::move(backend));
    _zoomPanController = std::make_unique<kk::ui::ElasticZoomPanController>();
}

- (void)setupViews {
    _backendView = [[KKSeatCraftCoreBackendView alloc] initWithFrame:self.bounds];
    _backendView.translatesAutoresizingMaskIntoConstraints = NO;
    _backendView.contentScaleFactor = UIScreen.mainScreen.scale;
    _backendView.delegate = self;

    [self addSubview:_backendView];

#if USE_UISCROLLVIEW_TRICK
    _scrollView = [[KKSeatCraftCoreAuxiliaryScrollView alloc] initWithFrame:self.bounds];
    _scrollView.userInteractionEnabled = NO;
    _scrollView.translatesAutoresizingMaskIntoConstraints = NO;
    _scrollView.delegate = self;
    _scrollView.alwaysBounceHorizontal = YES;
    _scrollView.alwaysBounceVertical = YES;
    __weak KKSeatCraftCoreView *weakSelf = self;
    _scrollView.didLayoutSubviews = ^(KKSeatCraftCoreAuxiliaryScrollView *scrollView) {
        UNUSED_PARAM(scrollView);
        [weakSelf updateRendererWithScrollViewState];
    };

    _zoomContentView = [[UIView alloc] initWithFrame:self.bounds];
    _zoomContentView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;

    // 一种 trick 的方式，利用UIScrollView 的滑动/缩放处理
    [self addSubview:_scrollView];
    [_scrollView addSubview:_zoomContentView];
    [self addGestureRecognizer:_scrollView.panGestureRecognizer];
#endif

    [NSLayoutConstraint activateConstraints:@[
        [_backendView.leadingAnchor constraintEqualToAnchor:self.leadingAnchor],
        [_backendView.topAnchor constraintEqualToAnchor:self.topAnchor],
        [_backendView.trailingAnchor constraintEqualToAnchor:self.trailingAnchor],
        [_backendView.bottomAnchor constraintEqualToAnchor:self.bottomAnchor],
#if USE_UISCROLLVIEW_TRICK
        [_scrollView.leadingAnchor constraintEqualToAnchor:self.leadingAnchor],
        [_scrollView.topAnchor constraintEqualToAnchor:self.topAnchor],
        [_scrollView.trailingAnchor constraintEqualToAnchor:self.trailingAnchor],
        [_scrollView.bottomAnchor constraintEqualToAnchor:self.bottomAnchor],
#endif
    ]];
}

- (void)setupGesture {

#if !USE_UISCROLLVIEW_TRICK
    UIPanGestureRecognizer *pan =
        [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(handlePan:)];
    pan.delegate = self;
    [self.backendView addGestureRecognizer:pan];

    UIPinchGestureRecognizer *pinch =
        [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(handlePinch:)];
    pinch.delegate = self;
    [self.backendView addGestureRecognizer:pinch];
#endif
}

#if !USE_UISCROLLVIEW_TRICK
- (void)handlePan:(UIPanGestureRecognizer *)gesture {
    if (gesture.state == UIGestureRecognizerStateBegan) {
        self.isTapEnabled = NO;
    }

    if (gesture.state == UIGestureRecognizerStateEnded) {
        self.isTapEnabled = YES;
        return;
    }

    CGPoint translation = [gesture translationInView:self.backendView];
    CGFloat contentScaleFactor = self.backendView.contentScaleFactor;
    translation.x *= contentScaleFactor;
    translation.y *= contentScaleFactor;

    tgfx::Point delta = tgfx::Point{
        static_cast<float>(translation.x),
        static_cast<float>(translation.y),
    };
    _zoomPanController->handlePan(delta);

    [gesture setTranslation:CGPointZero inView:self.backendView];

    [self updateZoomPanControllerState];
}

- (void)handlePinch:(UIPinchGestureRecognizer *)gesture {
    if (gesture.numberOfTouches != 2) {
        return;
    }

    CGPoint centerInView = [gesture locationInView:self.backendView];
    CGFloat contentScaleFactor = self.backendView.contentScaleFactor;
    centerInView.x *= contentScaleFactor;
    centerInView.y *= contentScaleFactor;

    CGFloat gestureScale = gesture.scale;

    _zoomPanController->handlePinch(gestureScale, tgfx::Point{static_cast<float>(centerInView.x), static_cast<float>(centerInView.y)});

    gesture.scale = 1.0;

    [self updateZoomPanControllerState];
}
#endif

- (void)updateZoomPanControllerState {
#if !USE_UISCROLLVIEW_TRICK

    float currentZoom = _zoomPanController->getZoomScale();
    tgfx::Point currentOffset = _zoomPanController->getContentOffset();

    [self updateZoom:currentZoom contentOffset:currentOffset];
#endif
}

- (void)setupDisplayLink {
    if (self.displayLink) {
        [self.displayLink invalidate];
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
    _renderer->draw();
}

- (void)updateZoom:(float)zoomScale contentOffset:(tgfx::Point)contentOffset {
    auto changed = _app->updateZoomAndOffset(zoomScale, contentOffset);
    if (changed) {
        _renderer->invalidateContent();
    }
}

- (void)updateMaxMinZoomScalesForCurrentBounds {
    float boundsWidth = _zoomPanController->getBounds().width;
    float contentWidth = _zoomPanController->getContentSize().width;

    if (boundsWidth == 0.0 || contentWidth == 0.0) {

        _zoomPanController->setMinimumZoomScale(1.0f);
        _zoomPanController->setMaximumZoomScale(1.0f);
        _zoomPanController->setZoomScale(1.0f);

        self.zoomScale9 = 1.0f;
        self.zoomScale18 = 1.0f;
        self.zoomScale30 = 1.0f;
        self.zoomScale50 = 1.0f;

#if USE_UISCROLLVIEW_TRICK
        self.scrollView.minimumZoomScale = 1.0f;
        self.scrollView.maximumZoomScale = 1.0f;
        self.scrollView.zoomScale = 1.0f;

        if (self.scrollView.pinchGestureRecognizer && [self.backendView.gestureRecognizers containsObject:self.scrollView.pinchGestureRecognizer]) {
            [self.backendView removeGestureRecognizer:self.scrollView.pinchGestureRecognizer];
        }

        [self adjustZoomContentViewCenter];
        [self updateRendererWithScrollViewState];
#else
        [self updateZoomPanControllerState];
#endif

        return;
    }

    float svgScale = self.svgScale;
    float svgModelScale = 1.0f;

    self.zoomScale9 = boundsWidth / ((svgScale * 36.0f) / svgModelScale * 9.0f);
    self.zoomScale18 = boundsWidth / ((svgScale * 36.0f) / svgModelScale * 18.0f);
    self.zoomScale30 = boundsWidth / ((svgScale * 36.0f) / svgModelScale * 36.0f);
    self.zoomScale50 = boundsWidth / ((svgScale * 36.0f) / svgModelScale * 50.0f);

    float minimumZoomScale = boundsWidth / contentWidth;
    float baseScale = 1.0f / (svgScale / svgModelScale);
    float maximumZoomScale = self.zoomScale9;
    if (maximumZoomScale < baseScale) {
        maximumZoomScale = baseScale;
    }

    _zoomPanController->setMinimumZoomScale(static_cast<float>(minimumZoomScale));
    _zoomPanController->setMaximumZoomScale(static_cast<float>(maximumZoomScale));
    _zoomPanController->setZoomScale(static_cast<float>(minimumZoomScale));

#if USE_UISCROLLVIEW_TRICK
    self.scrollView.minimumZoomScale = minimumZoomScale;
    self.scrollView.maximumZoomScale = maximumZoomScale;
    self.scrollView.zoomScale = minimumZoomScale;

    if (self.scrollView.pinchGestureRecognizer && ![self.backendView.gestureRecognizers containsObject:self.scrollView.pinchGestureRecognizer]) {
        [self.backendView addGestureRecognizer:self.scrollView.pinchGestureRecognizer];
    }

    [self adjustZoomContentViewCenter];
    [self updateRendererWithScrollViewState];
#else
    [self updateZoomPanControllerState];
#endif
}

- (void)updateContentSize {
    auto svgSize = _app->getOriginSize();
    auto density = _app->density();

#if USE_UISCROLLVIEW_TRICK
    do {
        CGSize contentSize = CGSizeMake(svgSize.width * self.svgScale, svgSize.height * self.svgScale);
        CGRect frame = CGRectMake(0, 0, contentSize.width, contentSize.height);
        self.zoomContentView.bounds = frame;
        self.scrollView.contentSize = contentSize;
    } while (0);
#endif

    do {
        tgfx::Size contentSize{
            static_cast<float>(svgSize.width * self.svgScale * density),
            static_cast<float>(svgSize.height * self.svgScale * density),
        };

        _zoomPanController->setContentSize(contentSize);
        _app->updateContentSize(contentSize);
    } while (0);

    [self updateMaxMinZoomScalesForCurrentBounds];
    _renderer->draw(true);
}

#if USE_UISCROLLVIEW_TRICK
- (void)adjustZoomContentViewCenter {
    UIScrollView *scrollView = self.scrollView;
    CGFloat xcenter = scrollView.center.x, ycenter = scrollView.center.y;
    xcenter = scrollView.contentSize.width > scrollView.frame.size.width ? scrollView.contentSize.width / 2 : xcenter;
    ycenter = scrollView.contentSize.height > scrollView.frame.size.height ? scrollView.contentSize.height / 2 : ycenter;
    self.zoomContentView.center = CGPointMake(xcenter, ycenter);

    [self updateRendererWithScrollViewState];
}

- (void)updateRendererWithScrollViewState {
    float zoomScale = static_cast<float>(self.scrollView.zoomScale);
    float contentScaleFactor = static_cast<float>(self.backendView.contentScaleFactor);

    // 获取 UIScrollView 的 contentOffset 和 zoomContentView 的 frame.origin
    // 它们都是点的单位
    CGPoint scrollViewContentOffset = self.scrollView.contentOffset;
    CGPoint zoomContentViewOrigin = self.zoomContentView.frame.origin;

    // tgfx 所需的平移量是两者相减
    // contentOffset的增加，意味着内容向左移动，所以需要减去
    tgfx::Point tgfxOffsetPoints{
        static_cast<float>(zoomContentViewOrigin.x - scrollViewContentOffset.x),
        static_cast<float>(zoomContentViewOrigin.y - scrollViewContentOffset.y),
    };

    // 将最终平移量转换为像素
    tgfx::Point tgfxOffsetPixels{
        static_cast<float>(tgfxOffsetPoints.x * contentScaleFactor),
        static_cast<float>(tgfxOffsetPoints.y * contentScaleFactor),
    };

    // 将缩放和偏移量传递给 tgfx 渲染器
    [self updateZoom:zoomScale contentOffset:tgfxOffsetPixels];
}

#pragma mark - UIScrollViewDelegate
- (nullable UIView *)viewForZoomingInScrollView:(UIScrollView *)scrollView {
    return self.zoomContentView;
}

- (void)scrollViewDidZoom:(UIScrollView *)scrollView {
    [self adjustZoomContentViewCenter];
    [self updateRendererWithScrollViewState];
}

- (void)scrollViewDidEndZooming:(UIScrollView *)scrollView withView:(nullable UIView *)view atScale:(CGFloat)scale {
    [self adjustZoomContentViewCenter];
    [self updateRendererWithScrollViewState];
}

#endif

#pragma mark - KKSeatCraftCoreBackendViewDelegate
- (void)seatCraftCoreBackendViewDidUpdateSize:(KKSeatCraftCoreBackendView *)backendView {
    _renderer->updateSize();
    _zoomPanController->setBounds(_app->getBoundsSize());

    [self updateContentSize];
}

#pragma mark - UIGestureRecognizerDelegate
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer {
    return YES;
}

#pragma mark - public
- (void)updateSeatStatusSVGPathMap {
    auto seatSvgMap = [self.seatStatusSvgPathMap getMap];
    _app->updateSeatStatusSVGPathMap(std::move(seatSvgMap));
    _renderer->invalidateContent();
    _renderer->draw(true);
}

- (void)updateAreaSVG:(NSString *)path {
    self.areaSVGPath = path;
    if (path != nil) {
        _app->updateAreaSvgPath(path.UTF8String);
    } else {
        _app->updateAreaSvgPath("");
    }
    _renderer->invalidateContent();

    auto svgSize = _app->getOriginSize();
    float maxWidth = 1000.0f;
    float scale = 1.0f;
    if (svgSize.width > 0.0f && svgSize.width > maxWidth) {
        scale = maxWidth / svgSize.width;
    }
    self.svgScale = scale;

    [self updateContentSize];
}

- (void)updateSeatStatusSvgPathMap:(KKSeatCraftCoreSeatStatusSvgPathMap *)seatStatusSvgPathMap {
    self.seatStatusSvgPathMap = seatStatusSvgPathMap;
    [self updateSeatStatusSVGPathMap];
}

@end
