import os
import subprocess
import time
import csv
import matplotlib.pyplot as plt

# Parameters
input_dir = 'input_files'
output_dir = 'output_files'
log_dir = 'logs'
python_script = 'python_scripts/testcases.py'
simulator_exec = './schedule_simulator/bin/e2e_deadlines'
plot_dir = 'output_plots'

# Step 1: Clean previous scenarios
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

# Step 3: Run the simulator and log output
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

# Step 4: Scrape the output files and plot the results
def scrape_results():
    deadlines = []
    completion_times = []

    # Extract data from each output file
    for output_file in os.listdir(output_dir):
        output_path = os.path.join(output_dir, output_file)

        try:
            with open(output_path, 'r') as file:
                reader = csv.reader(file)
                next(reader)  # Skip header

                for row in reader:
                    if len(row) < 5:
                        print(f"Skipping malformed row in {output_file}: {row}")
                        continue

                    # Assuming the following structure in your CSV:
                    # Task ID, <other fields>, Completion Time, Deadline
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

    return [sum(tardiness)/denom, [max(tardiness), min(tardiness)]]

def plot_results(plots):
    for plot in plots:
        plt.figure()
        x = plot['x']
        ys = plot['y']
        lines = plot['lines']
        line_label = plot['line_label']
        line_count = 0

        for y in ys:

            _label = ''

            if(len(lines) != 0):
                _label = f'{line_label} = {lines[line_count]}'
                line_count += 1

            if('err' in plot.keys()):
                plt.plot(x, y, 'o', ls='-', label=_label)
            else:
                plt.plot(x, y, 'o', ls='-', label=_label)
        
        plt.ylim(-10, 110)
        plt.xlabel(plot['x_label'])
        plt.ylabel(plot['y_label'])
        plt.title(plot['title'])
        plt.legend()
        plt.grid(True)
        plt.savefig(f'{plot_dir}/{plot['title']}.png')

# Main function to run the automation
if __name__ == "__main__":
    clean_previous_scenarios(True)
    start_time = time.time()

    n = 4
    m = 4
    rho = 0.25
    sigma_t = 0.2
    mu_u = 0.7
    sigma_l = 0.5
    I = 50
    num_cases = [10, 20, 30] #25000

    test_param = "num_cases"

    # for k in sigma_t:
    #     tardiness = []
    #     success_rate = []
    #     for j in rho:
    #         tardiness_mu = []
    #         success_rate_mu = []

    #         for i in mu_u:
    #             clean_previous_scenarios(False)

    #             print(f"Generating testcases for {test_param} = {i}")
    #             # Step 2: Generate test cases
    #             subprocess.run(['python3', python_script,
    #                 f'-n={n}', f'-m={m}', f'-rho={j}',
    #                 f'-sigma_t={k}', f'-mu_u={i}',
    #                 f'-sigma_l={sigma_l}', f'-I={I}', f'-output_dir=input_files', f'-num_cases={num_cases}'])

    #             print(f"Running the simulation for {test_param} = {i}")
    #             sr = run_simulation(num_cases)
    #             tr = scrape_results()

    #             print(f"Mean Tardiness: {tr}, Success Rate: {sr}")

    #             # Step 3: Run the scheduling simulator
    #             tardiness_mu.append(tr)
    #             success_rate_mu.append(sr)
    #         tardiness.append(tardiness_mu)
    #         success_rate.append(success_rate_mu)
        
    #     plot_results([
    #                     {
    #                         "x": mu_u,
    #                         "y": success_rate,
    #                         "lines": rho,
    #                         "title": f"Success Rate vs mu_u for sigma_t = {k}",
    #                         "line_label": "rho",
    #                         "x_label": "mu_u",
    #                         "y_label": "Success Rate"
    #                     },
    #                     {
    #                         "x": mu_u,
    #                         "y": tardiness,
    #                         "lines": rho,
    #                         "title": f"Mean Tardiness vs mu_u for sigma_t = {k}",
    #                         "line_label": "rho",
    #                         "x_label": "mu_u",
    #                         "y_label": "Mean Tardiness"
    #                     }
    #                 ])

    tardiness = []
    tardiness_limits = []
    success_rate = []
    for i in num_cases:
        clean_previous_scenarios(False)

        print(f"Generating testcases for {test_param} = {i}")
        # Step 2: Generate test cases
        subprocess.run(['python3', python_script,
            f'-n={n}', f'-m={m}', f'-rho={rho}',
            f'-sigma_t={sigma_t}', f'-mu_u={mu_u}',
            f'-sigma_l={sigma_l}', f'-I={I}', f'-output_dir=input_files', f'-num_cases={i}'])

        print(f"Running the simulation for {test_param} = {i}")
        sr = run_simulation(i)
        tr = scrape_results()

        print(f"Mean Tardiness: {tr[0]}, Success Rate: {sr}")

        # Step 3: Run the scheduling simulator
        tardiness.append(tr[0])
        tardiness_limits.append(tr[1])
        success_rate.append(sr)
        
    plot_results([
                    {
                        "x": num_cases,
                        "y": [success_rate],
                        "lines": [],
                        "title": f"Success Rate vs mu_u",
                        "line_label": "",
                        "x_label": "mu_u",
                        "y_label": "Success Rate"
                    },
                    {
                        "x": num_cases,
                        "y": [tardiness],
                        "err": [tardiness_limits],
                        "lines": [],
                        "title": f"Mean Tardiness vs mu_u",
                        "line_label": "",
                        "x_label": "mu_u",
                        "y_label": "Mean Tardiness"
                    }
                ])


    end_time = time.time()
    plt.show()
    print(f"Automation completed in {end_time - start_time:.2f} seconds.")
