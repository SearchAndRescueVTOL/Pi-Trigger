import csv
import numpy as np
import matplotlib.pyplot as plt

def analyze_and_plot(csv_filename):
    differences = []

    # Read differences from CSV
    with open(csv_filename, "r") as csvfile:
        reader = csv.reader(csvfile)
        next(reader)  # Skip header
        for row in reader:
            differences.append(int(row[0]))

    # Compute statistics
    mean_diff = np.mean(differences)
    std_diff = np.std(differences)

    print(f"Mean Difference: {mean_diff}")
    print(f"Standard Deviation: {std_diff}")

    # Plot the differences
    plt.figure(figsize=(8, 5))
    plt.plot(differences, linestyle="-", linewidth=2, color="b", label="Time Differences")  # Thicker line, no markers
    plt.axhline(mean_diff, color="r", linestyle="--", linewidth=2, label=f"Mean: {mean_diff:.2f}")  # Thicker mean line
    plt.xlabel("Index")
    plt.ylabel("Difference")
    plt.title("Time Differences Plot")
    plt.legend()
    plt.grid(True)
    plt.show()

# Example usage
analyze_and_plot("differences.csv")

