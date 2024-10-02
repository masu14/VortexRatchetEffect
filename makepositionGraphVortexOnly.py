import pandas as pd
import matplotlib.pyplot as plt
from matplotlib.patches import Circle
from matplotlib.animation import FuncAnimation

#csvファイルの読み込み
file_name = "output/20241002/006position.csv"

vortex_data = pd.read_csv(file_name, skiprows=2)

#ボルテックスの数を抽出
num_vortexs = (len(vortex_data.columns)-1)//2

#図と軸を作成
fig, ax = plt.subplots()
ax.set_xlim(0, 0.75)
ax.set_ylim(0, 0.5)

#分子をプロットするための初期設定
scatters = [ax.plot([], [], 'o')[0] for _ in range(num_vortexs)]

#アニメーション更新用の関数
def update(frame):
    for i in range(num_vortexs):
        scatters[i].set_data(vortex_data[f'x{i+1}'][frame], vortex_data[f'y{i+1}'][frame])
    return scatters

#アニメーションの作成
skip_step=20
ani = FuncAnimation(fig, update, frames=range(0,len(vortex_data),skip_step), interval=50, blit=True)

#アニメーションを保存または表示
ani.save('vortexOnly_animation.mp4', writer='ffmpeg')
plt.show()