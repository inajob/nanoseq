# nanoseq
nanoseq is nano size sequencer for Arduboy ( https://www.arduboy.com/ )

## screen
![screenshot](imgs/nanoseq.jpg)
- Vertical lines means each 8 beats.
- Horizontal lines means each 1 octaves.

## how to use
- Move cursor
  - allow keys
- Set tone at cursor position
  - A button
- Play music
  - Move cursor at the most left side and press left button.
- Semitone
  - Press A button and Up or Down key.

## how to compile

compile

```
$ platformio run
```

compile and upload

```
$ platformio run --target upload
```

## Changelog
- 2017/02/19
  - support platformio
  - include hex binary
- 2016/09/22
  - support semitone

