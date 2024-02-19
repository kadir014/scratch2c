# scratch2c
Scratch3 interface in C and transpiler.
The C API for Scratch uses SDL2 and its extensions.



# Blocks
Scratch blocks included in the C API:
- [Motion](#motion)
- [Looks](#looks)
- [Sound](#sound)
- [Events](#events)
- [Control](#control)
- [Sensing](#sensing)
- [Operators](#operators)
- [Variables](#variables)
- [Lists](#lists)
- [Pen](#pen)

## Motion
| Scratch | C | Implemented | Notes |
|---------------------------------|----------------------------|:-----------:|-----------------------------------------|
| Move `10` steps | `scSprite_move()` | ✔️ |  |
| Turn ↻ `15` degrees | `scSprite.angle += 15.0` | ✔️ | |
| Turn ↺ `15` degrees | `scSprite.angle -= 15.0` | ✔️ | |
| Go to `location` | - | ❌ | |
| Go to x: `0` y: `0` | `scSprite_goto()` | ✔️ |  |
| Glide `1` secs to `location` | - | ❌ | |
| Glide `1` secs to x: `0` y: `0` | - | ❌ |  |
| Point in direction `90` | `scSprite.angle = 90.0` | ✔️ |  |
| Point towards `location` | - | ❌ | |
| Set x to `0` | `scSprite.x = 0.0` | ✔️ |  |
| Set y to `0` | `scSprite.y = 0.0` | ✔️ |  |
| Change x by `10` | `scSprite.x += 10.0` | ✔️ |  |
| Change y by `10` | `scSprite.y += 10.0` | ✔️ |  |
| If on edge, bounce | - | ❌ |  |
| Set rotation style `rotation` | - | ❌ | |
| X position | `scSprite.x` | ✔️ | |
| Y position | `scSprite.y` | ✔️ | |
| Direction | `scSprite.angle` | ✔️ | |

## Looks
| Scratch | C | Implemented | Notes |
|--------------------------------|-----------------------------------|:----------:|----------------------------------------------|
| Say `Hello!` | - | ❌ |  |
| Say `Hello!` for `2` seconds | - | ❌ |  |
| Think `Hmm.` | - | ❌ |  |
| Think `Hmm.` for `2` seconds | - | ❌ |  |
| Switch costume to `costume1` | - | ❌ |  |
| Next costume | - | ❌ |  |
| Switch backdrop to `backdrop1` | - | ❌ |  |
| Next backdrop | - | ❌ |  |
| Set size to `100`% | - | ❌ |  |
| Change size by `10` | - | ❌ |  |
| Set `effect` effect to `0` | - | ❌ |  |
| Change `effect` effect by `25` | - | ❌ |  |
| Clear graphic effects | - | ❌ |  |
| Show | `scSprite_show()` | ✔️ |  |
| Hide | `scSprite_hide()` | ✔️ |  |
| Go to `front` layer | - | ❌ |  |
| Go `forward` `1` layers | - | ❌ |  |
| Costume `number` | - | ❌ |  |
| Backdrop `number` | - | ❌ |  |
| Size | - | ❌ |  |

## Sound
I don't plan any support for audio for a while!

## Events
| Scratch | C | Implemented | Notes |
|------------------------|----------------------------|:----------:|-------------------------------------------|
| When flag clicked | - | ✔️ | This is the entry point of the program. |
| When `space` key pressed | - | ❌ |  |
| When this sprite clicked | - | ❌ |  |
| When backdrop switches to `backdrop` | - | ❌ |  |
| When `loudness` > `10` | - | ❌ |  |
| When I receive `message1` | - | ❌ |  |
| Broadcast `message1` | - | ❌ |  |
| Broadcast `message1` and wait | - | ❌ |  |

## Control
| Scratch | C | Implemented | Notes |
|------------------------|----------------------------|:----------:|-------------------------------------------|
| Wait `1` seconds | - | ❌ |  |
| Repeat `10` | - | ✔️ |  |
| If `condition` then | - | ✔️ |  |
| If `condition` then, else | - | ✔️ |  |
| Wait until `condition` | - | ❌ |  |
| Repeat until `condition` | - | ✔️ |  |
| While `condition` | - | ✔️ |  |
| Stop `all` | - | ✔️ | Only for exiting the program. |
| When I start as a clone | - | ❌ |  |
| Create clone of `myself` | - | ❌ |  |
| Delete this clone | - | ❌ |  |

## Sensing
| Scratch | C | Implemented | Notes |
|------------------------|----------------------------|:----------:|-------------------------------------------|
| Touching `thing`? | - | ❌ |  |
| Touching color `color`? | - | ❌ |  |
| `color` is touching `color`? | - | ❌ |  |
| Distance to `thing` | - | ❌ |  |
| Ask `something` and wait | - | ❌ |  |
| Answer | - | ❌ |  |
| Key `space` pressed? | - | ❌ |  |
| Mouse down? | `mouse_pressed` | ✔️ |  |
| Mouse x | `mouse_x` | ✔️ |  |
| Mouse y | `mouse_y` | ✔️ |  |
| Set drag mode `draggable` | - | ❌ |  |
| Loudness | - | ❌ |  |
| Timer | - | ✔️ |  |
| Reset timer | - | ✔️ |  |
| `value` of `target` | - | ❌ |  |
| Current `date` | `sc_curent_date()` | ✔️ |  |
| Days since 2000 | `sc_days_since_2000()` | ✔️ |  |
| Username | `sc_username()` | ✔️ | Currently just an empty string. |

## Operators
| Scratch | C | Implemented | Notes |
|------------------------|----------------------------|:----------:|-------------------------------------------|
| `x` + `y` | - | ✔️ |  |
| `x` - `y` | - | ✔️ |  |
| `x` * `y` | - | ✔️ |  |
| `x` / `y` | - | ✔️ |  |
| Pick random `1` to `10` | - | ✔️ | TODO: Implement a better PRNG (xiroshiro, mersenne twister) |
| `x` > `y` | - | ✔️ |  |
| `x` < `y` | - | ✔️ |  |
| `x` = `y` | - | ✔️ | |
| `x` and `y` | - | ✔️ |  |
| `x` or `y` | - | ✔️ |  |
| Not `x` | - | ✔️ |  |
| Join `apple` `banana` | - | ❌ |  |
| Letter `1` of `apple` | - | ✔️ |  |
| Length of `apple` | - | ❌ |  |
| `apple` containts `a`? | - | ❌ |  |
| `x` mod `y` | - | ✔️ |  |
| round `x` | - | ✔️ |  |
| `abs` of `x` | - | ✔️ |  |

## Variables
| Scratch | C | Implemented | Notes |
|------------------------|----------------------------|:----------:|-------------------------------------------|
| Set `var` to `0` | - | ❌ |  |
| Change `var` by `1` | - | ❌ |  |
| Show variable `var` | - | ❌ |  |
| Hide variable `var` | - | ❌ |  |

## Lists
| Scratch | C | Implemented | Notes |
|------------------------|----------------------------|:----------:|-------------------------------------------|
| Add `thing` to `list` | - | ❌ |  |
| Delete `1` of `list` | - | ❌ |  |
| Delete all of `list` | - | ❌ |  |
| Insert `thing` at `1` of `list` | - | ❌ |  |
| Replace item `1` of `list` with `thing` | - | ❌ |  |
| Item `1` of `list` | - | ❌ |  |
| Item # of `thing` in `list` | - | ❌ |  |
| Length of `list` | - | ❌ | |
| `list` containst `thing`? | - | ❌ |  |
| Show list `list` | - | ❌ |  |
| Hide list `list` | - | ❌ |  |

## Extension: ✏️ Pen
| Scratch | C | Implemented | Notes |
|------------------------|----------------------------|:----------:|-------------------------------------------|
| Erase all | - | ❌ |  |
| Stamp | - | ❌ |  |
| Pen down | - | ❌ |  |
| Pen up | - | ❌ |  |
| Set pen color to `color` | - | ❌ |  |
| Change pen `color` by `10` | - | ❌ |  |
| Set pen `color` to `50` | - | ❌ |  |
| Change pen size by `1` | - | ❌ | |
| Set pen size to `1` | - | ❌ |  |

# Requirements
Python 3.10+



# License
[MIT](LICENSE) © Kadir Aksoy