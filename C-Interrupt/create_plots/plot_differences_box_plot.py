import pandas as pd
import matplotlib.pyplot as plt

def analyze_and_plot(csv_filename):
    # Read the CSV file
    df = pd.read_csv(csv_filename)
    
    # Compute statistics
    mean_diff = df["Difference"].mean()
    std_diff = df["Difference"].std()

    print(f"Mean Difference: {mean_diff}")
    print(f"Standard Deviation: {std_diff}")

    # Plot the boxplot
    plt.figure(figsize=(6, 4))
    plt.boxplot(df["Difference"], vert=True)
    plt.ylabel("Time Difference Between Captures [ns]")
    plt.title("Boxplot of Time Differences")
    plt.grid(True)
    plt.show()

# Example usage
analyze_and_plot("differences.csv")

