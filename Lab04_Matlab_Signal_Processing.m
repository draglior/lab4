clc, clearvars, close all

%% Lab04: Generation, Processing, and Analysis of Sine Waves

%% 1) Read Data
% Load the provided signal 'signal.csv' using the 'csvread()' function and
% split the signal and time into two separate vectors.

fprintf('1) Start.\n')

 
data = readmatrix('signal.csv');
time = data(:, 4);
signal = data(:, 5);


% Plot the signal
figure(1)

plot(time, signal);
title('Raw Signal');
xlabel('Time (s)');
ylabel('V_out (V)');
grid on;
hold on;

fprintf('1) Done.\n')

%% 2/3) Butterworth Filter
% Design a Butterworth filter that fulfills the required characteristics 
% given in the assignment description. Use the built-in functions of
% Matlab. The 'doc' and 'help' functions might be useful to obtain detailed
% information.

% 2) First, calculate the required filter order and cutoff frequency and
% print the result.
% Define the desired specifications for the filter

fprintf('2) Start.\n')

% Define the specifications for the Butterworth filter
Fs = 5000; % Sampling frequency in Hz (1 / 2e-4)
Fpass = 30; % Passband frequency in Hz
Fstop = 200; % Stopband frequency in Hz
Apass = 3; % Passband ripple in dB
Astop = 40; % Stopband attenuation in dB

% Calculate the normalized frequencies
Wn = Fpass / (Fs / 2);
Wp = Fstop / (Fs / 2);

% Calculate the filter order and cutoff frequency
[n, Wn] = buttord(Wn, Wp, Apass, Astop);

fc = Wn * (Fs / 2); %cut off freq

fprintf('2) Done.\n')

% 3) Calculate the filter coefficients and apply them to the signal, i.e.,
% filter the signal. Plot the filtered signal into the same figure as the
% original signal. Make sure to add a title, the axis descriptions, and a
% legend.

fprintf('3) Start.\n')

% Calculate the filter coefficients using the butter function
[b, a] = butter(n, Wn);

% Apply the filter to the signal
filtered_signal = filter(b, a, signal);

% Plot the filtered signal
plot(time, filtered_signal);
title('Filtered vs Unfiltered Signal');
xlabel('Time (s)');
ylabel('V_out (V)');
legend('Raw Signal', 'Filtered Signal');
grid on;
hold off;

fprintf('3) Done.\n')

%% 4. Fourier Transform
% Calculate the single-sided Fourier transform of the filtered signal.

fprintf('4) Start.\n')

% 4.1) First, obtain the length of the original and filtered signal and 
% calculate their means. Print both mean values.

% Obtain the length of the original and filtered signal
% If length is the same = the input and output vectors match, did not
% truncate the signal anywhere
len_original = length(signal);
len_filtered = length(filtered_signal);

% Calculate the means of both signals
% If the mean is almost the same, that means there is a small DC offset,
% Buttworth low-pass changed the baseline
mean_original = mean(signal);
mean_filtered = mean(filtered_signal);

% Print the results
fprintf('Length of original signal: %d\n', len_original);
fprintf('Length of filtered signal: %d\n', len_filtered);
fprintf('Mean of original signal: %.4f\n', mean_original);
fprintf('Mean of filtered signal: %.4f\n', mean_filtered);

% 4.2) Do the FFT for both signals; see the docs and lecture slides for
% help. Make sure to remove the mean from the signals.

% 4.2.1.) Original signal

% Remove the mean from the original signal
signal_nomean = signal - mean_original;

% Perform the FFT on the demeaned original signal
Y_original = fft(signal_nomean);

% Calculate the frequency axis
f_original = (0:len_original-1) * (Fs / len_original);

% Compute the single-sided spectrum
P2_original = abs(Y_original / len_original);
P1_original = P2_original(1:len_original/2+1);
P1_original(2:end-1) = 2 * P1_original(2:end-1);

% 4.2.2) Filtered signal

% Remove the mean from the filtered signal
filtered_signal_demeaned = filtered_signal - mean_filtered;

% Perform the FFT on the demeaned filtered signal
Y_filtered = fft(filtered_signal_demeaned);

% Calculate the frequency axis for the filtered signal
f_filtered = (0:len_filtered-1) * (Fs / len_filtered);

% Compute the single-sided spectrum for the filtered signal
P2_filtered = abs(Y_filtered / len_filtered);
P1_filtered = P2_filtered(1:len_filtered/2+1);
P1_filtered(2:end-1) = 2 * P1_filtered(2:end-1);

% Plot the single-sided spectrum of the filtered signal up to 500 Hz
f_limit = 500; % Frequency limit for visualization
freq_indices = f_filtered <= f_limit;

figure(2)
plot(f_filtered(freq_indices), P1_filtered(freq_indices));
title('Spectrum of Filtered Signal');
xlabel('Frequency (Hz)');
ylabel('Amplitude');
hold on
plot(f_original(freq_indices), P1_original(freq_indices));
legend('Filtered Signal', 'Original Signal');
grid on;

fprintf('4) Done.\n')

%% 5. Frequency Identification
% Write a function that automatically detects a signals frequency based
% on its frequency spectrum. You can assume there's only a single signal
% and noise has been removed. The function must return the amplitude and
% the frequency of this signal.

fprintf('5) Start.\n')

% 5.2) What is the frequency of the signal you have analyzed?
% Call the function to identify the frequency and amplitude of the original signal
[peak_amplitude, signal_frequency] = identify_signal_frequency(P1_original, f_original);

% Print the results
fprintf('Frequency: %.2f Hz\n', signal_frequency);
fprintf('Amplitude: %.4f\n', peak_amplitude);

%{ 
    Add your code here
%}

fprintf('5) Done.\n')

% 5.1) Define function
function [amplitude, frequency] = identify_signal_frequency(P1, f)

    [amplitude, peak_index] = max(P1);
    frequency = f(peak_index);
end
 