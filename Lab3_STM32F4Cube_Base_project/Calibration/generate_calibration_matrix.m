%% Generate the calibration matrix.

% Values for the Z axis pointing down.
Y1 = ones(500, 1) * [0 0 1];
w1 = horzcat(csvread('Z_down.csv', 0, 1), ones(500, 1));

% Values for the Z axis pointing up.
Y2 = ones(500, 1) * [0 0 -1];
w2 = horzcat(csvread('Z_up.csv', 0, 1), ones(500, 1));

% Values for the Y axis pointing down.
Y3 = ones(500, 1) * [0 1 0];
w3 = horzcat(csvread('Y_down.csv', 0, 1), ones(500, 1));

% Values for the Y axis pointing up.
Y4 = ones(500, 1) * [0 -1 0];
w4 = horzcat(csvread('Y_up.csv', 0, 1), ones(500, 1));

% Values for the X axis pointing down.
Y5 = ones(500, 1) * [1 0 0];
w5 = horzcat(csvread('X_down.csv', 0, 1), ones(500, 1));

% Values for the X axis pointing down.
Y6 = ones(500, 1) * [-1 0 0];
w6 = horzcat(csvread('X_up.csv', 0, 1), ones(500, 1));


% Combine the equations into the Y and w vectors.
Y = [ Y1 ; Y2 ; Y3 ; Y4 ; Y5 ; Y6 ];
w = [ w1 ; w2 ; w3 ; w4 ; w5 ; w6 ];

% Since w * X = Y, we can use the least squares method to find X.
X = w\Y;