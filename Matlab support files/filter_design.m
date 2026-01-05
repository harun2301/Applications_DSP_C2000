pkg load signal;

M = 1000;
w0 = 2*pi*15/M;   % Frecuencia digital del tono a cancelar
r  = 0.98;        % Qué tan estrecho es el notch (0.95–0.99)

b = [1 -2*cos(w0) 1]
a = [1 -2*r*cos(w0) r^2]

%% Respuesta en frecuencia
Nfft = 2048;                      % resolución en frecuencia
[H, w] = freqz(b, a, Nfft);       % w en rad/muestra

%% Magnitud en dB
Hdb = 20*log10(abs(H) + eps);     % eps evita log(0)

%% Frecuencia normalizada en ciclos/muestra
f = w / (2*pi);

%% Gráfica
figure;
plot(f, Hdb, 'LineWidth', 1.5)
grid on
xlabel('Frecuencia (ciclos / muestra)')
ylabel('|H(e^{j\omega})| [dB]')
title('Respuesta en frecuencia del filtro IIR notch')

xlim([0 0.05])      % zoom donde están los tonos
ylim([-80 5])

