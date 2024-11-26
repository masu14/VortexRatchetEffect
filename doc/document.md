# VoltexRatchetEffectシミュレーションドキュメント  

このドキュメントでは主にシミュレーションプログラムの設計について述べる。  

## プログラム設計・実装  

### InputReaderクラス  

#### ReadInputFile関数  

### FileHandlerクラス  

## ディレクトリ階層  

```markdown
VortexRatchetEffect/
├── source/
│   ├── main.cpp
│   ├── FileHandler.cpp
│   ├── InputReader.cpp
│   ├── MD.cpp
│   ├── PiningSite.cpp
│   └── Vortex.cpp
├── header/
│   ├── Eigen/
│   ├── FileHandler.h
│   ├── InputReader.h
│   ├── MD.h
│   ├── PiningSite.h
│   └── Vortex.h
├── project/
│   ├── VortexRatchetEffect.sln
│   ├── VortexRatchetEffect.vcxproj
│   ├── VortexRatchetEffect.filters
│   ├── VortexRatchetEffect.vcxproj.filters
│   ├── VortexRatchetEffect.user
│   └── output.csv
├── doc/
│   └── nippou.md
|   ├── document.md
|   ├── markdown記法.md
|   ├── markdown記法.pdf
├── input/
│   ├── input.ini
│   ├── pinningSite_Circle_data.txt
│   ├── pinningSite_Square_data.txt
│   └── vortex_data.txt  
├── output/  
│   ├── condition1/  
│   │   ├── MD_001/
│   │   │   ├── MD_var1=x_var2=y/
│   │   │   │   ├── position.csv
│   │   │   │   ├── velocity.csv
│   │   │   │   ├── force.csv
│   │   │   │   ├── lorentzForce-velocity.csv
│   │   │   │   ├── lorentzForce-siteDistance-velocity.csv
│   └── 
```
