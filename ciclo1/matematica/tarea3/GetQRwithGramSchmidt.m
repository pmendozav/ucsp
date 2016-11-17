% INPUT:
% A=v=[v_1 v_2... v_n], where v_i \in |R^{mx1}
% OUTPUT:
% Q=[u_1 u_2... u_n]: where v_i \in |R^{mx1}
% R=[alpha_ij]: where R \in |R^{n*n}
function [Q R] = GetQRwithGramSchmidt(v)
    n = size(v, 2);
    Q = v - v;
    R = eye(n);
    
    Q(:, 1) = v(:, 1);
    R(1,1) = 1;
    for k=2:n
        Q(:, k) = v(:, k);
        for i=1:k-1
            R(i, k) = (Q(:, k)'*Q(:, i)) / norm(Q(:, i))^2;
            Q(:, k) = Q(:, k) - R(i, k) * Q(:, i);
        end
    end
end