# Processingで地図を表示する

Processingで地図を表示して、
緯度経度を元に点をプロットするサンプルです。

![map](./map.png)

## 使用ライブラリ

### Unfolding

http://unfoldingmaps.org/

- Processingで地理情報の可視化をするためのライブラリ
- 最新版のProcessingには対応していない
    - Processing4に対応してくれている人がいたので使わせてもらう
    - https://github.com/tillnagel/unfolding/pull/119

## 実行手順

1. 以下のURLからProcessing4で実行可能なUnfoldingをダウンロードする
    - http://services.informatik.hs-mannheim.de/~nagel/GDV/Unfolding_for_processing_0.9.92.zip
2. ダウンロードしたZipを解凍して、 `Processing/libraries` に配置する
3. Processingで `MeiseiMapSample` を実行する

## 緯度経度情報をLocation配列を定義する文字列に変換するツール

スマートフォンのマップアプリでコピーアンドペーストした緯度経度情報を、
Processingで使うための文字列に変換するツールを用意しました。

以下のコマンドで標準出力されます。

```sh
python tools/ios2processing.py tools/sample_locations/ios.csv
# 出力結果
# Location[] locations = { new Location(35.64542, 139.40818), new Location(35.64578, 139.41030), new Location(35.64410, 139.40918), new Location(35.64340, 139.40807), new Location(35.64419, 139.40647), new Location(35.64457, 139.40730) };
```
