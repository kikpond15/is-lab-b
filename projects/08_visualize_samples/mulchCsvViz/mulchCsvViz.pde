/*3つのcsvデータを読み込み可視化する。
csvには距離センサの値が閾値を超えた(センサの前に人がいた、通った)時の時刻が保存されている。
00:00:00の時刻を秒数に直し、時刻経過と共にセンサの前を通った人数を可視化する
*/

EventVizualizer[] eventsViz; // EventVizualizerクラスの配列
int currentSec, startSec, endSec;

void setup(){
  size(800,800);
  eventsViz = new EventVizualizer[3];
  eventsViz[0] = new  EventVizualizer("data00.csv", 200, 400, 50, color(255,0,0));
  eventsViz[1] = new  EventVizualizer("data01.csv", 400, 400, 50, color(0,255,0));
  eventsViz[2] = new  EventVizualizer("data02.csv", 600, 400, 50, color(0,0,255));
  
  startSec = eventsViz[0].hmsToSec("10:45:00");
  endSec = eventsViz[0].hmsToSec("13:00:00");
  currentSec = startSec;

  textSize(30);
}

void draw(){
  background(230);

  // 全てのEventVizualizerを描画・更新
  for(int i=0; i<eventsViz.length; i++){
    eventsViz[i].vizualize();
    eventsViz[i].update(currentSec);
  }

  String hms = eventsViz[0].secToHms(currentSec);
  fill(0);
  text(hms, 20,20);

  currentSec++;
  if(currentSec > endSec){
    //currentSec = startSec;
    noLoop();
  }
}

void keyPressed(){
  if(key == 'p'){
    eventsViz[0].printArray();
  }
}
