# 日報  

## 日付  

### 作業内容詳細  

### その他  

## 20241121  

### 予定作業  

- 入力データ  
  - ボルテックス、ピニングサイトの初期位置を与える入力ファイルとその読み込み
- 出力データ
  - 時間とボルテックスの位置(t-x-y)
  - ローレンツ力とsite distanceと平均速度(f-d-v)
- ピニングサイトのインターフェース
  - 円形ピニングサイト
  - 矩形ピニングサイト
  - 線ピニングサイト

### 作業内容詳細  

#### 入力データ

オブジェクト(ボルテックス、ピニングサイト)の数はinput.inファイルで決める  
inputVortexPos.iniにボルテックスの初期座標を  
inputPinningSitePos.iniにピニングサイトの初期座標を書き込み、これをInputReaderクラスのメンバ関数で読み込む  
呼び出しはMDクラスのInit関数で行う  
InputReaderからMDへの可変長配列の受け渡しはstd::move()関数を使う  

サンプルコード  

```cpp
unique_ptr<Vortex[]> vortexs = std::make_unique<Vortex[]>(5);
for (int i = 0; i < 5; i++) {
  vortexs[i].SetPos(i, i);
}
unique_ptr<Vortex[]> vortexs2 = std::move(vortexs);
for (int i = 0; i < 5; i++) {
  std::cout << vortexs2[i].GetPos().x() << std::endl;
}
```

### 次への作業  

### その他  

## 20241127  

### 作業内容詳細  

#### 出力ファイル  

```markdown
├── output/  
│   ├── condition1/  
│   │   ├── MD_001/
│   │   │   ├── MD_var1=x_var2=y/
│   │   │   │   ├── position.csv
│   │   │   │   ├── velocity.csv
│   │   │   │   ├── force.csv
```

1回のシミュレーションでMD_00Xが作成され、input.iniの変数設定に応じてMD_var1=x_var2=yが作成される。  

ex.MD_lorentzForce=1.00_siteDistance=0.00

### その他  
