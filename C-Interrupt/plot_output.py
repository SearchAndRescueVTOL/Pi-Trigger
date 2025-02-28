import time
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.animation import FuncAnimation

# Track the last read position
last_line_index = 0

def read_new_timestamps(file_path):
    """Read new timestamps from the file since the last read."""
    global last_line_index
    with open(file_path, "r") as f:
        lines = f.readlines()
    
    # Extract all timestamps as integers
    timestamps = [int(line.strip()) for line in lines]

    
    # Get only the new timestamps
    new_timestamps = timestamps[last_line_index:]
    last_line_index = len(timestamps)  # Update the last read position
    
    return new_timestamps

def plot_differences(frame, file_path, differences, ax, last_timestamp):
    """Update the plot with new differences."""
    new_timestamps = read_new_timestamps(file_path)

    if len(new_timestamps) < 2:
        return  # Not enough new data

    # Compute differences with the previous timestamp
    for i in range(len(new_timestamps)):
        if last_timestamp is not None:
            diff = new_timestamps[i] - last_timestamp
            differences.append(abs(diff - 400_000_000))  # Difference from 400ms (400,000,000 ns)
        last_timestamp = new_timestamps[i]

    ax.clear()
    ax.plot(differences, label="Difference from 400ms")
    ax.axhline(y=0, color='r', linestyle='--', label="Perfect 400ms")
    ax.set_xlabel("Index")
    ax.set_ylabel("Difference in nanoseconds")
    ax.set_title("Difference Between Adjacent Timestamps from 400ms")
    ax.legend()
    ax.grid(True)

    return last_timestamp

def main():
    file_path = "output.txt"
    differences = []
    last_timestamp = None  # Track the last timestamp

    # Set up the plot
    fig, ax = plt.subplots(figsize=(10, 5))

    def update(frame):
        nonlocal last_timestamp
        last_timestamp = plot_differences(frame, file_path, differences, ax, last_timestamp)

    # Create an animated plot that updates every second
    ani = FuncAnimation(fig, update, interval=1000)

    plt.show()

if __name__ == "__main__":
    main()

