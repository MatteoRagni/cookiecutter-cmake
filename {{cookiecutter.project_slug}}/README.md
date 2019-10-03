Project {{cookiecutter.project_name}}
=====================================

This project has been generated with:

![Cookiecutter](https://raw.githubusercontent.com/cookiecutter/cookiecutter/3ac078356adf5a1a72042dfe72ebfa4a9cd5ef38/logo/cookiecutter_medium.png)

and template: https://gitlab.com/maciej.matuszak/cookiecutter-cpp-modern-cmake-lib

Requirements
------------
* cmake 3.8+
* make
* gcc or similar capable of compiling C++11
{% if cookiecutter.use_docopts == 'Y' -%} * [docopts.cpp](https://github.com/docopt/docopt.cpp) {% endif %}

Dependencies
------------
The [docopts.cpp](https://github.com/docopt/docopt.cpp) library can be installed globally or if you do not want to pollute your system with multiple version of libraries those can be installed along side this project: `../{{cookiecutter.project_slug}}_dependencies`
Use cmake subproject to build and install dependencies:
 ```shell script
cd dependencies/build
cmake ..
make
```
The dependencies will be build and installed to: `../{{cookiecutter.project_slug}}_dependencies`
in CMakeLists.txt there is entry added to `CMAKE_PREFIX_PATH` variable so the find_package can find it.

Add another dependency
----------------------
To add another dependency without installing it system wide you can use the following pattern:
* look at the `dependencies/CMakeLists.txt`. 
* add another entry using `ExternalProject_Add` cmake function
  * Make sure you use the same prefix: `PREFIX ${${PROJECT_NAME}_DIR}`
  * and same install location, if using cmake then `CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR>` will do it
* link your new dependency to main target as as it is done for docopts_ext: `add_dependencies(${PROJECT_NAME} docopt_ext)`
* build the dependencies again
 


Building
-----
Install dependencies if needed. 
Build process :
```shell script
mkdir build
cd build
cmake ..
make -j
```
Install / Uninstall
-------------------
Cmake install and uninstall targets are provided
```shell script
sudo make install
sudo make uninstall
```

Execution
---------

```shell script
$./{{cookiecutter.project_slug}}_exe 
Argument Parsed: 
 --help: false
 --version: false

$./{{cookiecutter.project_slug}}_exe --version
0.1.0

```     

License
-------
This project is licensed under the terms of the [MIT License](/LICENSE)
