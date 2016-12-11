
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

