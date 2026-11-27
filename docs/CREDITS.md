# Credits

OmegaDeck is a fork of a fork of a game that's been passed through a lot of hands since 1987.
Everyone below made it possible.

## The game: Omega

- **Laurence R. Brothers**: created Omega (1987–1989) and released Omega 0.90. He also
  gave permission in 2023 for it to be relicensed under GPLv3.
- **Erik Max Francis**, **Nathan Glasser**, **Robert Rendell** and **William Tanksley**:
  maintained and extended Omega over the years, and agreed to the GPLv3 relicence.
- Other known contributors to Omega's code: Sheldon Simms, David Gibbs, Alan L. Bostick,
  William Fang, Djuara Kurniawan, Alan Light, John O'Hare, Daniel Phillips, Ian Richards,
  Kevin W. Thomas and John Tomicich. Possible contributors are John Green, Daniel Packman,
  Tero Jyri Michael Pelander, Rupert G. Goldie, Chris Herborth and Charles William Swiger.
  These lists come from `lib/license.txt`.

## The C++ fork this is built on

- **Lyle Tafoya** ([Lyle-Tafoya/Omega](https://github.com/Lyle-Tafoya/Omega)) did the
  modern C++ port, with bug fixes, balance and UI improvements, and the GPLv3 relicensing
  work. Nearly all of the game code here is his.
- **Josh Backstein** ([joshbackstein](https://github.com/joshbackstein)) helped track down
  the original contributors for the relicence.
- **Matteo Vaccari** ([xpmatteo](https://github.com/xpmatteo)) did the macOS build support,
  and **William Tanksley Jr** sent fixes to the fork.

## OmegaDeck (this fork)

- **somethingthatdoesntdoxme** came up with and directs the handheld port: 2D tiles and
  console-style controls for Steam Deck and other handhelds.
- **Claude** (Anthropic) wrote the SDL2 frontend, curses shim, gamepad and touch controls,
  on-screen keyboard, tile mapping and release workflow, working with the above.

## Art

- **DawnLike** 16×16 tileset by **DragonDePlatino**, using the **DawnBringer** 16-colour
  palette, licensed [CC-BY 4.0](https://creativecommons.org/licenses/by/4.0/).
  The original is on [OpenGameArt](https://opengameart.org/content/dawnlike-16x16-universal-rogue-like-tileset-v181).
- **DawnLikeAtlas** by **Tommy Ettinger** ([tommyettinger](https://github.com/tommyettinger/DawnLikeAtlas)),
  CC-BY 4.0, provided the split, named sprites this port maps to.
- What changed: the sprites were packed into a new atlas (`assets/tiles.png`), and the
  Chaos Sea water is recoloured purple. Nothing else was altered.

## Fonts and libraries

- **DejaVu Sans Mono** by the DejaVu fonts team, which builds on Bitstream Vera by Bitstream,
  Inc. ([Bitstream Vera licence](https://dejavu-fonts.github.io/License.html)). It's
  rendered into the bitmap `assets/font.png`.
- **SDL2** by Sam Lantinga and the SDL contributors ([zlib licence](https://www.libsdl.org/license.php)),
  used for the window, rendering and controller input.

## Licence

The game code is GPLv3 (see `COPYING`), following Lyle Tafoya's relicensing. `lib/license.txt`
covers the history and the permissions received, and `lib/license.original` is Laurence
Brothers' original licence. The art and font keep their own licences, listed above.
