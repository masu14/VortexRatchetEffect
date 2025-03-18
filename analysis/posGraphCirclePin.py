import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.patches import Circle
from matplotlib.animation import FuncAnimation

# csvファイルの読み込み
dir_name = "analysis/../output/Circle-S2L2-S_is_Variable/MD010/MD_lorentzForce=1.90_siteDistance=-1.50"
file_name = dir_name + "\\position.csv"
circle_data = pd.read_csv(file_name, nrows=1)
vortex_data = pd.read_csv(file_name, skiprows=2)

# 円の数を取得
num_circles = len(circle_data.columns) // 3

# 円の初期座標と半径を取得
circle_positions = [
    (circle_data[f"x{i+1}"][0], circle_data[f"y{i+1}"][0], circle_data[f"r{i+1}"][0])
    for i in range(num_circles)
]

# ボルテックスの数を抽出
num_vortexs = (len(vortex_data.columns) - 1) // 2

# 図と軸を作成
fig, ax = plt.subplots()
ax.set_xlim(0, 16)
ax.set_ylim(0, 12)

# 円を描画
circles = [Circle((x, y), r, fill=False) for x, y, r in circle_positions]
for circle in circles:
    ax.add_patch(circle)

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

# アニメーションを保存または表示
ani.save(dir_name + "\\vortex_animation.mp4", writer="ffmpeg")
plt.show()
