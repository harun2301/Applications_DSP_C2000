clc; clear; close all;

%% Parámetros
M = 1000;              % número de muestras
k = 0:M-1;             % eje de frecuencia discreta (bins)

%% Cargar señales
x  = load("../C support files/data_files/3_tones.dat");     % señal original
y  = load("../C support files/data_files/filtered.dat");    % señal filtrada

%% Verificación rápida
if length(x) ~= M || length(y) ~= M
    error("Las señales no tienen %d muestras", M);
end

%% DFT (FFT)
X = fft(x);
Y = fft(y);

%% Magnitud normalizada
Xmag = abs(X)/M;
Ymag = abs(Y)/M;

%% Sólo espectro positivo
half = 1:M/2;

%% Frecuencia normalizada (ciclos/M)
f = k / M;

%% Gráficas
figure;

subplot(2,1,1)
stem(f(half), Xmag(half), 'filled')
title("Espectro señal original x(n)")
xlabel("Frecuencia (ciclos / M)")
ylabel("|X(k)|")
grid on
xlim([0 0.05])   % zoom donde están los tonos

subplot(2,1,2)
stem(f(half), Ymag(half), 'filled')
title("Espectro señal filtrada y(n)")
xlabel("Frecuencia (ciclos / M)")
ylabel("|Y(k)|")
grid on
xlim([0 0.05])

