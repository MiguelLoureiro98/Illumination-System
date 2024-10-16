% MATLAB script to plot the illuminance_resistance curve from known values
% and experimental data.
%

% Values estimated from measurements.
sunny_day = 2.3651e+03;
office_light = 2.7982e+03;
sunset = 1.2126e+04;
twilight = 4.9304e+05;
near_dark = 4.5487e+05;

% Provided by the manufacturer.
dark = 1e+06;
res_10_lux = 4e+04;

% Graph plot.
lux = [3.4, 10, 400, 500, 10000];
res = [twilight, res_10_lux, sunset, office_light, sunny_day];
lux_log = log(lux);
res_log = log(res);
[fit_func, GoF] = fit(res_log', lux_log', "poly1");

figure
plot(fit_func, res_log, lux_log), grid on, title("Curve fitting"), ...
    xlabel("Resistance (Ohms)"), ylabel("Illuminance (Lux)");
    %xlim([res_log(5), res_log(1)]), ylim([lux_log(1), lux_log(5)]);
    
fit_lux = res.^(fit_func.p1).*exp(fit_func.p2);
figure
plot(res, lux, 'b*'), grid on, title("Fitted values in the original coordinates"), ...
    hold on, plot(res, fit_lux, 'r'), legend("Experimental values", "Fitted curve"), ...
    xlabel("Resistance (Ohms)"), ylabel("Illuminance (Lux)"), ...
    xlim([sunny_day, twilight]), ylim([3.4, 10000]);

figure
loglog(res, lux), grid on, title("Illuminance-resistance curve"), ...
    hold on, loglog(res, fit_lux, 'r'), legend("Experimental values", "Fitted curve"), ...
    xlabel("Resistance (Ohms)"), ylabel("Illuminance (Lux)"), ...
    xlim([sunny_day, twilight]), ylim([3.4, 10000]);

fprintf(1, "Fitted curve: \n");
fprintf(1, "log(L) = %.3f*log(R) + %.3f\n", fit_func.p1, fit_func.p2);
