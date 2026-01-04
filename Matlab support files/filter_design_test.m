pkg load signal;

%% Filtro FIR supresor de banda (stopband) para 127 Hz
Fs = 8000;      % Frecuencia de muestreo
N  = 300;       % Orden del filtro
fo = 127;       % Frecuencia a suprimir
d  = 30;        % Ancho de banda

wl = 2*(fo-d)/Fs;
wh = 2*(fo+d)/Fs;
wc = [wl wh];

% FIR
h = fir1(N, wc, 'stop');

% IIR Butterworth
[b1, a1] = butter(2, wc, 'stop')

#{
%% Guardar FIR en archivo
fileID = fopen('h127_fir.dat', 'w');
fprintf(fileID, '%f\n', h);
fclose(fileID);
#}

%% Filtro FIR / IIR stopband para 352 Hz
fo = 352;
wl = 2*(fo-d)/Fs;
wh = 2*(fo+d)/Fs;
wc = [wl wh];


h2 = fir1(N, wc, 'stop');
[b2, a2] = butter(2, wc, 'stop');

#{
fileID2 = fopen('h352_fir.dat', 'w');
fprintf(fileID2, '%f\n', h2);
fclose(fileID2);
#}

figure;
freqz(h, 1, 1024, Fs);
title('Respuesta en frecuencia FIR 127 Hz');

figure;
freqz(b1, a1, 1024, Fs);
title('Respuesta en frecuencia IIR 127 Hz');



