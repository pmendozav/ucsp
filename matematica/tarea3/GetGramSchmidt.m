function u = GetGramSchmidt(v)
    n = size(v, 2);
    u = v - v;
    
    u(:, 1) = v(:, 1);
    for k=2:n
        u(:, k) = v(:, k);
        for i=1:v-1
            u(:, k) = u(:, k) - (u(:, k)'*u(:, i) / norm(u(:, i))^2) * u(:, i);
        end
        disp(1);
    end
    
    %for k=1:n
    %    u(:, 1) = u(:, 1) / norm(u(:, 1));
    %end
end

