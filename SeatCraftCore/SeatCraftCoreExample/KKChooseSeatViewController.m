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

    KKSeatCraftCoreSeatStatusSvgPathMap *seatStatusSvgPathMap = self.seatCraftView.seatStatusSvgPathMap;

    [seatStatusSvgPathMap add:1 path:[bundle pathForResource:@"icon_chooseSeat_canSelected" ofType:@"svg"]];
    [seatStatusSvgPathMap add:2 path:[bundle pathForResource:@"icon_chooseSeat_selected" ofType:@"svg"]];
    [seatStatusSvgPathMap add:3 path:[bundle pathForResource:@"icon_chooseSeat_noSelected" ofType:@"svg"]];

    NSString *svgPath = [bundle pathForResource:@"performbg_2" ofType:@"svg"];
    [self.seatCraftView updateAreaSVG:svgPath];
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];

    [self.seatCraftView updateContentSize:CGSizeMake(1000, 843)];
}
@end
