function [L U] = GaussianWithoutPivot(A)
    n = size(A, 1);
    L = eye(n);
    for i = 1:min(size(A))
       E = eye(n);
       E(1+i:n, i) = -A(1+i:n, i) / A(i, i);
       A = E*A;
       L = L*(inv(E));
    end
    U = A;
end