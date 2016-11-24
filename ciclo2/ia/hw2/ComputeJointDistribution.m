%ComputeJointDistribution Computes the joint distribution defined by a set
% of given factors
%
%   Joint = ComputeJointDistribution(F) computes the joint distribution
%   defined by a set of given factors
%
%   Joint is a factor that encapsulates the joint distribution given by F
%   F is a vector of factors (struct array) containing the factors
%     defining the distribution
%

function [Joint table] = ComputeJointDistribution(F)

  % Check for empty factor list
  if (numel(F) == 0)
      warning('Error: empty factor list');
      Joint = struct('var', [], 'card', [], 'val', []);
      return;
  end

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% YOUR CODE HERE:
% Compute the joint distribution defined by F
% You may assume that you are given legal CPDs so no input checking is required.
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    var = [];
    card = [];
    for i=1:length(F)
        var = union(var, F(i).var);
    end
    
    for i=1:length(F)
        [~, map{i}] = ismember(F(i).var, var);
    end
    
    for i=1:length(F)
        card(map{i}) = F(i).card;
    end
    
    val = ones(1, prod(card));
    assignments = IndexToAssignment(1:prod(card), card);
    
    for i=1:length(F)
        f = F(i);
        indx = AssignmentToIndex(assignments(:, map{i}), f.card);
        assig = IndexToAssignment(indx, [f.card]);
        
        p_i = GetValueOfAssignment(f, assig);
        val = val .* p_i;
    end
    
    
% Joint = struct('var', [], 'card', [], 'val', []); % Returns empty factor. Change this.
    Joint = struct('var', var, 'card', card, 'val', val);
    table = assignments;
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
end
