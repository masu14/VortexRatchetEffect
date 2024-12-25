import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation

# csvファイルの読み込み
dir_name = "output\Line-S2L2-S_is_Variable\MD033\MD_lorentzForce=1.95_siteDistance=2.00"
file_name = dir_name + "\\position.csv"
line_data = pd.read_csv(file_name, nrows=1)
vortex_data = pd.read_csv(file_name, skiprows=2)

# 線の数を取得
num_lines = len(line_data.columns) // 3

# 線の初期座標と長さを取得
line_positions = [
    (line_data[f"x{i+1}"][0], line_data[f"y{i+1}"][0], line_data[f"l{i+1}"][0])
    for i in range(num_lines)
]

# ボルテックスの数を抽出
num_vortexs = (len(vortex_data.columns) - 1) // 2

# 図と軸を作成
fig, ax = plt.subplots()
ax.set_xlim(0, 16)
ax.set_ylim(0, 4)
ax.set_aspect("equal")

# 線を描画
lines = []
for x, y, l in line_positions:
    # 始点と終点を計算
    x_start = x - l / 2.0
    x_end = x + l / 2.0
    (line,) = ax.plot([x_start, x_end], [y, y], "k-")  # 黒い線
    lines.append(line)

# vortexをプロットするための初期設定
scatters = [ax.plot([], [], "o")[0] for _ in range(num_vortexs)]


# アニメーション更新用の関数
def update(frame):
    for i in range(num_vortexs):
        scatters[i].set_data(
            vortex_data[f"x{i+1}"][frame], vortex_data[f"y{i+1}"][frame]
        )
    return scatters


# アニメーションの作成
skip_step = 40
ani = FuncAnimation(
    fig, update, frames=range(0, len(vortex_data), skip_step), interval=50, blit=True
)

# アニメーションの保存、表示
ani.save(dir_name + "\\vortex_animation.mp4", writer="ffmpeg")
plt.show()
