# colorPicker
OpenFrameWorksで制作した色抽出プログラムです。

-----------

## 使用方法
画面の表示の通りイメージファイルをDrag&Dropします。

### アルゴリズム

イメージの中で頻度を基準にしてHueグループを5つ取り出す
ーこのグループはTHRESHOLDで調整できる
各々のグループで一番多く使われたSaturation, Brightnessを取り出し、代表色を決める
5つの代表色をPalleteに入れ、順番に画面に表示する

### 改善点
代表色を決める際に不備がある
頻度を比較する時に整列してインデクスをチェックするのではなく、C++の関数を利用する方法を考えてみる必要がある
（頻度をチェックする配列を作ってmax_elementを使うなど）