import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("C:/dev/VoltexRatchetEffect_prottype/output/20240901/007position.csv")

initial_points = df[df['time']==0]

plt.figure(figsize=(10,8))

for i in range(1,13):
    plt.plot(df[f'x{i}'],df[f'y{i}'], label= f'組 {i}')
    plt.scatter(initial_points[f'x{i}'], initial_points[f'y{i}'], color='red', s=100, label=f'組 {i} 最初の位置' if i==1 else "")
    
plt.xlabel('X位置')
plt.ylabel('Y位置')
plt.title('各組の位置の時間変化')
plt.legend()
plt.grid(True)
plt.show()