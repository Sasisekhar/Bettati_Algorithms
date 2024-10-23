import os
import subprocess
import time
import csv
import matplotlib.pyplot as plt

# Parameters
num_testcases = 5
input_dir = 'input_files'
output_dir = 'output_files'
log_dir = 'logs'
python_script = 'python_scripts/testcases.py'
simulator_exec = './schedule_simulator/bin/e2e_deadlines'

# Step 1: Clean previous scenarios
def clean_previous_scenarios(ask):
    if(ask):
        response = input("Will delete previous scenarios. Are you sure? [y/N] ")
    else:
        response = 'yes'
    
    if response.lower() in ['y', 'yes']:
        print("Cleaning previous files...")
        # Remove old input, output, and log files
        for folder in [input_dir, output_dir, log_dir]:
            for file in os.listdir(folder):
                os.remove(os.path.join(folder, file))
        print("Old files removed.")
    else:
        print("Exiting.")
        exit()

# Step 2: Generate test cases using the Python script
def generate_test_cases(n,m, rho, sigma_t, mu_u, sigma_l, I):
    for i in range(1, num_testcases + 1):
        output_file = f'{input_dir}/testcase_{i}.csv'
        # print(f"Generating test case {i}...")
        subprocess.run(['python3', python_script,
                f'-n={n}', f'-m={m}', f'-rho={rho}',
                f'-sigma_t={sigma_t}', f'-mu_u={mu_u}',
                f'-sigma_l={sigma_l}', f'-I={I}', f'-output_file={output_file}'])

# Step 3: Run the simulator and log output
def run_simulation():
    success_rate = 0

    for i in range(1, num_testcases + 1):
        input_file = f'{input_dir}/testcase_{i}.csv'
        output_file = f'{output_dir}/output_{i}.csv'
        log_file = f'{log_dir}/simulation_{i}.log'

        # print(f"Running simulation for test case {i}...")

        # Run the simulator and capture the return code
        with open(log_file, 'w') as log:
            result = subprocess.run([simulator_exec, input_file, output_file], stdout=log)

        # Check the return code
        if result.returncode == -1:
            # Schedule is non-feasible, delete the input file
            print(f"Test case {i} is non-schedulable. Deleting input file {input_file}.")
            os.remove(input_file)
        else:
            success_rate += 1
    return (success_rate/num_testcases) * 100

# Step 4: Scrape the output files and plot the results
def scrape_and_plot_results():
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

    return sum([max(0, (completion_times[i] - deadlines[i])/len(completion_times)) for i in range(len(completion_times))])

# Main function to run the automation
if __name__ == "__main__":
    clean_previous_scenarios(True)
    start_time = time.time()

    n = 6
    m = 4
    rho = 0.25
    sigma_t = [0.1, 0.2, 0.3]
    mu_u = [0.2, 0.4, 0.6, 0.7]
    sigma_l = 0.5
    I = 50

    tardiness = []

    for j in sigma_t:
        tardiness_mu = []

        for i in mu_u:
            clean_previous_scenarios(False)

            print("Generating test cases for mu_u = ", i)
            # Step 2: Generate test cases
            generate_test_cases(n, m, rho, j, i, sigma_l, I)

            run_simulation()

            sr = scrape_and_plot_results()

            # Step 3: Run the scheduling simulator
            print("tardiness: ", sr)
            tardiness_mu.append(sr)
        tardiness.append(tardiness_mu)

    print(tardiness)
    plt.plot(tardiness[0], label='sigma_t = 0.1', color='blue')
    plt.plot(tardiness[1], label='sigma_t = 0.2', color='green')
    plt.plot(tardiness[2], label='sigma_t = 0.3', color='red')

    plt.xlabel('mu_u')
    plt.ylabel('Tardiness')
    plt.title('Tardiness vs mu_u')
    plt.legend()
    plt.grid(True)
    plt.savefig('T_vs_M.png')
    plt.show()
    # Step 4: Scrape the output files and plot the results
    # scrape_and_plot_results()

    end_time = time.time()
    print(f"Automation completed in {end_time - start_time:.2f} seconds.")
