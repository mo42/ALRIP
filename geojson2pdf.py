#!/usr/bin/env python

import sys

import geopandas as gpd
import matplotlib.pyplot as plt

def render_geojson_to_pdf(geojson_path, pdf_output_path):
    gdf = gpd.read_file(geojson_path)
    fig, ax = plt.subplots()
    gdf.plot(ax=ax)
    plt.savefig(pdf_output_path, bbox_inches='tight')
    plt.close()

if __name__ == "__main__":
    if len(sys.argv) != 3:
        sys.exit(f'Usage: {sys.argv[0]} infile outfile')
    render_geojson_to_pdf(sys.argv[1], sys.argv[2])
