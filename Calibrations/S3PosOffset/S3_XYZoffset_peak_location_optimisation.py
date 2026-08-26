import pandas as pd
import numpy as np

df = pd.read_csv("sigma_grid.csv")

outer_contour = 0.441
inner_contour = 0.227
number_contours = 8

contour_width = (outer_contour - inner_contour) / number_contours

sigma_acceptance = inner_contour + contour_width

df = df[(df["sigma"] > inner_contour) & (df["sigma"] < sigma_acceptance)]

df = df[(df["mean_1745"] > 1.74) & (df["mean_1745"] < 1.75)]

print(df)

nndc_energy = 1.74591
percentage_diff = (((df["mean_1745"] - nndc_energy) / nndc_energy)).abs() * 100
#optimal_offset = df.loc[percentage_diff.idxmin(), ["x", "y", "z"]].values
optimal_offset = df.loc[df["sigma_1745"].idxmin(), ["x", "y", "z"]].values
print("Optimal XYZ offset (mm) = ", optimal_offset)