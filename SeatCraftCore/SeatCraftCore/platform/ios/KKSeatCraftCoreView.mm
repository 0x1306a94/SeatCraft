//
//  KKSeatCraftCoreView.m
//  SeatCraftCore
//
//  Created by king on 2025/8/7.
//

#import "KKSeatCraftCoreView.h"

#import "KKSeatCraftCoreBackendView.h"

#import "KKSeatCraftCoreSeatStatusSvgPathMap.h"

#import "KKSeatCraftCoreSeatStatusSvgPathMap+Private.h"

#import "../../core/SeatCraftCoreApp.hpp"
#import "../../core/renderer/SeatCraftCoreRenderer.hpp"

#import "./renderer/IOSRendererBackend.h"

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

@interface KKSeatCraftCoreView () <KKSeatCraftCoreBackendViewDelegate, UIGestureRecognizerDelegate, UIScrollViewDelegate>
@property (nonatomic, strong) KKSeatCraftCoreBackendView *backendView;
@property (nonatomic, strong) KKSeatCraftCoreAuxiliaryScrollView *scrollView;
@property (nonatomic, strong) UIView *zoomContentView;

@property (nonatomic, assign) CGSize selfSize;
@property (nonatomic, assign) CGFloat svgScale;
@property (nonatomic, assign) CGFloat zoomScale9;
@property (nonatomic, assign) CGFloat zoomScale18;
@property (nonatomic, assign) CGFloat zoomScale30;
@property (nonatomic, assign) CGFloat zoomScale50;

@property (nonatomic, assign) BOOL isTapEnabled;
@property (nonatomic, strong) CADisplayLink *displayLink;
@property (nonatomic, assign) CGSize contentSize;

@property (nonatomic, strong) NSString *areaSVGPath;
@property (nonatomic, strong) KKSeatCraftCoreSeatStatusSvgPathMap *seatStatusSvgPathMap;
@end

@implementation KKSeatCraftCoreView {
    std::shared_ptr<kk::SeatCraftCoreApp> _app;
    std::unique_ptr<kk::renderer::SeatCraftCoreRenderer> _renderer;
}

- (void)dealloc {
    [self.displayLink invalidate];

#if DEBUG
    NSLog(@"[%@ dealloc]", NSStringFromClass(self.class));
#endif
}

- (void)layoutSubviews {
    [super layoutSubviews];
    CGSize size = self.bounds.size;
    if (!CGSizeEqualToSize(size, self.selfSize) && CGSizeEqualToSize(size, CGSizeZero)) {
        self.selfSize = size;
        [self updateContentSize];
    }

    [self adjustZoomContentViewCenter];
    _renderer->draw(true);
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
    /*custom view u want draw in here*/
    self.backgroundColor = [UIColor whiteColor];

    self.selfSize = CGSizeZero;
    self.svgScale = 1.0;
    self.seatStatusSvgPathMap = [[KKSeatCraftCoreSeatStatusSvgPathMap alloc] init];

    [self setupViews];
    [self setupGesture];

    _app = std::make_shared<kk::SeatCraftCoreApp>();
    auto backend = std::make_unique<kk::renderer::IOSRendererBackend>((CAEAGLLayer *)self.backendView.layer);
    _renderer = std::make_unique<kk::renderer::SeatCraftCoreRenderer>(_app, std::move(backend));
}

- (void)setupViews {
    _backendView = [[KKSeatCraftCoreBackendView alloc] initWithFrame:self.bounds];
    _backendView.translatesAutoresizingMaskIntoConstraints = NO;
    _backendView.contentScaleFactor = UIScreen.mainScreen.scale;
    _backendView.delegate = self;

    _scrollView = [[KKSeatCraftCoreAuxiliaryScrollView alloc] initWithFrame:self.bounds];
    _scrollView.translatesAutoresizingMaskIntoConstraints = NO;
    _scrollView.delegate = self;
    _scrollView.alwaysBounceHorizontal = YES;
    _scrollView.alwaysBounceVertical = YES;
    __weak typeof(self) weakSelf = self;
    _scrollView.didLayoutSubviews = ^(KKSeatCraftCoreAuxiliaryScrollView *scrollView) {
        [weakSelf onScrollViewLayoutSubviews];
    };

    _zoomContentView = [[UIView alloc] initWithFrame:self.bounds];
    _zoomContentView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;

    // 一种 trick 的方式，利用UIScrollView 的滑动/缩放处理
    [self addSubview:_scrollView];
    [_scrollView addSubview:_zoomContentView];
    [self addGestureRecognizer:_scrollView.panGestureRecognizer];

    [self addSubview:_backendView];

    [NSLayoutConstraint activateConstraints:@[
        [_backendView.leadingAnchor constraintEqualToAnchor:self.leadingAnchor],
        [_backendView.topAnchor constraintEqualToAnchor:self.topAnchor],
        [_backendView.trailingAnchor constraintEqualToAnchor:self.trailingAnchor],
        [_backendView.bottomAnchor constraintEqualToAnchor:self.bottomAnchor],
        [_scrollView.leadingAnchor constraintEqualToAnchor:self.leadingAnchor],
        [_scrollView.topAnchor constraintEqualToAnchor:self.topAnchor],
        [_scrollView.trailingAnchor constraintEqualToAnchor:self.trailingAnchor],
        [_scrollView.bottomAnchor constraintEqualToAnchor:self.bottomAnchor],
    ]];
}

- (void)setupGesture {

    UITapGestureRecognizer *tap =
        [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(handleBackendViewClicked:)];
    [self.backendView addGestureRecognizer:tap];
}

- (void)handleBackendViewClicked:(UITapGestureRecognizer *)gesture {
    // TODO:
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
    _renderer->draw();
}

- (void)updateZoom:(float)zoomScale contentOffset:(tgfx::Point)contentOffset {
    auto changed = _app->updateZoomAndOffset(zoomScale, contentOffset);
    if (changed) {
        _renderer->invalidateContent();
    }
}

- (void)onScrollViewLayoutSubviews {

    CGFloat zoomScale = self.scrollView.zoomScale;
    CGFloat contentScaleFactor = self.backendView.contentScaleFactor;
    CGPoint contentOffset = self.scrollView.contentOffset;
    contentOffset.x *= contentScaleFactor;
    contentOffset.y *= contentScaleFactor;

    contentOffset.x = -contentOffset.x;
    contentOffset.y = -contentOffset.y;

    [self updateZoom:zoomScale contentOffset:tgfx::Point{static_cast<float>(contentOffset.x), static_cast<float>(contentOffset.y)}];
}

- (void)adjustZoomContentViewCenter {
    UIScrollView *scrollView = self.scrollView;
    CGFloat xcenter = scrollView.center.x, ycenter = scrollView.center.y;
    xcenter = scrollView.contentSize.width > scrollView.frame.size.width ? scrollView.contentSize.width / 2 : xcenter;
    ycenter = scrollView.contentSize.height > scrollView.frame.size.height ? scrollView.contentSize.height / 2 : ycenter;
    self.zoomContentView.center = CGPointMake(xcenter, ycenter);
}

- (void)updateMaxMinZoomScalesForCurrentBounds {

    //    CGSize boundsSize = self.bounds.size;
    //    CGSize contentViewSize = self.contentSize;
    //
    //    CGFloat widthScale = boundsSize.width / contentViewSize.width;
    //    CGFloat heightScale = boundsSize.height / contentViewSize.height;
    //    CGFloat minScale = fmin(fmin(widthScale, heightScale), 1.0);
    //    CGFloat maxScale = boundsSize.width / 126.0;
    //
    //    if (UIDevice.currentDevice.userInterfaceIdiom == UIUserInterfaceIdiomPad) {
    //        maxScale = 4.0;
    //    }
    //
    //    self.scrollView.minimumZoomScale = minScale;
    //    self.scrollView.maximumZoomScale = maxScale;
    //    self.scrollView.zoomScale = minScale;

    CGFloat viewWidth = self.bounds.size.width;
    CGFloat svgScale = self.svgScale;
    CGFloat svgModelScale = 1.0;

    // 1. 计算不同级别下的缩放比例
    self.zoomScale9 = viewWidth / ((svgScale * 36.0) / svgModelScale * 9.0);
    self.zoomScale18 = viewWidth / ((svgScale * 36.0) / svgModelScale * 18.0);
    self.zoomScale30 = viewWidth / ((svgScale * 36.0) / svgModelScale * 36.0);
    self.zoomScale50 = viewWidth / ((svgScale * 36.0) / svgModelScale * 50.0);

    // 计算一个基础缩放比例
    CGFloat baseScale = 1.0 / (svgScale / svgModelScale);
    CGFloat maximumZoomScale = self.zoomScale9;
    // 确保最大缩放比例不小于这个基础比例
    if (maximumZoomScale < baseScale) {
        maximumZoomScale = baseScale;
    }

    // 2. 设置最大缩放比例 (maximumZoomScale)
    self.scrollView.maximumZoomScale = maximumZoomScale;
    // 3. 设置最小缩放比例 (minimumZoomScale)
    CGFloat minimumZoomScale = viewWidth / self.contentSize.width;
    self.scrollView.minimumZoomScale = minimumZoomScale;
    self.scrollView.zoomScale = minimumZoomScale;

    if (self.scrollView.pinchGestureRecognizer && ![self.gestureRecognizers containsObject:self.scrollView.pinchGestureRecognizer]) {
        [self addGestureRecognizer:self.scrollView.pinchGestureRecognizer];
    }

    // 逻辑单位需要转为像素单位
    CGFloat contentScaleFactor = self.backendView.contentScaleFactor;
    CGPoint contentOffset = self.scrollView.contentOffset;
    contentOffset.x *= contentScaleFactor;
    contentOffset.y *= contentScaleFactor;

    contentOffset.x = -contentOffset.x;
    contentOffset.y = -contentOffset.y;

    [self updateZoom:self.scrollView.zoomScale contentOffset:tgfx::Point{static_cast<float>(contentOffset.x), static_cast<float>(contentOffset.y)}];
}

- (void)configureWithSize:(CGSize)size {
    self.scrollView.contentSize = size;
    [self updateMaxMinZoomScalesForCurrentBounds];
    _renderer->draw(true);
}

- (void)updateContentSize {

    auto svgSize = _app->getOriginSize();
    CGSize contentSize = CGSizeMake(svgSize.width * self.svgScale, svgSize.height * self.svgScale);

    self.contentSize = contentSize;
    auto density = _app->density();
    _app->updateContentSize(tgfx::Size{static_cast<float>(contentSize.width * density), static_cast<float>(contentSize.height * density)});

    CGRect frame = CGRectMake(0, 0, contentSize.width, contentSize.height);
    self.zoomContentView.frame = frame;
    [self configureWithSize:contentSize];
}

#pragma mark - KKSeatCraftCoreBackendViewDelegate
- (void)seatCraftCoreBackendViewDidUpdateSize:(KKSeatCraftCoreBackendView *)backendView {
    _renderer->updateSize();
}

#pragma mark - UIGestureRecognizerDelegate
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer {
    return YES;
}

#pragma mark - UIScrollViewDelegate
- (nullable UIView *)viewForZoomingInScrollView:(UIScrollView *)scrollView {
    return self.zoomContentView;
}

- (void)scrollViewDidZoom:(UIScrollView *)scrollView {
    [self adjustZoomContentViewCenter];
}

- (void)scrollViewDidEndZooming:(UIScrollView *)scrollView withView:(nullable UIView *)view atScale:(CGFloat)scale {
    [self adjustZoomContentViewCenter];
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
    CGFloat maxWidth = 1000.0;
    CGFloat scale = 1.0;
    if (svgSize.width > 0.0 && svgSize.width > maxWidth) {
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
