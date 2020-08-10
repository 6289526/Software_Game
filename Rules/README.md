# 細かな命名規則等のルールを記載する

## ファイルの命名規則
- 小文字
- 単語の間は`_`
  - Ex ) sample_file_name.cpp


## ソースファイル
### 拡張子
- `cpp`


## Headerファイル
### 拡張子
- 基本は`h`
- クラス等を使うならそれ単体を`hpp`

### インクルードガード
- `#pragma once`で行う
  - Ex ) 
    ```
    #pragma once

    // ifndef INCLUDE
    // define INCLUDE
    // endif
    ```

## 変数、関数の命名規則
### 変数
基本的に単語の頭は大文字、残りは小文字  
変数名での判断は最初の単語の頭を見て行う
- グローバル変数    : 単語の頭大文字あと小文字
- public変数    : 単語の頭大文字あと小文字
- private, protected変数    : 変数名の頭に`_`
- local変数 : 最初の頭小文字
  - Ex ) 
    ```
    int PubicIntNumber; // グローバル、public変数
    int _PrivateIntNumber; // privaate変数
    int localIntNumber; // local変数
    ```
### 関数  
すべての関数は単語の頭大文字あと小文字
- Ex ) `void PublicAndPrivateFunction(){ }`
### 構造体
- 単語ごと頭大文字あと小文字
  - Ex ) `enum SampleEnumType{ }` 
