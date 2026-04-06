import pandas as pd
import numpy as np

# Reading CSV and isolating reflectance
ref_df = pd.read_csv("/home/bmw25494/Software/MetaView_Linux/MetaView_Linux_v1.2.1.2/MetaView_Data/StreamingAssets/SelectedData.csv")
ref = ref_df['Reflectance'].values
print(ref)

# Removing outliers using IQR method
Q1 = np.percentile(ref, 25, method='midpoint')
Q3 = np.percentile(ref, 75, method='midpoint')
IQR = Q3 - Q1
lower = Q1 - 1.5*IQR
upper = Q3 + 1.5*IQR
# Filtering by upper and lower bounds
upper_filtered_ref = [r for r in ref if r <= upper]
filtered_ref = [r for r in upper_filtered_ref if r >= lower]
print(filtered_ref)

# Calculations (average, minimum, maximum)
avg = sum(filtered_ref) / len(filtered_ref)
min = min(filtered_ref)
max = max(filtered_ref)

if avg > 24.40:
    moisture = 0
else: 
    moisture = (avg - 24.4) / -0.1021
print("Average reflectance: ")
print(avg)
print("Minimum reflectance: ")
print(min)
print("Maximum reflectance: ")
print(max)
print("Average Moisture Percentage: ")
print(moisture)

