# Siege of Nodes

Тактическая tower defense / roguelite с процедурной картой на C++. Игрок защищает «ядро» сети от волн врагов, которые движутся по графу узлов. Башни, способности и враги — объекты с полиморфным поведением. Рендер — 2D (OpenGL + GLSL), геймплей — с фокусом на алгоритмах, структурах данных, многопоточности и асинхронности.

## Цели обучения

- **Алгоритмы** — pathfinding, spatial queries, симуляция событий
- **Структуры данных** — heap, map, stack и собственные реализации
- **Многопоточность** — job system, разделение sim/render
- **Асинхронность** — загрузка ассетов, отложенные вычисления
- **ООП** — полиморфизм, паттерны проектирования, RAII
- **Шейдеры** — vertex и fragment (GLSL)

## Геймплей

Игрок размещает башни на узлах графа и защищает центральное ядро от волн врагов. Враги ищут путь к ядру по процедурно сгенерированной карте. Между волнами — улучшения, асинхронная загрузка следующего уровня. Визуализация угрозы и зон действия башен — через шейдеры (influence map, post-process).

## Алгоритмы

| Алгоритм | Применение |
|----------|------------|
| A* / Dijkstra | Поиск пути врагов по графу узлов |
| Flood fill / BFS | Зона действия башни, распространение эффектов |
| Sweep & prune / spatial hash | Broadphase для снарядов и коллизий |
| Topological sort | Порядок обновления систем (баффы → урон → смерть) |
| Min/max heap | Очередь событий симуляции |
| Quadtree | Выбор целей для башен в больших волнах |

## Структуры данных

Собственные реализации в `core/containers/` (с юнит-тестами и бенчмарками против STL):

| Структура | Назначение |
|-----------|------------|
| Binary heap | `EventQueue`: таймеры спавна, отложенные способности |
| Hash map (open addressing / chaining) | `EntityId → Component`, кэш путей |
| Stack | Undo в режиме паузы, DFS при генерации лабиринта |
| Ring buffer | Очередь команд игрока без input lag |
| Priority queue + intrusive list | Object pool (снаряды, частицы) без `new` каждый кадр |
| Graph (adjacency list) | Карта уровня, маршруты врагов |

## Многопоточность

```
[Main thread]     рендер, input, UI
[Sim thread]      pathfinding batch, AI scoring
[Loader thread]   ассеты, компиляция шейдеров
[Worker pool]     генерация волны, пересчёт influence map
```

- **Job system** (thread pool + work stealing) — параллельный A* для большого числа врагов
- **Double buffering** — sim пишет в `WorldFrame N`, render читает `N-1`
- **Mutex / lock-free queue** — передача команд из input в sim
- **Snapshot path cache** — sim обновляет, render не блокируется

## Асинхронность

- **`std::future` / coroutines (C++20)** — загрузка уровня с экраном ожидания
- **Async asset pipeline** — текстуры и звук между волнами
- **Deferred pathfinding** — запрос пути не блокирует кадр
- **Command buffer** — input в очередь, sim обрабатывает на следующем тике (`dt = 1/60`)

## ООП

ECS-lite или классический OOP с чёткими слоями:

```cpp
class ITowerBehavior {
public:
    virtual Target pickTarget(const WorldView& world) = 0;
    virtual void onTick(SimulationContext& ctx) = 0;
};

class ChainLightningTower : public ITowerBehavior { /* ... */ };
class SlowAuraTower     : public ITowerBehavior { /* ... */ };
```

Паттерны: **Strategy** (башни, враги), **Observer** (события → UI/звук), **Factory + Registry** (создание из JSON), **State** (FSM врага), **RAII** (`ScopedSimLock`, `GpuBuffer`, `ShaderProgram`).

## Шейдеры

**Vertex shader:**

- Instanced rendering тайлов карты
- Анимация пульсации узла при атаке (`sin` по `u_time`)
- Displacement по influence map

**Fragment shader:**

- Influence map — heatmap угрозы
- Tower range — круг с `smoothstep` falloff
- Post-process — vignette при низком HP ядра, chromatic aberration при перегрузке
- Outline — Sobel по depth/id buffer для выделения башен

Hot-reload шейдеров через loader thread.

## Структура проекта

```
siege-of-nodes/
├── core/
│   ├── containers/     # heap, hash_map, ring_buffer, object_pool
│   ├── threading/      # job_system, mpsc_queue
│   └── async/          # task_graph, coroutine_loader
├── gameplay/
│   ├── algorithms/     # pathfinding, spatial, influence
│   ├── entities/       # Tower, Enemy, Core
│   └── simulation/     # fixed tick, event queue
├── render/
│   ├── shaders/        # .vert, .frag
│   └── systems/        # batching, postprocess
├── tools/
│   └── level_gen/      # procedural graph generator
└── tests/              # Catch2 / Google Test
```

## Стек

- **C++20** (coroutines опционально)
- **CMake** + **vcpkg** (glfw3, glad, glm, catch2)
- **OpenGL 4.x** + GLSL
- **EnTT** (опционально) или свой лёгкий ECS

## Альтернативные концепции

1. **Dungeon Scheduler** — roguelike с turn-based симуляцией (heap / event queue)
2. **Shader Arena** — арена-шooter с фокусом на GPU (particles, trails)
3. **Network Breaker** — puzzle: соединение узлов, враги по потокам (графы + topological sort)

## Документация

План разработки по этапам — в [ROADMAP.md](ROADMAP.md).
