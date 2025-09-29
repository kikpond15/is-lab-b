Table table;

void setup() {
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
}
