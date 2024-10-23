import csv
import argparse
import sys
import numpy as np
from scipy.stats import uniform
from scipy.stats import truncnorm

# Function to generate a set of tasks
def generate_task_set(n, m, rho, sigma_t, mu_u, sigma_l, I):
    task_set = []

    ri = uniform.rvs(loc = 0, scale = I, size = n)
    ri = np.rint(ri).astype(int)

    mu_tj = uniform.rvs(loc = 0, scale = rho * I, size = m)
    mu_tj = np.rint(mu_tj).astype(int)

    tau_ij = []
    tau_i = []
    for mu_t in mu_tj:
        temp_tau = truncnorm.rvs(a = 0, b = np.inf, loc = 1, scale = sigma_t, size = n)
        temp_tau *= mu_t
        temp_tau = list(np.rint(temp_tau).astype(int))
        tau_ij.append(temp_tau)
    
    for i in range(n):
        tau = 0
        for j in range(m):
            tau += tau_ij[j][i]
        tau_i.append(tau)


    mu_l = (1 - mu_u)/mu_u
    li = mu_l*truncnorm.rvs(a = 0, b = np.inf, loc = 1, scale = sigma_l, size = n)

    di = []
    for i in range(n):
        di.append(tau_i[i] + (li[i] * tau_i[i]) + ri[i])
    di = np.rint(di).astype(int)

    for i in range(n):
        task_set.append({
            'release_time': ri[i],
            'deadline': di[i],
            'processing_times': [k[i] for k in tau_ij]
        })
    
    return task_set

# Write task set to CSV
def write_to_csv(task_set, num_processors, output_file):
    with open(output_file, mode='w', newline='') as file:
        writer = csv.writer(file)
        # Write header
        header = ['ri', 'di'] + [f'tau_i{p}' for p in range(1, num_processors + 1)]
        writer.writerow(header)
        
        # Write each task
        for task in task_set:
            # print(task)
            row = [task['release_time'], task['deadline']] + task['processing_times']
            writer.writerow(row)

# Example usage
if __name__ == "__main__":
    # Set up argument parser
    parser = argparse.ArgumentParser(description='Generate task sets for scheduling simulation.')

    # Add arguments for the parameters
    parser.add_argument('-n', type=int, required=True, help='Number of tasks (n)')
    parser.add_argument('-m', type=int, required=True, help='Number of processors (m)')
    parser.add_argument('-rho', type=float, required=True, help='Utilization factor (rho)')
    parser.add_argument('-sigma_t', type=float, required=True, help='Standard deviation of processing time (sigma_t)')
    parser.add_argument('-mu_u', type=float, required=True, help='Mean utilization (mu_u)')
    parser.add_argument('-sigma_l', type=float, required=True, help='Standard deviation of laxity (sigma_l)')
    parser.add_argument('-I', type=int, required=True, help='Maximum release time (I)')
    parser.add_argument('-num_cases', type=int, help='Number of testcases', default=3)
    # Add argument for the output file
    parser.add_argument('-output_dir', type=str, help='Path to output CSV file')

    # Parse the arguments
    args = parser.parse_args()

    # Use the parsed arguments
    n = args.n
    m = args.m
    rho = args.rho
    sigma_t = args.sigma_t
    mu_u = args.mu_u
    sigma_l = args.sigma_l
    I = args.I
    num_cases = args.num_cases
    output_dir = args.output_dir

    for i in range(1, num_cases + 1):
        output_file = f'{output_dir}/testcase_{i}.csv'
    
        # Generate task set
        task_set = generate_task_set(n, m, rho, sigma_t, mu_u, sigma_l, I)

        # Output to CSV file
        write_to_csv(task_set, m, output_file)
