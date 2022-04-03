% MATLAB script to collect data from the LDR sensor using an Arduino.
% The raw data is ploted and the sensor's resistance is computed. 
% This will help in characterizing the sensor's illuminance-resistance
% curve.

clear
clc

a = arduino("COM3", "Uno");

data = zeros(5000,1);

for i=1:length(data)
    data(i) = readVoltage(a, "A0");
    pause(0.001);
end

figure
plot(data);
title("LDR Raw Sensor data");
xlabel("Sample");
ylabel("Voltage across the LDR");

V = 5;
R = 9970;
R_ldr = R.*(data./(V-data));
R_LDR = mean(R_ldr);
fprintf(1,"R_LDR = %.3f Ohms\n", R_LDR);