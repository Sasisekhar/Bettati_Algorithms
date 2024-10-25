import os
import subprocess
import time
import csv
import matplotlib.pyplot as plt
import datetime

# Parameters
input_dir = 'input_files'
output_dir = 'output_files'
log_dir = 'logs'
python_script = 'python_scripts/testcases.py'
simulator_exec = './schedule_simulator/bin/e2e_deadlines'
plot_dir = 'output_plots'

os.makedirs(plot_dir, exist_ok=True)
os.makedirs(input_dir, exist_ok=True)
os.makedirs(output_dir, exist_ok=True)
os.makedirs(log_dir, exist_ok=True)

def clean_previous_scenarios(ask):
    if(ask):
        response = input("Will delete previous scenarios. Are you sure? [y/N] ")
    else:
        response = 'yes'
    
    if response.lower() in ['y', 'yes']:
        # Remove old input, output, and log files
        for folder in [input_dir, output_dir, log_dir]:
            for file in os.listdir(folder):
                os.remove(os.path.join(folder, file))
    else:
        print("Exiting.")
        exit()

def run_simulation(num_cases):
    success_rate = 0

    for i in range(1, num_cases + 1):
        input_file = f'{input_dir}/testcase_{i}.csv'
        output_file = f'{output_dir}/output_{i}.csv'
        log_file = f'{log_dir}/simulation_{i}.log'

        # print(f"Running simulation for test case {i}...")

        # Run the simulator and capture the return code
        with open(log_file, 'w') as log:
            result = subprocess.run([simulator_exec, input_file, output_file], stdout=log)

        # Check the return code
        if (result.returncode == -1) or (result.returncode == 255):
            # Schedule is non-feasible, delete the input file
            os.remove(input_file)
        else:
            success_rate += 1
    return (success_rate/num_cases) * 100

def scrape_results():
    deadlines = []
    completion_times = []

    # Extract data from each output file
    for output_file, input_file in zip(os.listdir(output_dir), os.listdir(input_dir)):
        output_path = os.path.join(output_dir, output_file)
        input_path = os.path.join(input_dir, input_file)

        try:
            with open(output_path, 'r') as out_file, open(input_path, 'r') as in_file: #SHOW DIFFERENCE IN INPUT ALSO
                reader_out = csv.reader(out_file)
                next(reader_out)  # Skip header

                for row in reader_out:
                    if len(row) < 5:
                        print(f"Skipping malformed row in {output_file}: {row}")
                        continue

                    completion_time = float(row[3])  # 4th column is completion time
                    deadline = float(row[4])  # 5th column is deadline

                    deadlines.append(deadline)
                    completion_times.append(completion_time)

        except Exception as e:
            print(f"Error processing file {output_file}: {e}")

    if len(deadlines) != len(completion_times):
        print(f"Warning: Deadlines and Completion times do not match in length. "
              f"Deadlines: {len(deadlines)}, Completion Times: {len(completion_times)}")

    denom = 1
    if len(completion_times) != 0:
        denom = len(completion_times)

    tardiness = [max(0, (completion_times[i] - deadlines[i])) for i in range(len(completion_times))]
    mean_tardiness = sum(tardiness)/denom

    return [mean_tardiness, mean_tardiness]

def plot_results(plots):
    for plot in plots:
        plt.figure(figsize=(8, 6))
        x = plot['x']
        ys = plot['y']
        lines = plot.get('lines', [])
        line_label = plot.get('line_label', '')
        line_count = 0
        line_styles = ['-', '-.', ':', '--']

        if('table' in plot.keys()):
            plt.subplot(2, 1, 1)
        
        for y in ys:
            _label = f'{line_label} = {lines[line_count]}' if lines else ''
            if 'err' in plot:
                y_err = plot['err'][line_count] if isinstance(plot['err'], list) else plot['err']
                plt.errorbar(
                    x, y, marker='o', linestyle=line_styles[line_count % len(line_styles)],
                    yerr=y_err, label=_label, lolims=True
                )
            else:
                plt.plot(x, y, marker='o', linestyle=line_styles[line_count % len(line_styles)], label=_label)
            line_count += 1
        
        if('y_lim' in plot):
            plt.ylim(plot['y_lim'])

        plt.xlabel(plot.get('x_label', 'X-axis'))
        plt.ylabel(plot.get('y_label', 'Y-axis'))
        plt.title(plot.get('title', 'Plot'))
        plt.legend()
        plt.grid(True)

        # Adding a table if specified in the plot dictionary
        if 'table' in plot:
            plt.subplot(2, 1, 2)
            table_data = list(plot['table'].values())
            row_labels = list(plot['table'].keys())
            col_labels = [plot.get('table_title', 'Parameters')]
            
            plt.table(
                cellText=[[value] for value in table_data],
                colWidths=[0.3],  # Adjust width as needed
                rowLabels=row_labels,
                colLabels=col_labels,
                loc='center'
            )
            plt.axis('off')  # Hide the axis for table

        plt.tight_layout()

        # using now() to get current time
        current_time = datetime.datetime.now()

        time_file = str(current_time.day) + '_' + str(current_time.month) + '_' + str(current_time.year) + '_' + str(current_time.hour) + '_' + str(current_time.minute) + '_' + str(current_time.second)
            
        plt.savefig(f'{plot_dir}/{plot['title'] + '_' + time_file}.png')

# Main function to run the automation
if __name__ == "__main__":
    clean_previous_scenarios(True)
    start_time = time.time()

    n = 6
    _m = [4]
    rho = 0.25
    _sigma_t = [0.3]
    mu_u = 0.7
    _sigma_l = [0.05, 0.25, 0.5, 0.75]
    I = 50
    num_cases = 15

    test_param = ["m", "sigma_t", "sigma_l"]

    results = []
    for m in _m:
        tardiness_n = []
        success_rate_n = []
        tardiness_err_n = []
        for sigma_t in _sigma_t:
            tardiness_sigma = []
            success_rate_sigma = []
            tardiness_err_sigma = []
            for sigma_l in _sigma_l:
                clean_previous_scenarios(False)
                print(f"Generating testcases for {test_param} = {m, sigma_t, sigma_l}")
                subprocess.run(['python3', python_script,
                    f'-n={n}', f'-m={m}', f'-rho={rho}',
                    f'-sigma_t={sigma_t}', f'-mu_u={mu_u}',
                    f'-sigma_l={sigma_l}', f'-I={I}', f'-output_dir=input_files', f'-num_cases={num_cases}'])

                print(f"Running the simulation for {test_param} = {m, sigma_t, sigma_l}")
                sr = run_simulation(num_cases)
                tr = scrape_results()

                print(f"Mean Tardiness: {tr[0]}, Success Rate: {sr}")
                tardiness_sigma.append(tr[0])
                success_rate_sigma.append(sr)
                tardiness_err_sigma.append(tr[1])
            tardiness_n.append(tardiness_sigma)
            success_rate_n.append(success_rate_sigma)
            tardiness_err_n.append(tardiness_err_sigma)

        plot_results([
                    {
                        "x": _sigma_l,
                        "y": success_rate_n,
                        "lines": _sigma_t,
                        "title": f"Success Rate vs sigma_l",
                        "line_label": "sigma_t",
                        "x_label": "sigma_l",
                        "y_label": "Success Rate",
                        "y_lim": [-10, 110],
                        "table": {
                            "n": n,
                            "m": m,
                            "rho": rho,
                            "mu_u": mu_u,
                            "num_cases": num_cases
                        }
                    },
                    {
                        "x": _sigma_l,
                        "y": tardiness_n,
                        "err": tardiness_err_n,
                        "lines": _sigma_t,
                        "title": f"Mean Tardiness vs sigma_l",
                        "line_label": "sigma_t",
                        "x_label": "sigma_l",
                        "y_label": "Mean Tardiness",
                        "table": {
                            "n": n,
                            "m": m,
                            "rho": rho,
                            "mu_u": mu_u,
                            "num_cases": num_cases
                        }
                    }
                ])


    end_time = time.time()
    plt.show()
    print(f"Automation completed in {end_time - start_time:.2f} seconds.")
