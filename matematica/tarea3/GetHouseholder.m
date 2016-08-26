function [H u rho] = GetHouseholder(x)
    if size(x, 1) == 1
        x = x';
    end
    m = max(x);
    u = x / m;
    if u(1) ~= 0
        rho = sign(u(1)) * norm(u);
    else
        rho = norm(u);
    end
    u(1) = u(1) + rho;
    rho = -m * rho;
    H = eye(numel(u)) - 2 * (u * u') / (u' * u);
end