X = [14; 8; 11]
Y = [14]

D = sqrt(sum(X.^2,2) - 2 * X*Y.' + sum(Y.^2,2).')