%%usando datos de la web de las diapos
%%
clear, clc
fileID = fopen('data/0.data','r');
input = textscan(fileID,'%s %s %s %s %s %s %s','Delimiter',',');
fclose(fileID);
%%
C = input{5};
X = [input{1} input{2} input{3} input{4}];

classifier = BuildBayes(X, C);

%%
C_predicted = Predict(X, classifier);
disp('Usando todos los datos:');
confusionmat_ = confusionmat(C, C_predicted')

fileID = fopen('data/0-prueba.data','r');
input = textscan(fileID,'%s %s %s %s %s %s','Delimiter',',');
fclose(fileID);

X = [input{1} input{2} input{3} input{4}];

C_predicted = Predict(X, classifier);
disp('Usando los datos de prueba:');
C_predicted'