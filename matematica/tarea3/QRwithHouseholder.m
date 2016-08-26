function [Q R] = QRwithHouseholder(A)
    n = size(A, 1);
    I = eye(n);
    Q = I;
    
    for i=1:n-1
        Hi = I;
        x = A(i:n,i);
        [hi u rho] = GetHouseholder(x);
        Hi(i:n, i:n) = hi;
        
        A = Hi * A;
        Q = Q * Hi';
    end
    
    R = A;
end