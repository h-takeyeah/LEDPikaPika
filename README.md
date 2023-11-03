# LED pikapika

My first project using [Seeed Studio XIAO RP2040](https://www.seeedstudio.com/XIAO-RP2040-v1-0-p-5026.html) and [arduino-cli](https://github.com/arduino/arduino-cli).

[working example](https://github.com/h-takeyeah/LEDPikaPika/assets/61489178/15299cf7-28d6-4e94-8231-62c5e15c0c9d)

## Before you begin to develop on VS Code
I recommend to disable VS Code Arduino plugin will not generate IntelliSense(You can compile by arduino-cli and code verification works well even if you skip this step while VS Code C++ Extension stil complains about missing header files).

- [see here](https://github.com/earlephilhower/arduino-pico/issues/753)
  - I used `.vscode/arduino.json` steps below instead of `.vscode/settings.json` which is used at the cited page, but same result I got.

### Notification about my environment
Since my PC may be different from your environment, plase according to your own environment. Let me show my Environment.
```bash
$ uname
Linux # Arch Linux
$ /usr/bin/arduino-cli version
arduino-cli  Version: 0.34.2-arch Commit: 963c1a76c9d2a6ea37956a100c0cd8070260208f Date:

$ cat .vscode/settings.json
{
    "arduino.commandPath": "arduino-cli",
    "arduino.path": "/usr/bin",
    "arduino.useArduinoCli": true,
    "arduino.additionalUrls": [
        "https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json"
    ]
}

# note: It's standalone arduino-cli not a VS Code Arduino Extension bundled one.
```

### Steps
1. Temporary enable automatic IntelliSense generation
```jsonc
// .vscode/arduino.json
{
  "intelliSenseGen": "enable"
}
```

2. Open `LEDPikaPika.ino`, and run command `Arduino: Verify` on command pallete or clicking the button right top corner. After this step, `.vscode/c_cpp_properties.json` will be created and you will see include error, but we will fix them later.

3. Turn off automatic IntelliSense generation again(Do it manually or git restore). Result:
```jsonc
// .vscode/arduino.json
{
  "intelliSenseGen": "disable"
}
```

4. Find full list of include path. Go to folder where arduino-pico library (referred to as `rp2040`) is installed and find `lib/platform_inc.txt`. You can also find same one at [platform_inc.txt(version 3.6.0)](https://github.com/earlephilhower/arduino-pico/blob/3.6.0/lib/platform_inc.txt).
```plain
# For my PC, files was placed at here
~/.arduino15/internal/rp2040_rp2040_3.6.0_794e66db9afdfc56/lib/platform_inc.txt
```

5. Copy `platform_inc.txt` to somewhere and open it in editor. Replace `-iwithprefixbefore` at the start of each lines with the path to them. The `includePath` of `.vscode/c_cpp_properties.json` which was generated at above step will give you a hint how to write them.
```bash
# Refer to .vscode/c_cpp_properties.json each element is written as absolute path. So we must replace the prefix `-iwithprefixbefore` with path to the folder.

# Copy
$ cp ~/.arduino15/internal/rp2040_rp2040_3.6.0_794e66db9afdfc56/lib/platform_inc.txt /tmp/platform_inc.txt

# For example, username is john
$ sed -i 's/^-iwithprefixbefore/\/home\/john\/.arduino15\/internal\/rp2040_rp2040_3.6.0_794e66db9afdfc56/' /tmp/platform_inc.txt
```

6. Update `includePath` of `.vscode/c_cpp_properties.json` retaining original `includePath` entries. Save it.
```jsonc
{
    "configurations": [
        {
            "includePath": [
                "...", // original (do not remove!)
                "/home/john/.arduino15/..." // append
            ]
        }
    ]
}
```

7. Do step 2 again, and make sure that IntelliSense no longer complains.

## Before you begin to develop on NeoVim
- nvim-lspconfig
- arduino-config
- [arduino/arduino-language-server](https://github.com/arduino/arduino-language-server)

I installed arduino-language-server by [Mason](https://github.com/williamboman/mason.nvim) and add setting to enable the LSP:
```lua
require'lspconfig'.arduino_language_server.setup{}
```

See `h: lspconfig-server-configurations` for more information.

## Notes

### arduino-cli
_How to prepare files from scratch to setup a project like this repo._

- **sketch.yaml** contains almost all information about this project.

Initialization:
```bash
$ arduino-cli sketch new LEDPikaPika
Sketch created in: /path/to/LEDPikaPika
$ cd $_
$ tree
.
└── LEDPikaPika.ino

1 directory, 1 file
```

Add board:
```plain
arduino-cli core install --additional-urls https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json rp2040:rp2040
```

> This command downloads and place rp2040 code at `~/.arduino15/packages/rp2040/hardware/` not at `~/.arduino15/internal`.

First compilation without `sketch.yaml`:
```plain
$ arduino-cli compile --only-compilation-database -b rp2040:rp2040:seeed_xiao_rp2040 --dump-profile .
Used platform Version Path
rp2040:rp2040 3.6.0   /home/john/.arduino15/packages/rp2040/hardware/rp2040/3.6.0

profiles:
  seeed_xiao_rp2040:
    fqbn: rp2040:rp2040:seeed_xiao_rp2040
    platforms:
      - platform: rp2040:rp2040 (3.6.0)
        platform_index_url: https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
```

After compilation copy profiles paste to `sketch.yaml`:
```diff
+profiles:
+  seeed_xiao_rp2040:
+    fqbn: rp2040:rp2040:seeed_xiao_rp2040
+    platforms:
+      - platform: rp2040:rp2040 (3.6.0)
+        platform_index_url: https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
```

And set `default_profile`:
```diff
        platform_index_url: https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
+default_profile: seeed_xiao_rp2040
```

After these configuration, the compile sequence become very simple:
```plain
arduino-cli compile .
```

Uploading sequence also become simple:
```plain
arduino-cli upload -p /dev/ttyACM0 .
```

Upload after compile in one command:
```plain
arduino-cli compile -u -p /dev/ttyACM0 .
```

### Where platforms are stored
- `~/.arduino15/platforms`
- This is default value and can be customized by editing `~/.arduino15/arduino-cli.yaml`
  - That file can be created by `arduino-cli config init`

When after cloning this repo and run `arduino-cli compile .` (with no `~/.arduino15` for the first time?), `arduino-cli` downloads platform codes into `~/.arduino15/internal` instead of `~/.arduino15/platforms`.
