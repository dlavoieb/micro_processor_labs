clear;clc;

import_data
pitch = rad2deg(pitch);
roll = rad2deg(roll);

%% Uniform Average Filters
windowSize = 5;
b = (1/windowSize)*ones(1,windowSize);
a = 1;

roll_filter = filter(b, a, roll);
f1 = figure(1);
plot(roll(windowSize:end))
hold on
plot(roll_filter(windowSize:end))
hold off
saveas(f1, 'average_size_5.eps', 'epsc')

windowSize = 10;
b = (1/windowSize)*ones(1,windowSize);
a = 1;

roll_filter = filter(b, a, roll);
f2 = figure(2);
plot(roll(windowSize:end))
hold on
plot(roll_filter(windowSize:end))
hold off
saveas(f2, 'average_size_10.eps', 'epsc')

%% Weighted average filters

windowSize = 5;
b = [0.1, 0.15, 0.5, 0.15, 0.1];
a = 1;

roll_filter = filter(b, a, roll);
f3 = figure(3);
plot(roll(windowSize:end))
hold on
plot(roll_filter(windowSize:end))
hold off
saveas(f3, 'weighted_average_5.eps', 'epsc')
