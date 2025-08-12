//
//  ViewController.m
//  SeatCraftAppPickerExample
//
//  Created by king on 2025/8/12.
//

#import "ViewController.h"

#import "KKChooseSeatViewController.h"

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (IBAction)chooseSeatButtonAction:(UIButton *)sender {
    KKChooseSeatViewController *vc = [KKChooseSeatViewController new];
    [self.navigationController pushViewController:vc animated:YES];
}
@end
