# entt-reactive
An experimental sprite rendering setup utilizing pooled SSBO's, a multithreaded setup based on Even Todd's [The Poor Man's Threading Architecture](http://etodd.io/2016/01/12/poor-mans-threading-architecture/), array-texture based sprite cache generation to allow for bindless per-sprite rendering, and EnTT's reactive systems to detect transform/sprite changes and generate update commands for the pools.

This repo was originally an experimental private repo, and as a result it is a bit dirty with not much documentation.