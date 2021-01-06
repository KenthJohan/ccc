# Experimental entity component system

## Files
* ../master/csc_ecs.h
* ../master/test_csc_ecs.h


## Data structures

### Chunks
1. Define chunk size to 14 bytes: `CSC_ECS_CHUNK_SIZE = 14`<br>
2. Add component A with size 1 bytes: `ecs.components.size[A] = 1`
2. Add component B with size 2 bytes: `ecs.components.size[B] = 2`
2. Add component C with size 4 bytes: `ecs.components.size[C] = 4`
2. Add chunk with components (AB): `csc_ecs_add_chunk (ecs, (AB), 2)`
3. Add chunk with components (CBA): `csc_ecs_add_chunk (ecs, (CBA), 3)`
4. Add entity with components (AB): `csc_ecs_gen_entity (ecs, AB)`
5. Add entity with components (AB): `csc_ecs_gen_entity (ecs, AB)`
6. Add entity with components (ABC): `csc_ecs_gen_entity (ecs, ABC)`
7. Add entity with components (ABC): `csc_ecs_gen_entity (ecs, ABC)`
8. Add entity with components (AB): `csc_ecs_gen_entity (ecs, AB)`
![csc_ecs1](../master/csc_ecs1.png)

### Components

### Entities


## Questions
* Support Array of Structures of Arrays? https://en.wikipedia.org/wiki/AoS_and_SoA
* Support arbitrary amount of components instead or 64?




## Insperational sources
* https://gitlab.com/Falconerd/ember-ecs/-/blob/master/example/main.c
* https://github.com/discoloda/Diana/blob/master/cpp/example.cpp
* https://skypjack.github.io/2020-08-02-ecs-baf-part-9/
* https://skypjack.github.io/2019-09-25-ecs-baf-part-5/
* https://stackoverflow.com/questions/6076720/sorting-a-c-array-based-on-contents-of-another-array
* http://www.manpagez.com/man/3/qsort_r/
* https://en.wikipedia.org/wiki/Entity%E2%80%93attribute%E2%80%93value_model#Description_of_concepts
* https://www.gamedev.net/forums/topic/691504-ecs-mega-entity-array-vs-many-component-arrays/
* https://github.com/cntools/rawdraw
* https://www.geeksforgeeks.org/sparse-set/
* https://programmingpraxis.com/2012/03/09/sparse-sets/
* https://rams3s.github.io/blog/2019-01-09-ecs-deep-dive/
* https://github.com/BrianWill/LearnUnity/blob/master/ecs-jobs/ecs.md
* https://medium.com/@5argon/unity-ecs-creating-an-efficient-system-with-chunk-iteration-didaddorchange-didchange-221427f5361b
* https://stackoverflow.com/questions/10082517/simplest-tool-to-measure-c-program-cache-hit-miss-and-cpu-time-in-linux
* https://tech.innogames.com/unitys-performance-by-default-under-the-hood/
* http://t-machine.org/index.php/2014/03/08/data-structures-for-entity-systems-contiguous-memory/
* https://stackoverflow.com/questions/23473783/entity-component-system-framework-that-is-cpu-cache-friendly
* More Data, Less Problems: an opinionated introduction to Data Oriented Design and Development https://www.youtube.com/watch?v=n9CD4zdeZto
* Unity DOTS (Data-Oriented Technology Stack) overview https://www.youtube.com/watch?v=EGKmNQL9CcM
* Unity ECS (Entity Component System) - 1 of 2 https://www.youtube.com/watch?v=OqzUr-Rg6w4
* Building a Data-Oriented Future - Mike Acton https://www.youtube.com/watch?v=u8B3j8rqYMw
* Data-Oriented Demo: SOA, composition https://www.youtube.com/watch?v=ZHqFrNyLlpA
* https://www.chessprogramming.org/General_Setwise_Operations
* Discussion of SIMD, SOA, AOSOA. Followed by Questions & Answers. https://www.youtube.com/watch?v=YGTZr6bmNmk