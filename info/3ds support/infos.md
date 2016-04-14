# 3DS support project documentation

## Requirements
- Devkitarm (found on [AUR][devkitarm-bin])
- Citrus-3ds-git (found on [AUR][citrus-3ds])


## Ideas
- Use [citro3d]?
  - Low-level OpenGL-like (with some flavour) API
  - Supports stuff like VBOs.
  - Built over [libctru].
- Use [sf2dlib]?
  - Opens a simple api (check for speed and GPU implementation.)
  - Would be quite easy to plug WRPG's graphisms over it.
- Use [cpp3ds]?
  - Straighter-forward SFML implementation for 3DS.
  - WOuld only have to make things like rendering offset, scaling and clipping.


[devkitarm-bin]: https://aur.archlinux.org/packages/devkitarm-bin/
[citrus-3ds]: https://aur.archlinux.org/packages/citrus-3ds-git/

[citro3d]: https://github.com/fincs/citro3d/
[sf2dlib]: https://github.com/xerpi/sf2dlib
[cpp3ds]: https://github.com/cpp3ds/cpp3ds
