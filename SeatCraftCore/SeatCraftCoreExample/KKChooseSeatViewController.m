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
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];

    [self.seatCraftView updateContentSize:CGSizeMake(1000, 843)];
}
@end
