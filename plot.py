from matplotlib import pyplot as plt
import pandas as pd
import os

def plot(files: list[str]):
    fig, ax = plt.subplots()
    markers = ['o', 's', 'd']

    for i, file in enumerate(files):
        type = (file.split('_')[-1]).split('.')[0]
        
        ber_df = pd.read_csv(file)
        ber_df = ber_df[ber_df['BER'] != 0]

        ber = ber_df.iloc[:, 1]
        parameter = ber_df.iloc[:, 0]

        ax.semilogy(parameter, ber, label=type, ls='--', marker=markers[i])

    ax.legend()
    ax.set_xlabel(ber_df.columns[0])
    ax.set_ylabel('BER')
    ax.set_xlim(left=0)
    # ax.set_xticks(range(0, 22, 2))
    ax.grid(which='both', axis='both')
    plt.show()

snr_files = [file for file in os.listdir() if ''.join(file.split('_')[:-1]) == 'bervssnr']
noise_var_files = [file for file in os.listdir() if ''.join(file.split('_')[:-1]) == 'bervsnoisevar']

plot(noise_var_files)
# plot(snr_files)
