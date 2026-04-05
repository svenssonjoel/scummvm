
# Build

```
./configure --disable-all-engines --enable-engine=lbmdungeon --enable-engine=kyra
```

```                                                                         
make -j$(nproc)                                                               
```

# Run

``` 
./scummvm --path=../dos_games/eob lbmdungeon
```