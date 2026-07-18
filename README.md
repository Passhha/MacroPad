# MacroPad 

Firmware for an Arduino Pro Micro macro pad with a 4x5 key matrix, GC9A01 display,
encoder, one WS2812 LED and USB HID.

## Build and upload

Open the project in PlatformIO and choose **Upload** for the
`sparkfun_promicro16` environment.

## Configuring bindings

Bindings are stored in EEPROM. They survive reset and power loss. Open Serial
Monitor at 115200 baud and terminate commands with a newline.

```
STATE
MODE SYSTEM
MODE CHROME
MODE DISCORD
BIND <mode> <key> <type> <modifiers> <code>
DEFAULTS
```

`mode` is `0` for System, `1` for Chrome and `2` for Discord. `key` is from
`0` to `19`, counting left-to-right and then top-to-bottom in the matrix.

Binding types:

- `0` — disabled (`modifiers` and `code` must be zero)
- `1` — USB keyboard key
- `2` — media/consumer HID action
- `3` — open/close the Settings screen (`modifiers` and `code` must be zero)

For keyboard bindings, `modifiers` is a bit mask: Ctrl=`1`, Shift=`2`,
Alt=`4`, GUI/Win=`8`. `code` is a USB HID key usage: A=`4`, C=`6`, S=`22`,
V=`25`, Z=`29`, F13=`104` through F24=`115`.

For media bindings, use `modifiers=0`. Common consumer codes are Mute=`226`,
Volume up=`233`, Volume down=`234`, Play/Pause=`205`, Next=`181`, Previous=`182`.

For example, this assigns Ctrl+Shift+S to the first key in System mode:

```
BIND 0 0 1 3 22
```

This assigns Play/Pause to the same key in Chrome mode:

```
BIND 1 0 2 0 205
```

`DEFAULTS` restores the supplied factory layout in all modes.

## Windows configurator

The `tools/MacroPadConfigurator` project provides a graphical editor. Build it
with `dotnet build tools/MacroPadConfigurator/MacroPadConfigurator.csproj`, then
run `MacroPadConfigurator.exe` from its `bin/Debug/net10.0-windows` folder.
Select the MacroPad COM port, mode, physical key and action, then click
**Apply binding**. The application writes the setting to EEPROM immediately.
