function out = Predict(X, classifier)
    p_xc = classifier.p_xc;
    p_c = classifier.p_c;
    tags_c = classifier.tags_c;
    tags_x = classifier.tags_x;

    [N, dim] = size(X);
    for l=1:N
        x = X(l,:);
        clear x2
        for i=1:size(x,2)
            x2{i} = find(strcmp(tags_x{i}, x{i}));
        end
        x2 = cell2mat(x2);
        
        c_max = -1;
        pos = -1;
        for j=1:size(tags_c, 1)
            pc = p_c(j);
            pxc = 1;
            for r=1:dim
                pxc = pxc * p_xc{r}{j}{x2(r)};
            end
        
            c = pxc * pc;
            if (c > c_max)
                c_max = c;
                pos = j;
            end
        end
        
        out{l} = tags_c{pos};
    end
end
















