"""
ENSDF Gamma Contribution Calculator
====================================
Parses ENSDF adopted-levels data, builds a decay network, and for a
user-specified gamma-ray energy computes the percentage contribution
of every excited state to the total number of those gamma rays emitted.

Written by Thomas Chillery.

ENSDF column layout (0-indexed):
  [0:5]  NUCID  e.g. ' 26MG'
  [5]    continuation/modifier: ' '=primary, else skip
  [6]    sub-modifier: ' '=data, 'C'=comment -> skip comments
  [7]    record type: 'L'=level, 'G'=gamma
  [9:19] energy field (keV)
  [21:29] intensity / spin-parity field

Usage:
    python ensdf_gamma_calc.py <ensdf_file> <E_gamma_keV> [isotope] [tolerance_keV] [snap_tolerance_keV]

    isotope           : NUCID string as it appears in the ENSDF file, without
                        leading space (default: 26MG)
    tolerance_keV     : gamma-energy matching window in keV (default: 2.0)
    snap_tolerance_keV: max distance to snap computed to_E to a known level (default: 3.0)

Example:
    python ensdf_gamma_calc.py mg26.txt 1003.25
    python ensdf_gamma_calc.py mg26.txt 1003.25 26MG 1.0 3.0
"""

import sys
import re
from collections import defaultdict


# ─────────────────────────────────────────────────────────────────────────────
# 1.  ENSDF PARSER
# ─────────────────────────────────────────────────────────────────────────────

def clean_number(s):
    """Return float from a field that may contain trailing uncertainty digits."""
    s = s.strip()
    if not s:
        return None
    tok = s.split()[0]
    cleaned = re.sub(r'[^0-9.]', '', tok)
    try:
        return float(cleaned)
    except ValueError:
        return None


def parse_ensdf(text, isotope='26MG'):
    """
    Parse ENSDF text.  Only primary L and G records are read.
    Primary record criteria:
      col[5] == ' '   (not continuation/xref/branching)
      col[6] == ' '   (not a comment)
      col[7] in ('L', 'G')

    Returns
    -------
    levels : dict { energy_keV (float) : {'spin': str} }
    gammas : list of dicts { 'from_E', 'gamma_E', 'RI' }
             NOTE: to_E is NOT stored here; it is resolved later by snapping
             to the nearest known level energy (see snap_gamma_endpoints).
    """
    levels          = {}
    gammas          = []
    current_level_E = None

    target_nucid = isotope.upper().strip()

    for raw_line in text.splitlines():
        line = raw_line.rstrip()

        if len(line) < 8:
            continue

        nucid    = line[0:5]
        mod1     = line[5]   # ' '=primary, 'X'=xref, 'B'=branching, digit=continuation
        mod2     = line[6]   # ' '=data, 'C'=comment
        rec_type = line[7]   # 'L' or 'G'

        if nucid.strip() != target_nucid:
            continue
        if mod1 != ' ':
            continue
        if mod2 != ' ':
            continue
        if rec_type not in ('L', 'G'):
            continue

        if rec_type == 'L':
            e_val = clean_number(line[9:19]) if len(line) > 9 else None
            if e_val is None:
                current_level_E = None
                continue
            current_level_E = e_val
            spin_str = line[21:39].strip() if len(line) > 21 else ''
            levels[current_level_E] = {'spin': spin_str}

        elif rec_type == 'G' and current_level_E is not None:
            gamma_E = clean_number(line[9:19]) if len(line) > 9 else None
            if gamma_E is None:
                continue
            ri_str = line[21:29].strip() if len(line) > 21 else ''
            ri = clean_number(ri_str)
            if ri is None:
                ri = 0.0
            gammas.append({
                'from_E':  current_level_E,
                'gamma_E': gamma_E,
                'RI':      ri,
            })

    return levels, gammas


def snap_gamma_endpoints(gammas, levels, snap_tol=3.0):
    """
    Resolve each gamma's final-state energy by:
      1. Computing to_E = from_E - gamma_E
      2. Finding the nearest known level within snap_tol keV
      3. If found, set to_E to that level's energy (eliminates rounding errors)
      4. If not found within tolerance, keep the raw computed to_E
         (it may still be useful for ground-state transitions etc.)

    Returns updated gamma list with 'to_E' field added.
    """
    level_energies = sorted(levels.keys())

    def nearest_level(e):
        best = min(level_energies, key=lambda l: abs(l - e))
        if abs(best - e) <= snap_tol:
            return best
        return None

    result = []
    for g in gammas:
        raw_to = g['from_E'] - g['gamma_E']
        snapped = nearest_level(raw_to)
        g2 = dict(g)
        g2['to_E']       = snapped if snapped is not None else round(raw_to, 2)
        g2['to_E_snapped'] = snapped is not None
        result.append(g2)
    return result


# ─────────────────────────────────────────────────────────────────────────────
# 2.  BUILD DECAY NETWORK
# ─────────────────────────────────────────────────────────────────────────────

def build_network(gammas):
    """
    Group gammas by parent level; normalise relative intensities → branching ratios.

    Returns
    -------
    branches : dict { from_E : [ {'to_E', 'gamma_E', 'RI', 'BR'}, … ] }
    """
    by_level = defaultdict(list)
    for g in gammas:
        by_level[g['from_E']].append(g)

    branches = {}
    for lvl_E, gs in by_level.items():
        total_RI = sum(g['RI'] for g in gs)
        if total_RI <= 0:
            total_RI = 1.0
        branches[lvl_E] = [{**g, 'BR': g['RI'] / total_RI} for g in gs]

    return branches


# ─────────────────────────────────────────────────────────────────────────────
# 3.  FIND TARGET TRANSITION
# ─────────────────────────────────────────────────────────────────────────────

def find_transition(gammas, target_E, tol=2.0):
    best, best_diff = None, tol + 1.0
    for g in gammas:
        diff = abs(g['gamma_E'] - target_E)
        if diff < best_diff:
            best_diff = diff
            best = g
    if best is not None and best_diff <= tol:
        return best['from_E'], best['to_E'], best['gamma_E']
    return None


# ─────────────────────────────────────────────────────────────────────────────
# 4.  CONTRIBUTION CALCULATION  (memoised recursion)
# ─────────────────────────────────────────────────────────────────────────────

def compute_contributions(branches, target_from, target_to, target_gamma_E, tol=2.0):
    """
    P(level) = probability that one de-excitation of that level ultimately
               produces the target gamma ray.

    P(target_from) = BR of target gamma at that level
    P(other)       = sum over outgoing gammas g:
                       g.BR                    if g IS the target gamma
                       g.BR * P(g.to_E)        if g.to_E >= target_from
                       0                       otherwise
    """
    source_BR = 0.0
    for g in branches.get(target_from, []):
        if abs(g['gamma_E'] - target_gamma_E) <= tol:
            source_BR = g['BR']
            break

    memo = {}

    def prob(E):
        if E in memo:
            return memo[E]
        if abs(E - target_from) < 0.5:
            memo[E] = source_BR
            return source_BR
        total = 0.0
        for g in branches.get(E, []):
            if abs(g['gamma_E'] - target_gamma_E) <= tol:
                total += g['BR']
            elif g['to_E'] >= target_from - 0.5:
                total += g['BR'] * prob(g['to_E'])
        memo[E] = total
        return total

    feeding_levels = sorted(E for E in branches if E >= target_from - 0.5)
    results = [{'level_E': E, 'prob': prob(E)} for E in feeding_levels]
    return results, source_BR


# ─────────────────────────────────────────────────────────────────────────────
# 5.  MAIN
# ─────────────────────────────────────────────────────────────────────────────

def main():
    if len(sys.argv) < 3:
        print(__doc__)
        sys.exit(1)

    ensdf_file = sys.argv[1]
    target_E   = float(sys.argv[2])
    isotope    = sys.argv[3]          if len(sys.argv) > 3 else '26MG'
    tol        = float(sys.argv[4])   if len(sys.argv) > 4 else 2.0
    snap_tol   = float(sys.argv[5])   if len(sys.argv) > 5 else 3.0

    with open(ensdf_file, 'r') as f:
        text = f.read()

    levels, gammas = parse_ensdf(text, isotope)
    print(f"Parsed {len(levels)} levels and {len(gammas)} gamma records.")

    if not levels:
        nucid = isotope.upper().rjust(5)
        print(f"ERROR: No levels found. Check NUCID '{nucid}' exists in the file.")
        sys.exit(1)

    gammas  = snap_gamma_endpoints(gammas, levels, snap_tol)
    n_snapped = sum(1 for g in gammas if g['to_E_snapped'])
    print(f"Snapped {n_snapped}/{len(gammas)} gamma endpoints to nearest level "
          f"(within ±{snap_tol} keV).")

    branches = build_network(gammas)

    result = find_transition(gammas, target_E, tol)
    if result is None:
        print(f"\nNo gamma found within ±{tol} keV of {target_E} keV.")
        sys.exit(1)

    from_E, to_E, matched_E = result
    print(f"\nTarget gamma : {matched_E:.2f} keV")
    print(f"Transition   : {from_E:.2f} keV  →  {to_E:.2f} keV")
    print(f"Tolerance    : ±{tol} keV\n")

    contributions, source_BR = compute_contributions(
        branches, from_E, to_E, matched_E, tol
    )

    total_prob = sum(r['prob'] for r in contributions)
    if total_prob == 0:
        print("No feeding paths found.")
        sys.exit(1)

    contributions.sort(key=lambda r: r['level_E'])
    spin_map = {E: levels[E]['spin'] for E in levels}

    # ── Write CSV ─────────────────────────────────────────────────────────
    import csv
    csv_name = f"gamma_{matched_E:.2f}keV_{isotope}.csv"
    rows = []
    for r in contributions:
        E   = r['level_E']
        p   = r['prob'] * 100.0
        pct = p / total_prob
        if pct < 0.001:
            continue
        spin = spin_map.get(E, '') or '?'
        rows.append([f"{E:.2f}", spin, f"{p:.6f}", f"{pct:.3f}"])

    with open(csv_name, 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(["Level (keV)", "Jpi", "P(level->gamma)", "Contribution (%)"])
        writer.writerows(rows)

    print(f"Results written to: {csv_name}")

    # ── Terminal summary ──────────────────────────────────────────────────
    print(
        f"\n  P(level->gamma) = probability that one de-excitation of that level\n"
        f"                    ultimately produces the {matched_E:.2f} keV gamma.\n"
        f"  Contribution    = P(level) normalised over all levels shown\n"
        f"                    (assumes equal initial population per level).\n"
        f"\n  Source level ({from_E:.2f} keV) direct BR for {matched_E:.2f} keV gamma: "
        f"{100*source_BR:.2f}%"
    )


if __name__ == "__main__":
    main()