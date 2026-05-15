%% EE 695 Research - Phase 4 Results Graphs
% Clean report-ready version with improved readability

clear; clc; close all;

%% -----------------------------
% Global figure styling
% -----------------------------
title_fs  = 18;
label_fs  = 15;
tick_fs   = 13;
text_fs   = 12;
axis_lw   = 1.6;
bar_lw    = 1.2;

%% -----------------------------
% Phase 4A: Task latency data
% -----------------------------
latency_ms = [1783, 1151, 1122, 1927, 946, 1453, 1142, 616, 1193, 1902];
read_num = 1:numel(latency_ms);

lat_min = min(latency_ms);
lat_max = max(latency_ms);
lat_avg = mean(latency_ms);
lat_med = median(latency_ms);

%% --------------------------------
% Phase 4B / 4C scheduler summaries
% --------------------------------
baseline_samples = 48;
baseline_min_period = 2007;
baseline_max_period = 2008;
baseline_avg_period = 2007.44;
baseline_min_error = 7;
baseline_max_error = 8;
baseline_avg_error = 7.44;
baseline_jitter = 1;

interference_samples = 45;
interference_min_period = 2379;
interference_max_period = 2381;
interference_avg_period = 2380.16;
interference_min_error = 379;
interference_max_error = 381;
interference_avg_error = 380.16;
interference_jitter = 2;

avg_period_increase = interference_avg_period - baseline_avg_period;
avg_error_increase  = interference_avg_error - baseline_avg_error;

%% -----------------------------
% Figure 5.2: Task Latency
% -----------------------------
fig1 = figure('Name','Figure 5.2','NumberTitle','off', ...
    'Color','w','Position',[100 100 1100 650]);

bar(read_num, latency_ms, 'LineWidth', bar_lw);
xlabel('Characteristic Read Number', 'FontSize', label_fs, 'FontWeight', 'bold', 'Color', 'k');
ylabel('Latency (ms)', 'FontSize', label_fs, 'FontWeight', 'bold', 'Color', 'k');
title('Figure 5.2. Sensor-to-BLE Read Latency for Ten Characteristic Reads', ...
    'FontSize', title_fs, 'FontWeight', 'bold', 'Color', 'k');
xticks(read_num);
grid on;
box on;

ax = gca;
ax.FontSize = tick_fs;
ax.LineWidth = axis_lw;
ax.XColor = 'k';
ax.YColor = 'k';
ax.GridColor = [0.4 0.4 0.4];
ax.GridAlpha = 0.3;
ax.Color = 'w';

ylim([0, max(latency_ms) * 1.18]);

for i = 1:numel(latency_ms)
    text(read_num(i), latency_ms(i) + max(latency_ms)*0.02, sprintf('%d', latency_ms(i)), ...
        'HorizontalAlignment', 'center', ...
        'FontSize', text_fs, ...
        'FontWeight', 'bold', ...
        'Color', 'k');
end

exportgraphics(fig1, 'Figure1_Sensor_to_BLE_Read_Latency.png', ...
    'Resolution', 300, 'BackgroundColor', 'white');

%% -----------------------------------------
% Figure 5.3: Average Period Comparison
% -----------------------------------------
fig2 = figure('Name','Figure 5.3','NumberTitle','off', ...
    'Color','w','Position',[120 120 950 650]);

period_data = [baseline_avg_period, interference_avg_period];
bar(period_data, 'LineWidth', bar_lw);
set(gca, 'XTickLabel', {'Baseline', 'Interference'});
ylabel('Average Period (ms)', 'FontSize', label_fs, 'FontWeight', 'bold', 'Color', 'k');
title('Figure 5.3. Average Sensor Thread Period Under Baseline and Interference Conditions', ...
    'FontSize', title_fs, 'FontWeight', 'bold', 'Color', 'k');
grid on;
box on;

ax = gca;
ax.FontSize = tick_fs;
ax.LineWidth = axis_lw;
ax.XColor = 'k';
ax.YColor = 'k';
ax.GridColor = [0.4 0.4 0.4];
ax.GridAlpha = 0.3;
ax.Color = 'w';

ylim([0, max(period_data) * 1.15]);

for i = 1:numel(period_data)
    text(i, period_data(i) + max(period_data)*0.025, sprintf('%.2f ms', period_data(i)), ...
        'HorizontalAlignment', 'center', ...
        'FontSize', text_fs, ...
        'FontWeight', 'bold', ...
        'Color', 'k');
end

exportgraphics(fig2, 'Figure2_Avg_Sensor_Thread_Period.png', ...
    'Resolution', 300, 'BackgroundColor', 'white');

%% -----------------------------------------
% Figure 5.4: Average Timing Error Comparison
% -----------------------------------------
fig3 = figure('Name','Figure 5.4','NumberTitle','off', ...
    'Color','w','Position',[140 140 950 650]);

error_data = [baseline_avg_error, interference_avg_error];
bar(error_data, 'LineWidth', bar_lw);
set(gca, 'XTickLabel', {'Baseline', 'Interference'});
ylabel('Average Timing Error (ms)', 'FontSize', label_fs, 'FontWeight', 'bold', 'Color', 'k');
title('Figure 5.4. Average Sensor Thread Timing Error Under Baseline and Interference Conditions', ...
    'FontSize', title_fs, 'FontWeight', 'bold', 'Color', 'k');
grid on;
box on;

ax = gca;
ax.FontSize = tick_fs;
ax.LineWidth = axis_lw;
ax.XColor = 'k';
ax.YColor = 'k';
ax.GridColor = [0.4 0.4 0.4];
ax.GridAlpha = 0.3;
ax.Color = 'w';

ylim([0, max(error_data) * 1.18]);

for i = 1:numel(error_data)
    text(i, error_data(i) + max(error_data)*0.025, sprintf('%.2f ms', error_data(i)), ...
        'HorizontalAlignment', 'center', ...
        'FontSize', text_fs, ...
        'FontWeight', 'bold', ...
        'Color', 'k');
end

exportgraphics(fig3, 'Figure3_Avg_Sensor_Thread_Timing_Error.png', ...
    'Resolution', 300, 'BackgroundColor', 'white');

%% -----------------------------------------
% Print results to command window
% -----------------------------------------
fprintf('\n===== Phase 4A Task Latency Summary =====\n');
fprintf('Samples   : %d\n', numel(latency_ms));
fprintf('Minimum   : %.0f ms\n', lat_min);
fprintf('Maximum   : %.0f ms\n', lat_max);
fprintf('Average   : %.2f ms\n', lat_avg);
fprintf('Median    : %.2f ms\n', lat_med);

fprintf('\n===== Phase 4B Baseline Scheduler Summary =====\n');
fprintf('Samples       : %d\n', baseline_samples);
fprintf('Min Period    : %d ms\n', baseline_min_period);
fprintf('Max Period    : %d ms\n', baseline_max_period);
fprintf('Avg Period    : %.2f ms\n', baseline_avg_period);
fprintf('Min Error     : +%d ms\n', baseline_min_error);
fprintf('Max Error     : +%d ms\n', baseline_max_error);
fprintf('Avg Error     : +%.2f ms\n', baseline_avg_error);
fprintf('Jitter Range  : %d ms\n', baseline_jitter);

fprintf('\n===== Phase 4C Interference Scheduler Summary =====\n');
fprintf('Samples       : %d\n', interference_samples);
fprintf('Min Period    : %d ms\n', interference_min_period);
fprintf('Max Period    : %d ms\n', interference_max_period);
fprintf('Avg Period    : %.2f ms\n', interference_avg_period);
fprintf('Min Error     : +%d ms\n', interference_min_error);
fprintf('Max Error     : +%d ms\n', interference_max_error);
fprintf('Avg Error     : +%.2f ms\n', interference_avg_error);
fprintf('Jitter Range  : %d ms\n', interference_jitter);

fprintf('\n===== Baseline vs Interference Comparison =====\n');
fprintf('Average Period Increase : +%.2f ms\n', avg_period_increase);
fprintf('Average Error Increase  : +%.2f ms\n', avg_error_increase);