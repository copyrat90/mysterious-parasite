# Mysterious Parasite

A 2D grid + turn based roguelike for the [GBA](https://en.wikipedia.org/wiki/Game_Boy_Advance), where you take control over your predator.

**WORK IN PROGRESS, nothing to see for now!**


## Dependencies

This program uses [Butano](https://github.com/GValiente/butano), which is licensed under the zlib license.


## Build

> See my [Butano template README](https://github.com/copyrat90/butano-template/blob/main/README.md) for more detailed info about setting up dev environment.

First, make sure the [Butano](https://github.com/GValiente/butano) is set up correctly, with its own dependencies.

This repo contains [`iso_bn` submodule](https://github.com/copyrat90/iso_butano), so you need to clone this repo recursively.
```bash
git clone --recursive https://github.com/copyrat90/mysterious-parasite.git
```

Next, change [the path in `Makefile`](Makefile#L30) which points to the directory where `butano.mak` is located,
from `LIBBUTANO := ../butano` to your own path.

That should do it, now do `make -j16` and see what happens.

## License

Asset's license differ from each other, see each asset's license from `licenses/assets/*`.

Source codes are generally licensed under the `GNU Affero General Public License v3.0`, See [LICENSE](LICENSE) for details.

    Mysterious Parasite, A 2D grid + turn based roguelike for the GBA, where you take control over your predator.
    Copyright (C) 2022  Guyeon Yu <copyrat90@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

But some sources are licensed differently, you should check each source's header carefully.
