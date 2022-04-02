%
%
%
clear
clc

a = arduino("COM3", "Uno");

data = zeros(500,1);

for i=1:length(data)
    data(i) = readVoltage(a, "A0");
end
