Table table;
int minTime = Integer.MAX_VALUE;
int maxTime = Integer.MIN_VALUE;

void setup() {
  size(800,800);
    // data.csvを読み込み（ヘッダー行ありの場合はheaderを指定）
    table = loadTable("myData.csv", "header");
    printlnTable(); //データ確認用

  for (TableRow row : table.rows()) {
    int t = row.getInt("time");
    if (t < minTime) minTime = t;
    if (t > maxTime) maxTime = t;
  }
}

void draw(){
  background(0);
  for (int i = 0; i < table.getRowCount(); i++) {
    TableRow row = table.getRow(i);
    int time = row.getInt("time"); // 列名指定
    int distance = row.getInt("distance");  // 列名指定
    float x = map(time, minTime, maxTime, 0, width); // timeをx座標にマッピング
    float y = map(distance, 0, 400, height, 0); // distanceをy座標にマッピング（距離が大きいほど上に）
    fill(255);
    noStroke();
    ellipse(x, y, 5, 5); // データ点を描画
  }
}

void printlnTable() {
    println("行数: " + table.getRowCount());
    println("列数: " + table.getColumnCount());
    //行ごとにデータを確認
    for (TableRow row : table.rows()) {
        int time = row.getInt("time"); // 列名指定
        int distance = row.getInt("distance");  // 列名指定
        println(time + " : " + distance);
    }
}
