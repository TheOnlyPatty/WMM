//
//  FlutterUnityView.m
//  FlutterUnityView
//
//  Created by krispypen on 8/1/2019
//  Updated by Rex Raphael on 8/27/2020.
//

#import "FlutterUnityView.h"

@implementation FLTUnityView

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    return self;
}

- (void)dealloc
{
}

- (void)setUnityView:(UIView *)view
{
    self.uView = view;
    [self setNeedsLayout];
}

- (void)layoutSubviews
{
    [super layoutSubviews];
    [(UIView *)self.uView removeFromSuperview];
    [self insertSubview:(UIView *)self.uView atIndex:0];
    ((UIView *)self.uView).frame = self.bounds;
    [(UIView *)self.uView setNeedsLayout];
}

@end
