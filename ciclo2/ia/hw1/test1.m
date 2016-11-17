%%usando los datos de la tarea
%%
clear, clc
fileID = fopen('data/car.data','r');
input = textscan(fileID,'%s %s %s %s %s %s %s','Delimiter',',');
fclose(fileID);
%%
C = input{7};
X = [input{1} input{2} input{3} input{4} input{5} input{6}];

classifier = BuildBayes(X, C);

%%
C_predicted = Predict(X, classifier);
disp('Usando todos los datos:');
confusionmat_ = confusionmat(C, C_predicted')

fileID = fopen('data/car-prueba.data','r');
input = textscan(fileID,'%s %s %s %s %s %s','Delimiter',',');
fclose(fileID);

X = [input{1} input{2} input{3} input{4} input{5} input{6}];

C_predicted = Predict(X, classifier);
disp('Usando los datos de prueba:');
C_predicted'