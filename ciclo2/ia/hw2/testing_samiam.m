% Pruebas usando 'Credit_net.net'
% Prueba 1: Prob. marginal de CreditWorthiness sin Fijar ninguna variable
% Prueba 2: Prob. marginal de CreditWorthiness fijando las variables en
% Reliability=Unreliable y FutureIncome=Not_promising
clc, clear

[F, names, valNames] = ConvertNetwork('Credit_net.net');

disp('****Sin fijar nada:');
result = ComputeMarginal([1], F, [])
Reference = struct('var', [1], 'card', [2], 'val', [0.2685 0.7315])
disp('*******************');

disp('****Fijando Reliability=Unreliable y FutureIncome=Not_promising:');
result = ComputeMarginal([1], F, [6 2; 8 2])
Reference = struct('var', [1], 'card', [2], 'val', [0.0644 0.9356])
disp('*******************');


