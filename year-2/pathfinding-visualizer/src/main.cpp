#include "Algorithms.hpp"
#include "Grid.hpp"

#include <SFML/Graphics.hpp>

#include <vector>

static sf::Color ColorFor(CellType t) {
  switch (t) {
    case CellType::Empty:
      return sf::Color(18, 25, 45);
    case CellType::Wall:
      return sf::Color(10, 10, 12);
    case CellType::Start:
      return sf::Color(93, 214, 255);
    case CellType::End:
      return sf::Color(157, 255, 144);
    case CellType::Visited:
      return sf::Color(160, 120, 255);
    case CellType::Path:
      return sf::Color(255, 220, 100);
    default:
      return sf::Color::White;
  }
}

struct Animation {
  std::vector<int> visit;
  std::vector<int> path;
  size_t visit_i{0};
  size_t path_i{0};
  bool showing_path{false};
  bool running{false};
};

static void StartAnimation(Animation& a, const PathResult& r) {
  a.visit = r.visit_order;
  a.path = r.path;
  a.visit_i = 0;
  a.path_i = 0;
  a.showing_path = false;
  a.running = true;
}

int main() {
  const int cols = 35;
  const int rows = 25;
  const int cell = 24;

  sf::RenderWindow window(sf::VideoMode((unsigned)(cols * cell), (unsigned)(rows * cell)), "Pathfinding Visualizer");
  window.setFramerateLimit(60);

  Grid grid(cols, rows);
  Animation anim;

  sf::RectangleShape rect(sf::Vector2f((float)cell - 1.0f, (float)cell - 1.0f));

  auto run_algo = [&](int which) {
    grid.ClearVisitedAndPath();
    auto adj = grid.BuildAdjacencyList();
    PathResult r;
    if (which == 0) r = RunBfs(adj, grid.Start(), grid.End());
    if (which == 1) r = RunDijkstra(adj, grid.Start(), grid.End());
    if (which == 2) r = RunAStarGrid(adj, grid.Cols(), grid.Rows(), grid.Start(), grid.End());
    StartAnimation(anim, r);
  };

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window.close();
      if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) window.close();
        if (event.key.code == sf::Keyboard::R) grid.ClearVisitedAndPath();
        if (event.key.code == sf::Keyboard::C) grid.ClearAll();
        if (event.key.code == sf::Keyboard::B) run_algo(0);
        if (event.key.code == sf::Keyboard::D) run_algo(1);
        if (event.key.code == sf::Keyboard::A) run_algo(2);
      }

      if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        if (anim.running) continue;
        int mx = event.mouseButton.x / cell;
        int my = event.mouseButton.y / cell;
        if (!grid.InBounds(mx, my)) continue;
        int idx = grid.Index(mx, my);

        bool shift = sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) ||
                     sf::Keyboard::isKeyPressed(sf::Keyboard::RShift);
        bool ctrl = sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) ||
                    sf::Keyboard::isKeyPressed(sf::Keyboard::RControl);

        if (shift) {
          grid.SetStart(idx);
        } else if (ctrl) {
          grid.SetEnd(idx);
        } else {
          if (idx == grid.Start() || idx == grid.End()) continue;
          if (grid.Get(idx) == CellType::Wall)
            grid.Set(idx, CellType::Empty);
          else
            grid.Set(idx, CellType::Wall);
        }
      }
    }

    // Animate
    if (anim.running) {
      // speed: update a few nodes per frame
      for (int step = 0; step < 8; step++) {
        if (!anim.showing_path) {
          if (anim.visit_i < anim.visit.size()) {
            int idx = anim.visit[anim.visit_i++];
            if (idx != grid.Start() && idx != grid.End() && grid.Get(idx) == CellType::Empty) {
              grid.Set(idx, CellType::Visited);
            }
          } else {
            anim.showing_path = true;
          }
        } else {
          if (anim.path_i < anim.path.size()) {
            int idx = anim.path[anim.path_i++];
            if (idx != grid.Start() && idx != grid.End()) {
              grid.Set(idx, CellType::Path);
            }
          } else {
            anim.running = false;
            break;
          }
        }
      }
    }

    window.clear(sf::Color(11, 16, 32));
    for (int y = 0; y < rows; y++) {
      for (int x = 0; x < cols; x++) {
        int idx = grid.Index(x, y);
        rect.setPosition((float)(x * cell), (float)(y * cell));
        rect.setFillColor(ColorFor(grid.Get(idx)));
        window.draw(rect);
      }
    }
    window.display();
  }

  return 0;
}

