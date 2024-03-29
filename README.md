# IsLibrary

Image Processing & Recognition Standard Library  

## 参考
+ NNabla https://github.com/sony/nnabla

## 背景
+ NdArrayベースの画像処理・認識周りのライブラリの需要があると感じている
+ NdArrayベースのC++ライブラリを使って様々なアプリケーションの作成を実現したい
+ 画像処理・認識系の各技術を実際に実装して理解する場とする

## 目的
+ PythonのNumpyライクな画像処理・認識をC++で実現する
+ CMakeの実装経験を積む
+ GPU Computing(OpenCL, Cuda, Metal)の実装経験を積む
+ テスト駆動開発の経験を積む


## ロードマップ
| マイルストン | 内容 | 達成状況 |
| :-- | :-- | :-- |
| 0合目 | NdArrayの仕組み理解, 実装(Nnabla参考) | ◯ |
| 1合目 | 画像ファイルI/O機能 | ◯ |
| 2合目 | フィルタ機能 | 実行中 |
| 3合目 | 画像スケーリング機能 | - |
| 3合目 | 動画ファイル, カメラからのスルー画像取り込み機能 | - |
| 4合目 | 2値化機能 | - |
| 5合目 | モルフォロジー機能 | - |
| 6合目 | ラベリング機能 | - |
| 7合目 | 形状特性解析機能 | - |
| 8合目 | 画像補間機能 | - |
| 9合目 | 座標変換機能 | - |
| 10合目 | 3DCV機能(回転, エピポーラ, ステレオ, など) | - |
| 11合目 | バンドル調整実装, カメラキャリブレーション機能 | - |
+ ....

## 機能要件
+ Windows用ライブラリ
+ macOS用ライブラリ
+ Linux用ライブラリ
+ Android用ライブラリ
+ iOS用ライブラリ
+ カメラからのスルー画像I/O機能
+ 動画ファイルI/O機能
+ フィルタ機能
+ 周波数解析機能
+ 2値化機能
+ 座標変換機能
+ 画像補間機能
+ 画像スケーリング機能
+ モルフォロジー機能
+ ラベリング機能
+ 形状特性解析機能
+ カメラキャリブレーション機能
+ 3DCV機能(回転, エピポーラ, ステレオ, など)
+ 特徴点マッチング機能
+ 物体トラッキング機能
+ ...

## 非機能要件
1. Cmake >= 3.16.0  
2. C++ >= 17
3. Compilers: GNU, MSVC, Clang, AppleClang
4. Eigen 3.4.0
5. GoogleTest 1.12.0
6. OpenCV 4.x
7. RapidJson x.x
