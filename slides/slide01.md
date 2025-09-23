---
marp: true
theme: gaia
paginate: true
style: |
  /* 全体を縮小 */
  section { font-size: 2.3em; }

  /* 見出しが大きすぎるなら個別に下げる */
  section h1 { font-size: 2.7em; }
  section h2 { font-size: 1.35em; }
  section h3 { font-size: 1.1em; }

  /* 箇条書きの行間が詰まる場合の微調整（任意） */
  section ul, section ol { line-height: 1.2; }
---
<!-- _class: lead -->
2025年後期

# 情報学実験B
## 実世界センシング＆ビジュアライゼーション

フィジカルコンピューティングチーム
担当教員：菊池、遠藤


---
目次


- 略語
    - Processing = P5
    - M5Stack, M5Stack Core2 = M5
    - VS Code = VSC
    - PlatformIO = PIO


---
<!-- _class: lead -->
### 実世界センシング＆ビジュアライゼーション
# イントロダクション


---
## 本実験の概要
実世界における空間や事象、人やものなど、普段何気なく認識知覚している物事をセンサを用いて観測することで、環境や現象をデータとして再認識する。

そして、観測したデータをビジュアライズ(可視化)することで、
実世界の空間や事象をコンピュータの中(仮想世界)で再構築する。

---
## 実世界をセンシングするとは
<br>

### 実世界　＝　現実として存在する世界、我々が生活する空間
<br>

### センシング　＝ センサを使って温度、振動、画像、生体情報などの物理的・化学的・生物的特性を検出し、定量的な情報として取得する技術

---
## 実世界で観測できる情報(一部)
<br>
<img src="img/ss_img 2025-09-23 14.32.38.png" width=1200>


---
## IoT・スマートセンシング
<img src="img/IoToverview-e1439545614802.png" width=920>

IoTNEWS：https://iotnews.jp/smart-city/1218/

---
## デジタルツイン・サイバーフィジカルシステム
<img src="img/fig-digitaltwin1-01.jpg" width=920>

村田製作所：https://article.murata.com/ja-jp/article/digital-twin-that-recreates-twins

---
## ビジュアライゼーションとは
ビジュアライゼーションとは、人間が直接「見る」ことのできない現象・事象・関係性を「見る」ことを可能にする(可視化) こと。視覚化・可視化情報化・視覚情報化ともいう。


---
## ビジュアライゼーションの例1
[Takram. 東京の未来を考えるための人口推移データの可視化](https://www.takram.com/ja/projects/tokyo-urban-studies)

<img src="img/image01.png" width=900>

---
## ビジュアライゼーションの例2
[Yukari Nagata. Twitter Analysis : つぶやきの分布](https://public.tableau.com/app/profile/yukari.nagata0623/viz/TwitterAnalysis_15595641262440/sheet0)
<img src="img/image02.png" width=750>


---
## ビジュアライゼーションの例3
[CCBT:
Future Ideations Camp Vol.4「生態系をデータとしてとらえる／表現する」](https://ccbt.rekibun.or.jp/events/camp04__exhibition)
<img src="img/image03.png" width=800>


---
<!-- _class: lead -->
### 実世界センシング＆ビジュアライゼーション
# 実験概要


---
## 実験詳細
- ESP32マイコンの環境構築&プログラミング(C言語)について
- 各種センサの使用方法(プログラミング&回路作成)
- センサデータの記録保存方法
- センサデータのグラフプロット
- データの可視化方法＆プログラミング(Java)
- 環境データの可視化＆生体データの可視化
- ビジュアライズ作品のアイディア出し＆センサデータ選定
- データ収集フィールドワーク
- ビジュアライゼーション作品の制作
- 実験レポート作成

---
## 実験スケジュール
- 1週目：導入＆実験準備
    - 実験概要説明＆環境構築
    - M5のサンプルコード動作確認
    - センサーのサンプルコードテスト
- 2週目：データ保存＆ビジュアライゼーション手法
    - SDへのデータ保存＆センサデータのSD保存
    - ビジュアライゼーション手法＆サンプル

- 3週目：ビジュアライゼーションのアイディア出し
    - アイディア出し
    - アイディアチェック＆修正

---
- 4週目：センシングデバイスのプロトタイプ実装
    - M5センサの実装＆データ保存の確認
    - 可視化プロトタイプ実装＆ダミーデータテスト
- 5週目：実世界フィールドワーク 
    - 学内でデータ収集実験
    - データの確認＆追加実験
- 6週目：ビジュアライゼーション制作
    - ビジュアライゼーション制作1
    - ビジュアライゼーション制作2

- 休み期間：(学祭 or 冬休み)

- 7週目：予備日(追実験＆レポート作成)
    - レポート作成

---
## 学内フィールドワーク

<img src="img/ss_img 2025-09-23 19.14.49.png" width=1200>


---
## 実験機材
- [ESP32マイコン：M5Stack Core2](https://docs.m5stack.com/ja/core/core2)
- 各種センサ

<img src="https://static-cdn.m5stack.com/resource/docs/products/core/core2/core2_01.webp" width=400>



---
## 開発環境
[Platform IO](https://platformio.org/) in [VS Code](https://code.visualstudio.com/)

[Processing](https://processing.org/) Java or p5.js

<br>

<img src="img/platformIO.jpg" width=300> <img src="img/Processing_2021_logo.svg" width=300>



---
## 実験を行う前の注意点
- 実験機材は最新の注意を払って扱うこと
- 電子機器、回路を扱うので、水分が付いた手で扱わないこと(感電します)
- PCの上でマイコン、電子機器を扱わないこと(ショートします)

<br>

- ChatGPTや生成AIの使用について
    - コードの実装には使っていい
    - レポートには使っちゃダメ
    - 生成AIはコードが脱線するから1から作り直しになることもある
    - 生成AIより教員とSA/TAを頼って


---
## レポートについて
- 各回の実験内容をレポートにまとめる
- 実験環境や実験機材など細かく記載する
- 実験の様子など正確に撮影し、レポートに図として取り入れる
- 制作したビジュアライゼーション作品についてレポートでまとめる
(コンセプト、実装-方法、実験環境、ビジュアライズの方法など)


- レポート作成時に、実験内容を思い出せるように、
ノートを用意し各回メモを取ること。

---
<!-- _class: lead -->
# 教員&TASAの紹介
<br>

## 教員の紹介
#### 担当教員：菊池康太
#### 非常勤講師：遠藤勝也

<br>

## TA・SAの紹介

#### TA：大学院修士1年　岸野さん
#### SA：学部3年　太田さん


---
<!-- _class: lead -->
### 実世界センシング＆ビジュアライゼーション
# 環境構築


---
## PlatformIO の環境構築

VS Codeを開いて、左メニューの①Extensions(拡張機能)から、②PlatformIOを検索し、③選択し「インストール」をクリックする。
<img src="img/ss_img 2025-09-23 19.32.54.png" width=1000> 

---
インストールが完了すると、左メニューにPIOのロゴが出現する。
これでインストールはOK
<img src="img/ss_img 2025-09-23 19.42.36.png" width=1000> 

---
①PIOのロゴをクリックし、②”Create New Project”、③”New Project”から新しいプロジェクトを作成する。

<img src="img/ss_img 2025-09-23 20.04.39.png" width=1200> 

---
Name: **m5-test**
Board: **M5Stack Core ESP32**
Framework: **Arduino**
Location☑️から保存先をデフォルトか任意のディレクトリに変更できる。
デスクトップに「**meisei-exb2025**」フォルダを作成し、そこに保存しましょう。
<img src="img/ss_img 2025-09-23 20.48.52.png" width=700>
設定できたら「Finish」をクリック


---
#### プロジェクト作成の画面

<img src="img/ss_img 2025-09-23 20.55.34.png" width=1100>

---
#### ディレクトリ構成
m5-test/
├── platformio.ini（**libraryなどの設定ファイル**）
├── src/
│   └── main.cpp（**メインとなるプログラム**）
├── include/
│   └── README
├── lib/
│   └── README
├── test/
│   └── README
├── .vscode/
│   └── （VSCode設定ファイル群）
├── .pio/
│   └── （PlatformIOビルド成果物）

---


---


---


---


---



## 

---
## 
---
## 

---
## 
---
## 

---
## 

