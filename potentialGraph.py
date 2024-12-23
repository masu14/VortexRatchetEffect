import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# CSVファイルを読み込む
dir = "output/Line-S2L2-S_is_Variable/MD030"
data = pd.read_csv(dir + "/PinningPotential.csv")

# y を固定
fixed_y = 0.5  # 例: 固定したい y の値
filtered_data = data[data["y"] == fixed_y]

# x と E を抽出
x = filtered_data["x"]
E = filtered_data["E"]

# グラフを描画
plt.figure(figsize=(8, 6))
plt.plot(x, E, linestyle="-", color="black", label=f"y = {fixed_y}")

# 軸ラベルとタイトル
plt.xlabel("x")
plt.ylabel("Pinning Potential (E)")
plt.title(f"Pinning Potential vs x (y = {fixed_y})")
plt.legend()

# グラフを保存または表示
plt.savefig(dir + f"/PinningPotential_y{fixed_y}.png", dpi=300)
plt.show()
