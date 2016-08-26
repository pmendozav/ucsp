function [P L U] = GaussianWithPartialPivot(A)
    eps = 1e-6;
    n = size(A, 1);
    I = eye(n);
    P = I;
    M = I;
    for i = 1:min(size(A))
       Mi = I;
       Pi = I;
       %permutation
       r = find(abs(A(i:n,i)) == max(abs(A(i:n,i)))) + (i - 1);
       if abs(A(i, i) - A(r(1), i)) > eps
           Pi([i r(1)], :) = Pi([r(1) i], :);
       end
       A = Pi*A;
       %reduction
       Mi(i+1:n, i) = -A(i+1:n, i) / A(i, i);
       A = Mi*A;
       %save partial results
       P = Pi*P;
       M = Mi*Pi*M;
    end
    U = A;
    L = P*(inv(M));
end