Table table;
int dataCount = 0;

void setup() {
    size(800,800);
    // data.csvを読み込み（ヘッダー行ありの場合はheaderを指定）
    table = loadTable("m5.csv", "header");  
    println("行数: " + table.getRowCount());
    println("列数: " + table.getColumnCount());
    
    //行ごとにデータを確認
    for (TableRow row : table.rows()) {
        int time = row.getInt("time"); // 列名指定
        int distance = row.getInt("distance");  // 列名指定
        println(time + " : " + distance);
    } 
    textSize(20);
}

void draw() {
    background(0);
    TableRow row = table.getRow(dataCount);
    int time = row.getInt("time"); // 列名指定
    int distance = row.getInt("distance");  // 列名指定

    text("time: " + str(time), 10, 30);//時間表示
    distance = int(map(distance, 0, 70, 0, height));  //距離を画面サイズにマッピング
    fill(100, 255, 255); //四角形の色
    rect(width / 2 - 50, height, 100, -distance);  //距離を四角形の高さに反映
    
    dataCount++;//データカウントを進める
    if (dataCount >= table.getRowCount()) dataCount = 0; //最後まで行ったら最初に戻る
}
