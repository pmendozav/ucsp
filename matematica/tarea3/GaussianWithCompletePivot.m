function [P Q L U] = GaussianWithCompletePivot(A)
    n = size(A, 1);
    I = eye(n);
    P = I;
    Q = I;
    M = I;
    for i = 1:min(size(A))
       Mi = I;
       Pi = I;
       Qi = I;
       %permutation
       [r c] = find(abs(A(i:n,i:n)) == max(max(abs(A(i:n,i:n)))));
       r = r + (i - 1);
       c = c + (i - 1);
       Pi([i r(1)], :) = Pi([r(1) i], :);
       Qi([i c(1)], :) = Qi([c(1) i], :);
       A = Pi * A * Qi;
       %reduction
       Mi(1+i:n, i) = -A(1+i:n, i) / A(i, i);
       A = Mi*A;
       %save partial results
       P = Pi*P;
       Q = Q*Qi;
       M = Mi*Pi*M;
    end
    U = A;
    L = P*(inv(M));
end