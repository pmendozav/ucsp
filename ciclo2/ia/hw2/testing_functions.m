% Basado en FactorTutorial.m
% Pruebas unitarias de cada funcion implementada

clc, clear

% FACTORS.INPUT(1) contains P(X_1)
FACTORS.INPUT(1) = struct('var', [1], 'card', [2], 'val', [0.11, 0.89]);

% FACTORS.INPUT(2) contains P(X_2 | X_1)
FACTORS.INPUT(2) = struct('var', [2, 1], 'card', [2, 2], 'val', [0.59, 0.41, 0.22, 0.78]);

% FACTORS.INPUT(3) contains P(X_3 | X_2)
FACTORS.INPUT(3) = struct('var', [3, 2], 'card', [2, 2], 'val', [0.39, 0.61, 0.06, 0.94]);

disp('****Factor Product:');
result = FactorProduct(FACTORS.INPUT(1), FACTORS.INPUT(2))
Reference = struct('var', [1, 2], 'card', [2, 2], 'val', [0.0649, 0.1958, 0.0451, 0.6942])
disp('*******************');

disp('****Factor Marginalization:');
result = FactorMarginalization(FACTORS.INPUT(2), [2])
Reference = struct('var', [1], 'card', [2], 'val', [1 1])
disp('*******************');

disp('****Observe Evidence:');
result = ObserveEvidence(FACTORS.INPUT, [2 1; 3 2]);
Reference(1) = struct('var', [1], 'card', [2], 'val', [0.11, 0.89]);
Reference(2) = struct('var', [2, 1], 'card', [2, 2], 'val', [0.59, 0, 0.22, 0]);
Reference(3) = struct('var', [3, 2], 'card', [2, 2], 'val', [0, 0.61, 0, 0]);
for i=1:3
    fprintf('Result(%d):\n', i);
    disp(result(i));
    fprintf('Reference(%d):\n', i);
    disp(Reference(i));
end
disp('*******************');

disp('****Observe Evidence:');
result = ComputeJointDistribution(FACTORS.INPUT)
Reference = struct('var', [1, 2, 3], 'card', [2, 2, 2], 'val', [0.025311, 0.076362, 0.002706, 0.041652, 0.039589, 0.119438, 0.042394, 0.652548])
result_val = result.val
reference_val = Reference.val
disp('*******************');

disp('****Observe Evidence:');
result = ComputeMarginal([2, 3], FACTORS.INPUT, [1, 2])
Reference = struct('var', [2, 3], 'card', [2, 2], 'val', [0.0858, 0.0468, 0.1342, 0.7332])
disp('*******************');







