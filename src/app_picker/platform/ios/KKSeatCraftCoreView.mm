//
//  KKSeatCraftCoreView.m
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#import <SeatCraftAppPicker/KKSeatCraftCoreSeatStatusSvgPathMap.h>
#import <SeatCraftAppPicker/KKSeatCraftCoreView.h>

#import <SeatCraft/common/common_macro.h>

#import "core/SeatCraftCoreApp.hpp"
#import "core/ui/ElasticZoomPanController.hpp"
#import "core/ui/SeatCraftViewCore.hpp"

#import "KKSeatCraftCoreBackendView.h"
#import "KKSeatCraftCoreSeatStatusSvgPathMap+Private.h"
#import "UIScrollViewZoomPanController.h"
#import "renderer/IOSRendererBackend.h"

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

@property (nonatomic, assign) BOOL isTapEnabled;
@property (nonatomic, strong) CADisplayLink *displayLink;

@property (nonatomic, strong) NSString *areaSVGPath;
@property (nonatomic, strong) KKSeatCraftCoreSeatStatusSvgPathMap *seatStatusSvgPathMap;
@end

@implementation KKSeatCraftCoreView {
    std::shared_ptr<kk::SeatCraftCoreApp> _app;
    std::unique_ptr<kk::ui::SeatCraftViewCore> _viewCore;
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
        _viewCore->invalidateContent();
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

    self.seatStatusSvgPathMap = [[KKSeatCraftCoreSeatStatusSvgPathMap alloc] init];

    [self setupViews];
    [self setupGesture];

    _app = std::make_shared<kk::SeatCraftCoreApp>();
    auto backend = std::make_unique<kk::renderer::IOSRendererBackend>((CAEAGLLayer *)self.backendView.layer);
#if USE_UISCROLLVIEW_TRICK
    auto zoomPanController = std::make_unique<kk::ui::UIScrollViewZoomPanController>();
#else
    auto zoomPanController = std::make_unique<kk::ui::ElasticZoomPanController>();
#endif
    _viewCore = std::make_unique<kk::ui::SeatCraftViewCore>(_app, std::move(backend), std::move(zoomPanController));
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
    _viewCore->handlePan(delta);

    [gesture setTranslation:CGPointZero inView:self.backendView];
}

- (void)handlePinch:(UIPinchGestureRecognizer *)gesture {
    if (gesture.numberOfTouches != 2) {
        return;
    }

    CGPoint centerInView = [gesture locationInView:self.backendView];
    CGFloat contentScaleFactor = self.backendView.contentScaleFactor;
    centerInView.x *= contentScaleFactor;
    centerInView.y *= contentScaleFactor;

    float gestureScale = static_cast<float>(gesture.scale);

    _viewCore->handlePinch(gestureScale, tgfx::Point{static_cast<float>(centerInView.x), static_cast<float>(centerInView.y)});

    gesture.scale = 1.0;
}
#endif

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
    _viewCore->draw();
}

#if USE_UISCROLLVIEW_TRICK

- (void)updateMaxMinZoomScalesForCurrentBounds {
    auto app = _viewCore->getApp();
    float boundsWidth = app->getBoundsSize().width;
    float contentWidth = app->getContentSize().width;

    if (boundsWidth == 0.0 || contentWidth == 0.0) {

        self.scrollView.minimumZoomScale = 1.0f;
        self.scrollView.maximumZoomScale = 1.0f;
        self.scrollView.zoomScale = 1.0f;

        if (self.scrollView.pinchGestureRecognizer && [self.backendView.gestureRecognizers containsObject:self.scrollView.pinchGestureRecognizer]) {
            [self.backendView removeGestureRecognizer:self.scrollView.pinchGestureRecognizer];
        }

        [self adjustZoomContentViewCenter];
        [self updateRendererWithScrollViewState];

        return;
    }

    auto minimumZoomScale = _viewCore->getMinimumZoomScale();
    auto maximumZoomScale = _viewCore->getMaximumZoomScale();
    auto zoomScale = _viewCore->getZoomScale();

    self.scrollView.minimumZoomScale = minimumZoomScale;
    self.scrollView.maximumZoomScale = maximumZoomScale;
    self.scrollView.zoomScale = zoomScale;

    if (self.scrollView.pinchGestureRecognizer && ![self.backendView.gestureRecognizers containsObject:self.scrollView.pinchGestureRecognizer]) {
        [self.backendView addGestureRecognizer:self.scrollView.pinchGestureRecognizer];
    }

    [self adjustZoomContentViewCenter];
    [self updateRendererWithScrollViewState];
}

- (void)updateContentSize {
    auto app = _viewCore->getApp();
    auto svgSize = app->getOriginSize();
    auto svgScale = _viewCore->getSvgScale();

    do {
        CGSize contentSize = CGSizeMake(svgSize.width * svgScale, svgSize.height * svgScale);
        CGRect frame = CGRectMake(0, 0, contentSize.width, contentSize.height);
        self.zoomContentView.bounds = frame;
        self.scrollView.contentSize = contentSize;
    } while (0);

    [self updateMaxMinZoomScalesForCurrentBounds];
}

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

    _viewCore->setZoomScale(zoomScale);
    _viewCore->setContentOffset(tgfxOffsetPixels);
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
    auto sizeChanged = _viewCore->updateSize();
#if USE_UISCROLLVIEW_TRICK
    if (sizeChanged) {
        [self updateContentSize];
    }
#endif
}

#pragma mark - UIGestureRecognizerDelegate
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer {
    return YES;
}

#pragma mark - public
- (void)updateSeatStatusSVGPathMap {
    auto seatSvgMap = [self.seatStatusSvgPathMap getMap];
    _app->updateSeatStatusSVGPathMap(std::move(seatSvgMap));
    _viewCore->draw(true);
}

- (void)updateAreaSVG:(NSString *)path {
    if (path != nil) {
        _viewCore->updateAreaSvgPath(path.UTF8String);
    } else {
        _viewCore->updateAreaSvgPath("");
    }
    _viewCore->invalidateContent();
#if USE_UISCROLLVIEW_TRICK
    [self updateContentSize];
#endif
}

- (void)updateSeatStatusSvgPathMap:(KKSeatCraftCoreSeatStatusSvgPathMap *)seatStatusSvgPathMap {
    self.seatStatusSvgPathMap = seatStatusSvgPathMap;
    [self updateSeatStatusSVGPathMap];
}

@end
