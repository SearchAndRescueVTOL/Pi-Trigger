import csv

def process_timestamps(input_filename, output_filename):
    with open(input_filename, "r") as file:
        timestamps = [int(line.strip()) for line in file.readlines()]
    
    differences = [timestamps[i] - timestamps[i-1] for i in range(1, len(timestamps))]

    with open(output_filename, "w", newline="") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(["Difference"])
        
        for diff in differences:
            writer.writerow([diff])

# Example usage
process_timestamps("output.txt", "differencesPolling.csv")

