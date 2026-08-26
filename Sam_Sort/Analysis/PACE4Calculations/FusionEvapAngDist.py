import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
import scipy.stats as stats
import pandas as pd

mpl.rcParams.update({
    "font.family": "serif",
    "mathtext.fontset": "cm",
    "font.size": 11,
    "figure.dpi": 300,
    "savefig.dpi": 300,
    "savefig.bbox": "tight",
    "savefig.pad_inches": 0.02,
    "axes.labelsize": 11,
    "axes.titlesize": 11,
    "axes.spines.top": False,
    "axes.spines.right": False,
    "xtick.direction": "in",
    "ytick.direction": "in",
    "xtick.minor.visible": True,
    "ytick.minor.visible": True,
    "legend.frameon": False,
})


def autoscale_fonts(fig, base=10, ref_width=6.4):
    scale = fig.get_figwidth() / ref_width
    fs = base * scale

    for ax in fig.axes:
        ax.title.set_fontsize(fs * 1.1)
        ax.xaxis.label.set_fontsize(fs)
        ax.yaxis.label.set_fontsize(fs)
        ax.tick_params(labelsize=fs * 0.9)

        leg = ax.get_legend()
        if leg is not None:
            for t in leg.get_texts():
                t.set_fontsize(fs * 0.9)

    if fig._suptitle is not None:
        fig._suptitle.set_fontsize(fs * 1.2)

    return fs


def prettify(a):
    a.minorticks_on()
    a.grid(True, which="major", linewidth=0.6, alpha=0.35)
    a.grid(True, which="minor", linewidth=0.4, alpha=0.20)
    a.tick_params(which="both", top=False, right=False)

def read_angular_dist_csv(channel, particle):
    df = pd.read_csv(f"{channel}/{channel}_{particle}_angDist.csv")
    return df

def main():

    channels = ["17O_7Li",
                "17O_19F",
                "17O_12C"
                ]

    evaporated_ej = ["proton",
                    "neutron",
                    "alpha"
                    ]

    colours = {
        "proton": "tab:red",
        "neutron": "tab:blue",
        "alpha": "tab:green",
        }

    titles = {
        "17O_7Li":  r"$^{17}\mathrm{O} + ^{7}\mathrm{Li}$",
        "17O_19F":  r"$^{17}\mathrm{O} + ^{19}\mathrm{F}$",
        "17O_12C":  r"$^{17}\mathrm{O} + ^{12}\mathrm{C}$",
        }


    for reaction in channels:

        fig, ax = plt.subplots(figsize=(6.4, 4.5))

        for ejectile in evaporated_ej:
            dataframe = read_angular_dist_csv(reaction, ejectile)
            
            # PACE4 bins the data in angular bins of degrees from 0 to 180 (lab angles)
            bin_edges = np.append(dataframe["angle_lower_lim"].to_numpy(),180)

            xsec = dataframe["diff_xsec(mb/sr)"].to_numpy()

            # Step function representing the binned distribution
            ax.step(
                bin_edges,
                np.append(xsec, xsec[-1]),
                where="post",
                color=colours[ejectile],
                linewidth=1.4,
                label=ejectile.capitalize()
            )

            # Vertical lines at the 0 and 180 degree extremities
            ax.vlines(
                0,
                0,
                xsec[0],
                color=colours[ejectile],
                linewidth=1.4
            )
            ax.vlines(
                180,
                0,
                xsec[-1],
                color=colours[ejectile],
                linewidth=1.4
            )

        ax.axvspan(
            131,                        # start of axvspan
            159,                        # end of axvspan
            color = 'palegoldenrod',
            alpha = 0.45,                # opacity
            label = 'S3 acceptance'
        )


        ax.set_xlabel(r"$\theta_{\mathrm{lab}}$ $[^\circ]$")
        ax.set_ylabel(r"$\frac{d\sigma}{d\Omega}$ [mb/sr]")
        ax.set_title(titles[reaction])


        prettify(ax)
        ax.legend()
        autoscale_fonts(fig)

        plt.savefig(f"{reaction}/{reaction}_angDist.pdf")
        

if __name__ == "__main__":
    main()