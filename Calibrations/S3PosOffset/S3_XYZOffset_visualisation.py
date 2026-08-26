import pandas as pd
import numpy as np
import pyvista as pv

df = pd.read_csv("sigma_grid.csv")

x = np.sort(df.x.unique())
y = np.sort(df.y.unique())
z = np.sort(df.z.unique())

nx = len(x)
ny = len(y)
nz = len(z)

sigma = df["sigma"].values.reshape((nx, ny, nz), order="C")

min_idx = df["sigma"].idxmin()
min_point = df.loc[min_idx, ["x", "y", "z"]].values
print("Optimal XYZ offset (mm) = ",min_point)

grid = pv.ImageData()

grid.dimensions = (nx, ny, nz)

grid.origin = (x.min(), y.min(), z.min())

grid.spacing = (
    x[1]-x[0],
    y[1]-y[0],
    z[1]-z[0]
)

grid["sigma"] = sigma.flatten(order="F")

# Defines the amount and ranges of the contours
levels = np.linspace(df.sigma.min(),
                     df.sigma.max()-2,
                     8)

contours = grid.contour(levels, scalars="sigma")

plotter = pv.Plotter()

plotter.add_mesh(
    contours,
    cmap="viridis",
    opacity=0.5,
    smooth_shading=True,
)

plotter.add_axes()

plotter.show_grid(
    xtitle="X offset (mm)",
    ytitle="Y offset (mm)",
    ztitle="Z offset (mm)"
)

marker = pv.Sphere(radius=0.02, center=min_point)

plotter.add_mesh(marker, color="red")

plotter.add_point_labels(
    [min_point],
    ["Min σ"],
    font_size=34,
    point_color="red",
    text_color="red",
)

#plotter.export_html("S3_XYZ_contour.html") # run the HTML with xdg-open S3_XYZ_contour.html

plotter.open_movie("S3_XYZ_contour.mp4", framerate=30)

n_frames = 360

for i in range(n_frames):
    plotter.camera.azimuth += 1
    plotter.render()
    plotter.write_frame()

plotter.close()