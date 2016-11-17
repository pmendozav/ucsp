function ok = HouseholderProdAb(u, x)
    if size(x, 1) == 1
        x = x';
    end
    
    if size(u, 1) == 1
        u = u';
    end
    
    beta = 2 / (u'*u);
    s = u'*x;
    beta = beta * s;
    
    ok = x - beta*u;
end