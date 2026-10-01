import de.fhpotsdam.unfolding.*;
import de.fhpotsdam.unfolding.geo.*;
import de.fhpotsdam.unfolding.utils.*;
import de.fhpotsdam.unfolding.providers.Microsoft;
import de.fhpotsdam.unfolding.providers.Google;
import de.fhpotsdam.unfolding.providers.OpenStreetMap;
import de.fhpotsdam.unfolding.providers.MapBox;

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
  size(800, 600, P2D);
  
  // ここで地図の種類を切り替えられる
  //map = new UnfoldingMap(this, new Microsoft.RoadProvider());
  //map = new UnfoldingMap(this, new Microsoft.HybridProvider());
  //map = new UnfoldingMap(this, new Microsoft.AerialProvider());
  map = new UnfoldingMap(this, new Google.GoogleMapProvider());
  //map = new UnfoldingMap(this, new Google.GoogleSimplified2Provider());
  //map = new UnfoldingMap(this, new Google.GoogleSimplifiedProvider());
  //map = new UnfoldingMap(this, new Google.GoogleTerrainProvider());
  
  map.zoomAndPanTo(17, new Location(35.64435017198614, 139.40846229633914));
}

void draw() {
  map.draw();
  
  noStroke();
  for (int i = 0; i < locations.length; i++) {
    Location location = locations[i];
    ScreenPosition position = map.getScreenPosition(location);
    fill(0, 200, 0, 100);
    ellipse(position.x, position.y, 20, 20);
  }
}
