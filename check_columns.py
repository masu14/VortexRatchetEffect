import pandas as pd

#csvファイルの読み込み
file_name = "output/20241002/002position.csv"
circle_data = pd.read_csv(file_name, nrows=1)
vortex_data = pd.read_csv(file_name, skiprows=2)

# 列名の表示
print(circle_data.columns)
print(vortex_data.columns)