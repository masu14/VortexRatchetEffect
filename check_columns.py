import pandas as pd

# CSVファイルの読み込み
data = pd.read_csv("output/20240930/002position.csv")

# 列名の表示
print(data.columns)
