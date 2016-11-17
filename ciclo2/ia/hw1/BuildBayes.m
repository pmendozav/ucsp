function out = BuildBayes(X, C)
    [N, dim] = size(X);

    tag_c = unique(C,'stable');
    p_ci = cellfun(@(x) sum(ismember(C,x)), tag_c, 'un', 0);
    % p_ci = cell2mat(p_ci) / N;

    for i=1:dim
        xi = X(:,i);
        tags_xi{i} = unique(xi, 'stable');

        for j=1:size(tag_c,1)
            oc_cj = strcmp(C, tag_c(j));

            for k=1:size(tags_xi{i},1)
                tag = tags_xi{i}{k};
                pxc = sum(and(strcmp(xi, tag), oc_cj));
                pxc = pxc / p_ci{j};
                p_xc{i}{j}{k} = pxc;
            end
        end
    end

    p_ci = cell2mat(p_ci) / N;
    
    out.tags_x = tags_xi;
    out.tags_c = tag_c;
    out.p_c = p_ci;
    out.p_xc = p_xc;
end