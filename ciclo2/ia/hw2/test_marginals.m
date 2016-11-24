% FACTORS.INPUT(1) contains P(X_1)
FACTORS.INPUT(1) = struct('var', [1], 'card', [2], 'val', [0.11, 0.89]);

% FACTORS.INPUT(2) contains P(X_2 | X_1)
FACTORS.INPUT(2) = struct('var', [2, 1], 'card', [2, 2], 'val', [0.59, 0.41, 0.22, 0.78]);

% FACTORS.INPUT(3) contains P(X_3 | X_2)
FACTORS.INPUT(3) = struct('var', [3, 2], 'card', [2, 2], 'val', [0.39, 0.61, 0.06, 0.94]);

FACTORS.EVIDENCE = ObserveEvidence(FACTORS.INPUT, [2 1; 3 2]);

for i=1:length(FACTORS.EVIDENCE)
    s = sum(FACTORS.EVIDENCE(i).val);
    FACTORS.EVIDENCE(i).val = FACTORS.EVIDENCE(i).val / s;
end

