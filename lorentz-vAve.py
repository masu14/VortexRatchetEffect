import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.ticker import MaxNLocator

# CSVファイルを読み込む
dir = "output/Line-S2L2-S_is_Variable/MD066"
data = pd.read_csv(dir + "/velocity_averages.csv")

# y を固定
fixed_sd = -1.5  # 例: 固定したい site distance の値
filtered_data = data[data["siteDistance"] == fixed_sd]

# lorentz force と Mean Velocity を抽出
lorentzForce = filtered_data["lorentzForce"]
mean_v = filtered_data["vAve"]

# lorentz force を pinning force で割る
pinForce = 2.0
lorentzForce = lorentzForce.values / pinForce

# Mean Velocity を正規化
v_min = mean_v.min()
v_max = mean_v.max()
v_abs_max = max(abs(v_min), abs(v_max))
mean_v = mean_v.values / v_abs_max

# グラフを描画
plt.figure(figsize=(6, 6))
plt.plot(
    lorentzForce,
    mean_v,
    linestyle="-",
    color="black",
    label=f"site distance = {fixed_sd}",
)

# 軸の設定
plt.tick_params(axis="both", direction="in")
major_locator = MaxNLocator(nbins=5)

# 軸ラベルとタイトル
plt.xlabel("Lorentz Force / Pinning Force")
plt.ylabel("Mean Velocity")
plt.legend()

# グラフを保存または表示
plt.savefig(dir + f"/lorentz-vAve_sd={fixed_sd}.png", dpi=300)
plt.show()
