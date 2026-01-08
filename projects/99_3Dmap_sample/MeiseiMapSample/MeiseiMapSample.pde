import de.fhpotsdam.unfolding.*;
import de.fhpotsdam.unfolding.geo.*;
import de.fhpotsdam.unfolding.utils.*;

import peasy.*; //PeasyCamライブラリをインストールしてインポート
PeasyCam cam;

UnfoldingMap map;
Location[] locations = {
  new Location(35.64542, 139.40818), // 噴水付近
  new Location(35.64578, 139.41030), // 33号館付近
  new Location(35.64410, 139.40918), // 29号館付近
  new Location(35.64340, 139.40807), // 野球場付近
  new Location(35.64419, 139.40647), // 資料図書館付近
  new Location(35.64457, 139.40730), // ソルブラン
};

void setup() {
  //size(800, 600, P2D);
  size(800, 600, P3D);
  
  cam = new PeasyCam(this, 1200);

  map = new UnfoldingMap(this);
  map.zoomAndPanTo(17, new Location(35.64435017198614, 139.40846229633914));
}

void draw() {
  background(0);
  map.draw();
  
  noStroke();
  for (int i = 0; i < locations.length; i++) {
    Location location = locations[i];
    ScreenPosition position = map.getScreenPosition(location);
    
    push();
    translate(position.x, position.y, 50);
    fill(0, 200, 0, 100);
    //ellipse(position.x, position.y, 20, 20);
    
    //box(20, 20, 100);
    sphere(50);
    pop();
  }
}
