class EventVizualizer {
  String fileName;
  ArrayList<Integer> eventsData = new ArrayList<Integer>();
  int x, y, diameter;
  color c;
  
  EventVizualizer(String filename, int x, int y, int dia, color col) {
    this.fileName = filename;
    this.x = x;
    this.y = y;
    this.diameter = dia;
    this.c = col;
    Table t = loadTable(filename, "header");
    
    for (TableRow r : t.rows()) {
      String hms = r.getString("timestamp");// 例: "12:14:32"
      int s = hmsToSec(hms);//"HH:MM:SS"を秒に変換
      eventsData.add(s);//秒データをArrayListに追加
    }
  }
  
  void printArray() {
    for (int i = 0; i < eventsData.size(); i++) {
      println(i,":",eventsData.get(i));
    }
  }
  
  void vizualize() {
    fill(c);
    noStroke();
    ellipse(x, y, diameter * 2, diameter * 2);
  }
  
  void update(int currentSec) {
    // currentSecがeventsDataに含まれているかどうかで直径を変化
    if (eventsData.contains(currentSec)) {
      diameter += 1;
    }
  }
  
  //"HH:MM:SS"を秒に変換
  int hmsToSec(String hms) {
    if (hms == null || hms.length() < 7) return - 1;
    // "H:MM:SS" のような可変長にもゆるく対応
    String[] p = split(hms.trim(), ':');
    if (p == null || p.length < 2) return - 1;
    int hh = 0, mm = 0, ss = 0;
    try {
      hh = int(p[0]);
      mm = int(p[1]);
      ss = (p.length >= 3) ? int(p[2]) : 0;
    }
    catch(Exception e) {
      return - 1;
    }
    if (hh < 0 || hh > 23 || mm < 0 || mm > 59 || ss < 0 || ss > 59) return - 1;
    return hh * 3600 + mm * 60 + ss;
  }
  
  //秒を"HH:MM:SS"に変換
  String secToHms(int s) {
    //0〜86399（1日＝24h*60m*60s-1）の範囲に収める
    s = (s % 86400 + 86400) % 86400;  // マイナス値対応（念のため）
    int hh = s / 3600;
    int mm = (s % 3600) / 60;
    int ss = s % 60;
    return nf(hh, 2) + ":" + nf(mm, 2) + ":" + nf(ss, 2);
  }
    
} 
