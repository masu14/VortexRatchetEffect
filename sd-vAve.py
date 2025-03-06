import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# CSVファイルを読み込む
dir = "output/Line-S2L2-S_is_Variable/MD050"
data = pd.read_csv(dir + "/velocity_averages.csv")

# y を固定
fixed_lorentz = 1.84  # 例: 固定したい site distance の値
filtered_data = data[data["lorentzForce"] == fixed_lorentz]

# lorentz force と Mean Velocity を抽出
sorted_data = filtered_data.sort_values("siteDistance")
siteDistance = sorted_data["siteDistance"]
mean_v = sorted_data["vAve"]

# Mean Velocity を正規化
v_min = mean_v.min()
v_max = mean_v.max()
v_abs_max = max(abs(v_min), abs(v_max))
mean_v = mean_v.values / v_abs_max

# グラフを描画
plt.figure(figsize=(10, 10))
plt.plot(
    mean_v,  # X軸を Mean Velocity に変更
    siteDistance,  # Y軸を siteDistance に変更
    linestyle="-",
    color="black",
    label=f"Lorentz Force = {fixed_lorentz}",
)

# 軸の設定
plt.tick_params(axis="both", direction="in")
plt.xticks(fontsize=28, ticks=[-1.0, -0.5, 0, 0.5, 1.0])  # X軸に Mean Velocity
plt.yticks(fontsize=28, ticks=[-2, -1, 0, 1, 2])  # Y軸に Dx

# 軸ラベルとタイトル
plt.xlabel("Mean Velocity", fontsize=30)  # X軸ラベル変更
plt.ylabel("Dx", fontsize=30)  # Y軸ラベル変更
plt.legend()

# グラフを保存または表示
plt.savefig(dir + f"/vAve-sdx_sd={fixed_lorentz}.png", dpi=300)
plt.show()
