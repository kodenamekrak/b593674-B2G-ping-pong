# b593674-B2G-ping-pong

## Windows

Clone the project and build using Visual Studio 2022

## Linux

### Installing dependencies 

#### Ubuntu
```
apt install build-essential meson libx11-dev

```

#### Arch Linux
```
pacman -S base-devel meson libx11
```


### Build and run
```
meson setup build -Dbuild_type=release && meson compile -C build
./build/pingpong
```