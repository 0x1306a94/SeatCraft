//
//  ViewController.m
//  SeatCraftCoreExample
//
//  Created by king on 2025/8/7.
//

#import "ViewController.h"

#import "KKChooseSeatViewController.h"

@interface ViewController ()
@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.

    //    self.seatCraftView = [[KKSeatCraftCoreView alloc] initWithFrame:self.view.bounds];
    //    self.seatCraftView.translatesAutoresizingMaskIntoConstraints = NO;
    //
    //    [self.view addSubview:self.seatCraftView];
    //    [NSLayoutConstraint activateConstraints:@[
    //        [self.seatCraftView.leadingAnchor constraintEqualToAnchor:self.view.leadingAnchor],
    //        [self.seatCraftView.topAnchor constraintEqualToAnchor:self.view.topAnchor],
    //        [self.seatCraftView.trailingAnchor constraintEqualToAnchor:self.view.trailingAnchor],
    //        [self.seatCraftView.bottomAnchor constraintEqualToAnchor:self.view.bottomAnchor],
    //    ]];
}

- (void)viewDidAppear:(BOOL)animated {
    [super viewDidAppear:animated];

    //    [self.seatCraftView updateContentSize:CGSizeMake(1000, 843)];
}

- (IBAction)chooseSeatButtonAction:(UIButton *)sender {

    KKChooseSeatViewController *vc = [KKChooseSeatViewController new];
    [self.navigationController pushViewController:vc animated:YES];
}

@end
