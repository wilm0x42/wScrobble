# wScrobble
`wScrobble` is an mpv plugin that utilizes GVFS metadata to keep track of how many times a file has been played, and
likewise updates the GVFS metadata to display emblems in Thunar file manager, to give an impression of how often
a file is played.

Mpv must be compiled with cplugin support.

The emblems go as follows:
* >= 3 plays: white emblem
* >= 10 plays: blue emblem
* >= 30 plays: violet emblem

To read and manipulate the playcount value, the `gio` utility can be used as follows:

```sh
gio info --attributes=metadata::playcount ./song.mp3
gio set ./song.mp3 metadata::playcount 0
```

## Build
Run `make`, the included Makefile should do the rest.

## Install
Run `make install` to automatically copy the object file in `$HOME/.config/mpv/scripts`, which will enable it for your user.
