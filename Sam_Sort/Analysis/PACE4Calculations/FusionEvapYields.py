import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
import scipy.stats as stats
import pandas as pd

# Maps the atomic numbers to their elemental symbols 
ELEMENTS = {
    1: "H", 2: "He", 3: "Li", 4: "Be", 5: "B",
    6: "C", 7: "N", 8: "O", 9: "F", 10: "Ne",
    11: "Na", 12: "Mg", 13: "Al", 14: "Si", 15: "P",
    16: "S", 17: "Cl", 18: "Ar", 19: "K", 20: "Ca",
}


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

def read_yield_csv(channel):
    df = pd.read_csv(f"{channel}/{channel}_yields.csv")
    return df

def main():

    channels = ["17O_7Li",
                "17O_19F",
                "17O_12C"
                ]

    titles = {
        "17O_7Li":  r"$^{17}\mathrm{O} + ^{7}\mathrm{Li}$",
        "17O_19F":  r"$^{17}\mathrm{O} + ^{19}\mathrm{F}$",
        "17O_12C":  r"$^{17}\mathrm{O} + ^{12}\mathrm{C}$",
        }

    for reaction in channels:
        yield_df = read_yield_csv(reaction)

        yield_df["A"] = yield_df["Z"] + yield_df["N"]
        # Chemical symbol
        yield_df["isotope"] = (yield_df["A"].astype(int).astype(str) + yield_df["Z"].map(ELEMENTS))
        # Formats the chemical symbol into standard notation
        yield_df["isotope"] = [rf"$^{{{A}}}\mathrm{{{ELEMENTS[Z]}}}$" for A, Z in zip(yield_df["A"], yield_df["Z"])]
        # Sort by cross-section, largest first
        yield_df = yield_df.sort_values("xsec(mb)", ascending=False)

        fig, ax = plt.subplots(figsize=(11, 6))
        
        x = np.arange(len(yield_df))
        bars = ax.bar(
                x,                      # Plots the length of the dataframe on the x axis 
                yield_df["xsec(mb)"],
                width=0.8,
                edgecolor='black',
                color='steelblue'
            )

        # Highlight 21Ne if present
        is_21Ne = (yield_df["Z"] == 10) & (yield_df["A"] == 21)
        if is_21Ne.any():
            idx = np.where(is_21Ne)[0][0]

            bars[idx].set_facecolor("red")
            bars[idx].set_edgecolor("black")
            bars[idx].set_alpha(0.9)

        ax.set_xticks(x)
        ax.set_xticklabels(yield_df["isotope"]) # Assigns the tick labels to be the chemical symbols
        ax.set_xlabel("Residual isotope")
        ax.set_ylabel(r"$\sigma$ [mb]")
        ax.set_title(titles[reaction])

        plt.tight_layout()
        prettify(ax)
        autoscale_fonts(fig)

        plt.savefig(f"{reaction}/{reaction}_yields.pdf")


if __name__ == "__main__":
    main()