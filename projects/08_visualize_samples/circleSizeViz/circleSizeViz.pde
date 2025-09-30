Table table;
int dataCount = 0;

void setup() {
    size(800,800);
    table = loadTable("m5.csv", "header");
    printlnTable(); //データ確認用
    textSize(20);
}

void draw() {
    background(0);
    TableRow row = table.getRow(dataCount);
    int time = row.getInt("time"); // 列名指定
    int distance = row.getInt("distance");  // 列名指定
    text("time: " + str(time), 10, 30);//時間表示
    distance = int(map(distance, 0, 70, 0, height));  //距離を画面サイズにマッピング
    
    //colorMode(HSB, 360,255,255); // 色相環を使う
    //int hue = int(map(distance, 0, height, 0, 360)); //距離を色相にマッピング
    //fill(hue, 255, 255);

    fill(100, 255, 255);
    ellipse(width/2, height/2, distance, distance); //距離を円の直径に反映
    
    dataCount++;//データカウントを進める
    if (dataCount >= table.getRowCount()) dataCount = 0; //最後まで行ったら最初に戻る
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