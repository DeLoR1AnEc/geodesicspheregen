# Geodesic sphere generator
This tool will help you add some spheres to minecraft!

# Usage
```.\spheregen.exe <subdivisions> <radius> <offset x> <offset y> <offset z> <output file>```
Please note that subdivisions must integer.
Radius and offsets may be floats.

Copy text from generated file to your mcfunction.

# Build
```gcc \src\spheregenv<current version>.c -o \spheregen```
You can use any other build system if you want so.

# Examples
[```.\spheregen.exe 1 3 0 0 0 minecraft:flame .\example\output.txt```](example/screenshots/0.png)
[```.\spheregen.exe 2 7 0 1.62 0 minecraft:flame .\example\output.txt```](example/screenshots/1.png)

# License
GNU General Public License v3.0