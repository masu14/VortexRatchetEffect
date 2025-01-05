import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.colors import LinearSegmentedColormap

# CSVファイルを読み込む
dir = "output\Line-S2L2-S_is_Variable\MD060"
data = pd.read_csv(dir + "\\velocity_averages.csv")

# データをピボットテーブル形式に変換
pivot_table = data.pivot(index="siteDistance", columns="lorentzForce", values="vAve")

# 正規化処理
v_min = pivot_table.values.min()
v_max = pivot_table.values.max()
v_abs_max = max(abs(v_max), abs(v_min))

# normalized_values = 0.6 * (pivot_table.values - v_min) / (v_max - v_min) - 0.3
normalized_values = pivot_table.values / v_abs_max


# カラーマップの定義(-1:青, 0:緑, 1:赤)
colors = [
    (0, 0, 1),  # 青(-1)
    (0, 1, 1),  # シアン
    (0, 1, 0),  # 緑(0)
    (1, 1, 0),  # 黄色
    (1, 0, 0),  # 赤(1)
]

custom_cmap = LinearSegmentedColormap.from_list("custom_jet", colors)

# グラフを描画
plt.figure(figsize=(10, 8))
c = plt.pcolormesh(
    pivot_table.columns,
    pivot_table.index,
    normalized_values,
    shading="auto",
    cmap=custom_cmap,
    vmin=-1,
    vmax=1,
)
plt.colorbar(c, label="Average Velocity")

# 軸ラベルとタイトル
plt.xlabel("lorentzForce")
plt.ylabel("siteDistance")
plt.title("Average Velocity (lorentzForce vs siteDistance)")

# グラフを保存または表示
plt.savefig(dir + "\\velocity_heatmap.png", dpi=300)
plt.show()
