//
//  KKChooseSeatViewController.m
//  SeatCraftCoreExample
//
//  Created by king on 2025/8/7.
//

#import "KKChooseSeatViewController.h"

@import SeatCraftCore;

@interface KKChooseSeatViewController ()
@property (weak, nonatomic) IBOutlet KKSeatCraftCoreView *seatCraftView;

@end

@implementation KKChooseSeatViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.

    NSBundle *bundle = [NSBundle mainBundle];

    KKSeatCraftCoreSeatStatusSvgPathMap *seatStatusSvgPathMap = [KKSeatCraftCoreSeatStatusSvgPathMap new];

    [seatStatusSvgPathMap add:1 path:[bundle pathForResource:@"icon_chooseSeat_canSelected" ofType:@"svg"]];
    [seatStatusSvgPathMap add:2 path:[bundle pathForResource:@"icon_chooseSeat_selected" ofType:@"svg"]];
    [seatStatusSvgPathMap add:3 path:[bundle pathForResource:@"icon_chooseSeat_noSelected" ofType:@"svg"]];

    NSString *svgPath = [bundle pathForResource:@"performbg_2" ofType:@"svg"];
    [self.seatCraftView updateAreaSVG:svgPath];
    [self.seatCraftView updateSeatStatusSvgPathMap:seatStatusSvgPathMap];
    CGSize svgSize = CGSizeMake(8204, 6916);
    CGFloat maxWidth = 1000.0;
    CGFloat scale = 1.0;
    if (svgSize.width > 0.0 && svgSize.width > maxWidth) {
        scale = maxWidth / svgSize.width;
    }

    CGFloat boxWidth = svgSize.width * scale;
    CGFloat boxHeight = svgSize.height * scale;
    [self.seatCraftView updateContentSize:CGSizeMake(boxWidth, boxHeight)];
}

- (void)viewDidLayoutSubviews {
    [super viewDidLayoutSubviews];
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];
}
@end
