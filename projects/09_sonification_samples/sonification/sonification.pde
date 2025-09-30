import processing.sound.*;
SinOsc osc;
Table table;
int dataCount = 0;

void setup() {
    size(800,800);
    // data.csvを読み込み（ヘッダー行ありの場合はheaderを指定）
    table = loadTable("m5_heartbeat.csv", "header");  
    printlnTable(); //データ確認用
    osc = new SinOsc(this);
    osc.amp(0.5); //音量
    osc.play();
    textSize(30);
}

void draw() {
    background(0);
    TableRow row = table.getRow(dataCount);
    int time = row.getInt("time"); // 列名指定
    int heartbeat = row.getInt("heartbeat");  // 列名指定
    text("time: " + str(time), 10, 30);//時間表示
    
    
    float value = constrain(map(heartbeat, 1000, 2500, 0, 1), 0, 1);
    float freq = map(value, 0, 1, 220, 1760);
    osc.freq(freq);
    // 音量も少し連動させたいときosc.amp(map(value, 0, 1, 0.05, 0.4));

    dataCount++;//データカウントを進める
    if (dataCount >= table.getRowCount()) dataCount = 0; //最後まで行ったら最初に戻る
}

void printlnTable() {
    println("行数: " + table.getRowCount());
    println("列数: " + table.getColumnCount());
    //行ごとにデータを確認
    for (TableRow row : table.rows()) {
        int time = row.getInt("time"); // 列名指定
        int distance = row.getInt("heartbeat");  // 列名指定
        println(time + " : " + distance);
    } 
}
