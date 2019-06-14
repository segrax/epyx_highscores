# Epyx World Records

Importing and archiving records from Epyx Games series (Summer Games, Summer Games II, Winter Games, World Games, California Games)

## Description

This tool can be used to import World Records from the C64 Epyx games, both disk and the "Epyx Games Collection" cartridge release are supported.
It will search every block of a D64, hunting for deleted world records, as well as loading the current records.
These records are then stored in a JSON 


## Usage

```
Epyx World Records: Keep your world records
Usage:
  epyx_worldrecords [OPTION...]

      --about                   About
  -h, --help                    Help
      --games                   List known games
      --filter-game "World Games"
                                Filter records for game (default: )
      --filter-name "My Name"   Filter records for name (default: )
      --list-all                List records
      --import-path "Path"      Search Path (default: )
      --import-cart "Path.crt"  Import from the Epyx cartridge (default: )
```


## Example Import from current directory

```
epyx_worldrecords
Importing from path: D:\Projects\epyx_worldrecords\run
Searching for D64 files
 Summer.Games.2-1.d64
SUMMER GAMES II
         Triple Jump: JTH4 18.7. -     17.35m
              Rowing:     ROBERT -     0:27.0
             Javelin:     ROBERT -     101.31
          Equestrian:      ROBYN -          0
           High Jump: JTH1 19.7. -      2.50m
             Cycling: JTH 16.10. -     0:28.5
            Kayaking:  JTH2 5.8. -       1:27

          Equestrian:     ROBERT -          0
 Summer.Games.2-2.d64
 Summer.Games.d64
SUMMER GAMES
          Pole Vault:     ROBERT -      5.70m
     Platform Diving:    KATHRYN -     576.60
        4x400m Relay:     ROBERT -     2:25.2
           100m Dash:     ROBERT -      09.67
          Gymnastics:     ROBERT -     13.0
     Freestyle Relay:     ROBERT -     1:41.4
      100m Freestyle:    KATHRYN -     0:24.6
      Skeet Shooting:     ROBERT -         25

     Platform Diving:     ROBERT -     472.00
           100m Dash:     ROBERT -      09.75
          Gymnastics:   U.S.S.R. -     11.9
     Freestyle Relay:     ROBERT -     1:44.2
      100m Freestyle:     ROBERT -     0:25.3
      Skeet Shooting:     ROBERT -         23
          Gymnastics:     ROBERT -     12.9
          Pole Vault:     U.S.A. -      4.50m
     Platform Diving:      ITALY -     283.35
        4x400m Relay:      SPAIN -     3:11.8
           100m Dash:     U.S.A. -      10.47
     Freestyle Relay:     BRAZIL -     1:48.6
      100m Freestyle:      JAPAN -     0:26.4
      Skeet Shooting:     NORWAY -         12
 Winter.Games-1.d64
WINTER GAMES
             Hot Dog:    KATHRYN -        9.6
            Biathlon:     ROBERT -       1:43
      Figure Skating:      ROBYN -        2.8
            Ski Jump:     ROBERT -      217.4
       Speed Skating:      ROBYN -     0:26.5
        Free Skating:      ROBYN -        3.0
             Bobsled:     ROBERT -      21.74

             Hot Dog:      ROBYN -        9.3
 Winter.Games-2.d64
WINTER GAMES
        Free Skating:      ROBYN -        3.4

 World.Games-1.d64
WORLD GAMES
      Weight Lifting:     ROBERT -     330 KG
      Barrel Jumping:     ROBERT -         17
        Cliff Diving:     ROBERT -        100
       Slalom Skiing:     ROBERT -     5:54:0
         Log Rolling:      ROBYN -        318
         Bull Riding:     ROBERT -         00
          Caber Toss:     ROBERT -     38' 2"
      Sumo Wrestling:     ROBERT -       0000

      Weight Lifting:     ROBERT -     320 KG
 World.Games-2.d64
Searching for CRT files
 epyx_games_collection.crt
```

## Example Listing

```
epyx_worldrecords.exe --list-all

Listing Records

SUMMER GAMES
          Pole Vault:     ROBERT -      5.70m on 2019-06-13
     Platform Diving:    KATHRYN -     576.60 on 2019-06-13
     Platform Diving:     ROBERT -     472.00 on 2019-06-13
        4x400m Relay:     ROBERT -     2:25.2 on 2019-06-13
           100m Dash:     ROBERT -      09.67 on 2019-06-13
           100m Dash:     ROBERT -      09.75 on 2019-06-13
          Gymnastics:     ROBERT -     13.0   on 2019-06-13
          Gymnastics:     ROBERT -     12.9   on 2019-06-13
     Freestyle Relay:     ROBERT -     1:41.4 on 2019-06-13
     Freestyle Relay:     ROBERT -     1:44.2 on 2019-06-13
      100m Freestyle:    KATHRYN -     0:24.6 on 2019-06-13
      100m Freestyle:     ROBERT -     0:25.3 on 2019-06-13
      Skeet Shooting:     ROBERT -         25 on 2019-06-13
      Skeet Shooting:     ROBERT -         23 on 2019-06-13

SUMMER GAMES II
              Rowing:     ROBERT -     0:27.0 on 2019-06-13
             Javelin:     ROBERT -     101.31 on 2019-06-13
          Equestrian:      ROBYN -          0 on 2019-06-13
          Equestrian:     ROBERT -          0 on 2019-06-13

WINTER GAMES
             Hot Dog:    KATHRYN -        9.6 on 2019-06-13
             Hot Dog:      ROBYN -        9.3 on 2019-06-13
            Biathlon:     ROBERT -       1:43 on 2019-06-13
      Figure Skating:      ROBYN -        2.8 on 2019-06-13
            Ski Jump:     ROBERT -      217.4 on 2019-06-13
       Speed Skating:      ROBYN -     0:26.5 on 2019-06-13
        Free Skating:      ROBYN -        3.4 on 2019-06-13
        Free Skating:      ROBYN -        3.0 on 2019-06-13
        Free Skating:     ROBERT -        3.0 on 2019-06-13
             Bobsled:     ROBERT -      21.74 on 2019-06-13

WORLD GAMES
      Weight Lifting:     ROBERT -     330 KG on 2019-06-13
      Weight Lifting:     ROBERT -     320 KG on 2019-06-13
      Barrel Jumping:     ROBERT -         17 on 2019-06-13
        Cliff Diving:     ROBERT -        100 on 2019-06-13
       Slalom Skiing:     ROBERT -     5:54:0 on 2019-06-13
         Log Rolling:      ROBYN -        318 on 2019-06-13
         Bull Riding:     ROBERT -         00 on 2019-06-13
          Caber Toss:     ROBERT -     38' 2" on 2019-06-13
      Sumo Wrestling:     ROBERT -       0000 on 2019-06-13

CALIFORNIA GAMES
```
