
# Ludum Dare 37

[Ludum Dare entry is here.](http://ludumdare.com/compo/ludum-dare-37/?action=preview&uid=37473)

# Compiling

After cloning the project, you must do
```
git submodule init
git submodule update
```
to get the GameDevTools dependency.

Then make a build directory and build from there.

```
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=./install ..
make
```

(note GameDevTools has glm as a dependency)

# Running the Game

Make sure the res folder is in the same directory as the binary, or else the resources won't load.

